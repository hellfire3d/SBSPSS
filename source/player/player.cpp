/*=========================================================================

	player.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

/*----------------------------------------------------------------------
	Includes
	-------- */

#include "player\player.h"

#ifndef __PAD_PADS_H__
#include "pad\pads.h"
#endif

#ifndef	__GAME_GAMESLOT_H__
#include "game\gameslot.h"
#endif

#ifndef __LAYER_COLLISION_H__
#include "level\layercollision.h"
#endif

#ifndef	__PLAYER_PMODES_H__
#include "player\pmodes.h"
#endif

#ifndef	__PLAYER_PMBLOON_H__
#include "player\pmbloon.h"
#endif

#ifndef	__PLAYER_PMBUBBLE_H__
#include "player\pmbubble.h"
#endif

#ifndef	__PLAYER_PMNET_H__
#include "player\pmnet.h"
#endif

#ifndef	__PLAYER_PMCHOP_H__
#include "player\pmchop.h"
#endif

#ifndef	__PLAYER_PMCORAL_H__
#include "player\pmcoral.h"
#endif

#ifndef	__PLAYER_PMDEAD_H__
#include "player\pmdead.h"
#endif

#ifndef	__PLAYER_PMFLY_H__
#include "player\pmfly.h"
#endif

#ifndef	__PLAYER_PMJELLY_H__
#include "player\pmjelly.h"
#endif

#ifndef __GFX_FONT_H__
#include "gfx\font.h"
#endif

#ifndef __GFX_SPRBANK_H__
#include "gfx\sprbank.h"
#endif

// to be removed
#include "gfx\tpage.h"


/*	Std Lib
	------- */

/*	Data
	---- */

#ifndef __SPR_SPRITES_H__
#include <sprites.h>
#endif


/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

//#define _RECORD_DEMO_MODE_
#ifdef __USER_paul__
#define _STATE_DEBUG_
#endif 


#define SLIPSPEED		10					// Speed that player slips on icy surfaces

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

// Two dice.  One says 'Re' on every face, the other says 'boot',
// 'install', 'try', 'tire', 'sume' and 'number'

/*
WEAPON MODES
	unamred				constant
	karate-chop			constant
	balloon				timed ( respawn )
	bubble mixture		(un)limited supply ( respawn )
	helmet				constant ( respawn )
	coral blower		constant ( respawn )
	net					constant
	jelly launcher		limited supply ( respawn )

POWER-UPS
	glasses				constant
	squeaky boots		timed ( respawn )
	mm & bb ring		timed
*/

/*----------------------------------------------------------------------
	Function Prototypes
	------------------- */

/*----------------------------------------------------------------------
	Vars
	---- */

#ifdef _RECORD_DEMO_MODE_

#include "player\demoplay.h"

#define MAX_DEMO_SIZE			512			// So max size of a demo is 1k
#define MAX_DEMO_TIME_IN_FRAMES	30*60		// Recorded demo will last 30 seconds

static CDemoPlayer::demoPlayerControl s_demoControls[MAX_DEMO_SIZE]={{PI_NONE,0}};
static int s_demoSize=0;
static int s_demoFrameCount=0;

static void writeDemoControls()
{
	char	filename[32];
	int		fh;
	int		fc=MAX_DEMO_TIME_IN_FRAMES;

	sprintf(filename,"demo____.dmo");
	fh=PCcreat((char *)filename,0);
	ASSERT(fh!=-1);
	PCwrite(fh,(char*)&fc,sizeof(fc));													// frame count
	PCwrite(fh,(char*)&s_demoSize,sizeof(s_demoSize));									// demo size
	for(int i=0;i<s_demoSize;i++)
		PCwrite(fh,(char*)&s_demoControls[i],sizeof(CDemoPlayer::demoPlayerControl));	// control data
	PCclose(fh);

	SYSTEM_DBGMSG("Written demo file %s with %d frames",filename,s_demoSize);
}
#endif


#ifdef _STATE_DEBUG_
static const char *s_modeText[NUM_PLAYERMODES]=
{
	"BASICUNARMED",
	"FULLUNARMED",
	"BALLOON",
	"BUBBLE MIXTURE",
	"NET",
	"CORALBLOWER",
	"JELLY LAUNCHER",
	"DEAD",
	"FLY",
};
#endif


int		s_screenPos;
int		m_cameraLookOffset;

int MAP2D_CENTRE_X=-256;
int MAP2D_CENTRE_Y=-170;
int MAP2D_BLOCKSTEPSIZE=16;

CPlayerModeBase				PLAYERMODE;
CPlayerModeChop				PLAYERMODECHOP;
CPlayerModeBalloon			PLAYERMODEBALLOON;
CPlayerModeBubbleMixture	PLAYERMODEBUBBLEMIXTURE;
CPlayerModeNet				PLAYERMODENET;
CPlayerModeCoralBlower		PLAYERMODECORALBLOWER;
CPlayerModeJellyLauncher	PLAYERMODEJELLYLAUNCHER;
CPlayerModeDead				PLAYERMODEDEAD;
CPlayerModeFly				PLAYERMODEFLY;

CPlayerMode	*CPlayer::s_playerModes[NUM_PLAYERMODES]=
{
	&PLAYERMODE,				// PLAYER_MODE_BASICUNARMED
	&PLAYERMODECHOP,			// PLAYER_MODE_FULLUNARMED
	&PLAYERMODEBALLOON,			// PLAYER_MODE_BALLOON
	&PLAYERMODEBUBBLEMIXTURE,	// PLAYER_MODE_BUBBLE_MIXTURE
	&PLAYERMODENET,				// PLAYER_MODE_NET
	&PLAYERMODECORALBLOWER,		// PLAYER_MODE_CORALBLOWER
	&PLAYERMODEJELLYLAUNCHER,	// PLAYER_MODE_JELLY_LAUNCHER
	&PLAYERMODEDEAD,			// PLAYER_MODE_DEAD
	&PLAYERMODEFLY,				// PLAYER_MODE_FLY
};


// A big bunch of 'temporary' variables for tweaking things
// This #def makes them static under a release build..
#ifdef __VERSION_DEBUG__
#define		pint		int
#else
#define		pint		static const int
#endif

pint	sbanimspeed=0;

pint	looktimeout=20;
pint	lookmaxoffsetup=3*MAP2D_BLOCKSTEPSIZE;
pint	lookmaxoffsetdown=6*MAP2D_BLOCKSTEPSIZE;
pint	lookspeed=2;
pint	lookreturnspeed=5;

pint	ledgeTimer=50;
pint	ledgeSpeedIn=1;
pint	ledgeSpeedOut=3;
pint	ledgeShift=1;

pint	cammove=2;


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CPlayer::init()
{
	CPlayerThing::init();

	m_fontBank=new ("PlayerFont") FontBank();
	m_fontBank->initialise(&standardFont);
	m_fontBank->setOt(5);

	m_spriteBank=new ("PlayerSprites") SpriteBank();
	m_spriteBank->load(SPRITES_SPRITES_SPR);

	m_layerCollision=NULL;

	m_actorGfx=CActorPool::GetActor(ACTORS_SPONGEBOB_SBK);

	for(int i=0;i<NUM_PLAYERMODES;i++)
	{
		s_playerModes[i]->initialise(this);
	}
	m_currentPlayerModeClass=NULL;
	setMode(PLAYER_MODE_FULLUNARMED);	//PKG

m_animNo=0;
m_animFrame=0;
	setFacing(FACING_RIGHT);
	respawn();

	m_lives=CGameSlotManager::getSlotData().m_lives;

	m_lastPadInput=m_padInput=PI_NONE;

	s_screenPos=128;

	resetPlayerCollisionSizeToBase();

	m_divingHelmet=false;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CPlayer::shutdown()
{
	for(int i=0;i<NUM_PLAYERMODES;i++)
	{
		s_playerModes[i]->shutdown();
	}

	delete m_actorGfx;

	m_spriteBank->dump();	delete m_spriteBank;
	m_fontBank->dump();		delete m_fontBank;

	CPlayerThing::shutdown();
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int newmode=-1;

#ifdef _STATE_DEBUG_
char posBuf[100];
#endif
void	CPlayer::think(int _frames)
{
	int	i;


if(PadGetHeld(0)&PAD_L1&&PadGetHeld(0)&PAD_L2)
{
	respawn();
}
if(newmode!=-1)
{
	setMode((PLAYER_MODE)newmode);
	newmode=-1;
}

	if(isOnPlatform())
	{
		shove(m_platform->getPosDelta());
	}
	for(i=0;i<_frames;i++)
	{
		// Think
		updatePadInput();
//		s_modes[m_currentMode].m_modeControl->think();
//		m_currentStateClass->think(this);
		m_currentPlayerModeClass->think();

		// Powerups
		if(m_squeakyBootsTimer)
		{
			m_squeakyBootsTimer--;
		}
		if(m_invincibilityRingTimer)
		{
			m_invincibilityRingTimer--;
		}

		// Flashing..
		if(m_invincibleFrameCount)
		{
			m_invincibleFrameCount--;
		}


if(Pos.vx<64)Pos.vx=64;
else if(Pos.vx>m_mapEdge.vx-64)Pos.vx=m_mapEdge.vx-64;
if(Pos.vy<64)Pos.vy=64;
else if(Pos.vy>m_mapEdge.vy-64)Pos.vy=m_mapEdge.vy-64;


		// Look around
		int	pad=getPadInputHeld();
		if(pad&PI_UP&&canDoLookAround())
		{
			if(m_padLookAroundTimer>0)
			{
				m_padLookAroundTimer=0;
			}
			else if(m_padLookAroundTimer>-looktimeout)
			{
				m_padLookAroundTimer--;
			}
			else if(m_cameraLookOffset>-lookmaxoffsetup)
			{
				m_cameraLookOffset-=lookspeed;
				if(m_cameraLookOffset<-lookmaxoffsetup)
				{
					m_cameraLookOffset=-lookmaxoffsetup;
				}
			}
		}
		else if(pad&PI_DOWN&&canDoLookAround())
		{
			if(m_padLookAroundTimer<0)
			{
				m_padLookAroundTimer=0;
			}
			else if(m_padLookAroundTimer<looktimeout)
			{
				m_padLookAroundTimer++;
			}
			else if(m_cameraLookOffset<lookmaxoffsetdown)
			{
				m_cameraLookOffset+=lookspeed;
				if(m_cameraLookOffset>lookmaxoffsetdown)
				{
					m_cameraLookOffset=lookmaxoffsetdown;
				}
			}
		}
		else
		{
			m_padLookAroundTimer=0;
		}

		// Return to centre
		if(m_padLookAroundTimer==0&&m_cameraLookOffset<0)
		{
			m_cameraLookOffset+=lookreturnspeed;
			if(m_cameraLookOffset>0)
			{
				m_cameraLookOffset=0;
			}
		}
		if(m_padLookAroundTimer==0&&m_cameraLookOffset>0)
		{
			m_cameraLookOffset-=lookreturnspeed;
			if(m_cameraLookOffset<0)
			{
				m_cameraLookOffset=0;
			}
		}

		// Automatic anim sfx
		playAnimFrameSfx(m_animNo,m_animFrame);
	}

	// Ledge look-ahead stuff
	if(m_ledgeLookAhead&&m_ledgeLookAhead==m_lastLedgeLookAhead)
	{
		if(m_ledgeLookTimer<ledgeTimer)
		{
			m_ledgeLookTimer+=_frames;
		}
		else
		{
			int limit;
			limit=(m_ledgeLookAhead*MAP2D_BLOCKSTEPSIZE)<<ledgeShift;
			if(m_ledgeLookAhead>0)
			{
				if(m_ledgeLookOffset<limit)
				{
					// Look down
					m_ledgeLookOffset+=ledgeSpeedIn*_frames;
					if(m_ledgeLookOffset>limit)
					{
						m_ledgeLookOffset=limit;
					}
				}
				else if(m_ledgeLookOffset>limit)
				{
					// Look up
					m_ledgeLookOffset-=ledgeSpeedIn*_frames;
					if(m_ledgeLookOffset<limit)
					{
						m_ledgeLookOffset=limit;
					}
				}
			}
		}
	}
	else
	{
		if(m_ledgeLookOffset>0)
		{
			// Relax from look down
			m_ledgeLookOffset-=ledgeSpeedOut*_frames;
			if(m_ledgeLookOffset<=0)
			{
				m_ledgeLookOffset=0;
				m_ledgeLookTimer=0;
			}
		}
		else if(m_ledgeLookOffset<0)
		{
			// Relax from look up
			m_ledgeLookOffset+=ledgeSpeedOut*_frames;
			if(m_ledgeLookOffset>=0)
			{
				m_ledgeLookOffset=0;
				m_ledgeLookTimer=0;
			}
		}
	}
	m_lastLedgeLookAhead=m_ledgeLookAhead;
	m_ledgeLookAhead=0;

	
	// Camera focus point stuff
	m_currentCamFocusPointTarget.vx=Pos.vx+MAP2D_CENTRE_X;
	m_currentCamFocusPointTarget.vy=Pos.vy+MAP2D_CENTRE_Y;
	for(i=0;i<_frames;i++)
	{
		m_currentCamFocusPoint.vx+=(m_currentCamFocusPointTarget.vx-m_currentCamFocusPoint.vx)>>cammove;
		m_currentCamFocusPoint.vy+=(m_currentCamFocusPointTarget.vy-m_currentCamFocusPoint.vy)>>cammove;
	}

	// Final camera position
	int yoff;
	yoff=m_cameraLookOffset+(m_ledgeLookOffset>>ledgeShift);
	if(yoff<-lookmaxoffsetup)yoff=-lookmaxoffsetup;
	else if(yoff>lookmaxoffsetdown)yoff=lookmaxoffsetdown;
	m_cameraPos.vx=m_currentCamFocusPoint.vx;
	m_cameraPos.vy=m_currentCamFocusPoint.vy+yoff;


	// Limit camera scroll to the edges of the map
	if(m_cameraPos.vx<0)
	{
		m_cameraPos.vx=0;
	}
	else if(m_cameraPos.vx>m_mapCameraEdges.vx)
	{
		m_cameraPos.vx=m_mapCameraEdges.vx;
	}
	if(m_cameraPos.vy<0)
	{
		m_cameraPos.vy=0;
	}
	else if(m_cameraPos.vy>m_mapCameraEdges.vy)
	{
		m_cameraPos.vy=m_mapCameraEdges.vy;
	}
	
	CPlayerThing::think(_frames);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int panim=-1;
#include "gfx\prim.h"	// (pkg)
int healthx=40;
int healthy=40;
int healthr=200;
int healthg=75;
int healthb=75;

#ifdef __USER_paul__
#define NUM_LASTPOS	50
static DVECTOR	lastpos[NUM_LASTPOS];
static int		lastposnum=0;
#endif

#ifdef __USER_paul__
int mouth=-1,eyes=-1;
#endif
void	CPlayer::render()
{
	CPlayerThing::render();
	
#ifdef _STATE_DEBUG_
sprintf(posBuf,"%03d (%02d) ,%03d (%02d) = dfg:%+02d",Pos.vx,Pos.vx&0x0f,Pos.vy,Pos.vy&0x0f,getHeightFromGround(Pos.vx,Pos.vy));
m_fontBank->print(40,40,posBuf);
#endif


#ifdef __USER_paul__
if(Pos.vx!=lastpos[lastposnum].vx||Pos.vy!=lastpos[lastposnum].vy)
{
	lastposnum=(lastposnum+1)%NUM_LASTPOS;
	lastpos[lastposnum]=Pos;
}
for(int i=0;i<NUM_LASTPOS;i++)
{
	int	x,y;
	x=lastpos[i].vx-m_cameraPos.vx;
	y=lastpos[i].vy-m_cameraPos.vy;
	DrawLine(x-4,y-4,x+4,y+4,0,0,255,0);
	DrawLine(x-4,y+4,x+4,y-4,0,0,255,0);
}
#endif

	// Render
	if(m_invincibleFrameCount==0||m_invincibleFrameCount&2)
	{
		DVECTOR	sbPos=
		{
			Pos.vx-m_cameraPos.vx,
			Pos.vy-m_cameraPos.vy,
		};
		renderSb(&sbPos,m_animNo,m_animFrame>>sbanimspeed);
		m_currentPlayerModeClass->render(&sbPos);
	}


#ifdef _STATE_DEBUG_
	char	buf[128];
	sprintf(buf,"MODE:  %s",s_modeText[m_currentMode]);
	m_fontBank->print(40,210,buf);
#endif


	// Health
	{
		static int	s_fullHealthFrames[]=
		{
			FRM__HEALTH_FULL_1,
			FRM__HEALTH_FULL_2,
			FRM__HEALTH_FULL_3,
			FRM__HEALTH_FULL_4,
			FRM__HEALTH_FULL_5,
		};
		static int	s_emptyHealthFrames[]=
		{
			FRM__HEALTH_EMPTY_1,
			FRM__HEALTH_EMPTY_2,
			FRM__HEALTH_EMPTY_3,
			FRM__HEALTH_EMPTY_4,
			FRM__HEALTH_EMPTY_5,
		};
		int			i,x,y;
		POLY_FT4	*ft4;
		int			*frames;

		x=healthx;
		y=healthy;
		if(m_health==0||m_healthReactFrames)
		{
			if(m_healthReactFrames)
			{
				m_healthReactFrames--;
			}
			frames=s_emptyHealthFrames;
		}
		else
		{
			frames=s_fullHealthFrames;
		}

		for(i=5;i>0;i--)
		{
			ft4=m_spriteBank->printFT4(*frames++,x,y,0,0,5);
			if(i>m_health)
			{
				setRGB0(ft4,healthr,healthg,healthb);
			}
			y+=9;
		}
	}

	// Mode specific ui
	m_currentPlayerModeClass->renderModeUi();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:	Pre-calcs the visible edges of the map ( ie: the hard limits
				for the camera pos )
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayer::setMapSize(DVECTOR _mapSize)
{
	m_mapCameraEdges.vx=(_mapSize.vx-34)*MAP2D_BLOCKSTEPSIZE;		// Made up numbers! :) (pkg)
	m_mapCameraEdges.vy=(_mapSize.vy-18)*MAP2D_BLOCKSTEPSIZE;
	m_mapEdge.vx=_mapSize.vx*MAP2D_BLOCKSTEPSIZE;
	m_mapEdge.vy=_mapSize.vy*MAP2D_BLOCKSTEPSIZE;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int CPlayer::getHeightFromGround(int _x,int _y,int _maxHeight)
{
	int	height;
	DVECTOR	platformPos;
	DVECTOR newPos;
	if(isOnPlatform())
	{
		CThing *platform = isOnPlatform();
		height = platform->getNewYPos( this ) - Pos.vy;

		int groundHeight = m_layerCollision->getHeightFromGround(_x,_y,_maxHeight);

		if ( groundHeight < height )
		{
			height = groundHeight;
			clearPlatform();
		}
	}
	else
	{
		height=m_layerCollision->getHeightFromGround(_x,_y,_maxHeight);
	}
	return height;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int CPlayer::getHeightFromGroundNoPlatform(int _x,int _y,int _maxHeight=32)
{
	return( m_layerCollision->getHeightFromGround(_x,_y,_maxHeight) );
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayer::addHealth(int _health)
{
	m_health+=_health;
	if(m_health>MAX_HEALTH)
	{
		m_health=MAX_HEALTH;
	}
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayer::addLife()
{
	m_lives++;
	if(m_lives>MAX_LIVES)
	{
		m_lives=MAX_LIVES;
	}
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
ATTACK_STATE	CPlayer::getAttackState()
{
	return m_currentPlayerModeClass->getAttackState();
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayer::setMode(PLAYER_MODE _mode)
{
	resetPlayerCollisionSizeToBase();
	m_currentMode=_mode;
	m_currentPlayerModeClass=s_playerModes[_mode];
	m_currentPlayerModeClass->enter();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int CPlayer::getFacing()
{
	  return m_facing;
}
void CPlayer::setFacing(int _facing)
{
	m_facing=_facing;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int CPlayer::getAnimFrame()
{
	return m_animFrame;
}
void CPlayer::setAnimFrame(int _animFrame)
{
	m_animFrame=_animFrame;
}
int CPlayer::getAnimFrameCount()
{
	return m_actorGfx->getFrameCount(m_animNo)<<sbanimspeed;
}
int CPlayer::getAnimNo()
{
	return m_animNo;
}
void CPlayer::setAnimNo(int _animNo)
{
	m_animNo=_animNo;
	setAnimFrame(0);
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayer::playAnimFrameSfx(int _animNo,int _animFrame)
{
	static int				lastAnimNo=-1;
	static const AnimSfx	*sfx;

	if(_animNo!=lastAnimNo)
	{
		// Lookup the new anim number and cache it for next time :)
		int	i;
		sfx=s_animSfx;
		for(i=0;i<s_numAnimSfx;i++)
		{
			if(sfx->m_animNumber==_animNo)
			{
				break;
			}
			sfx++;
		}
		if(i==s_numAnimSfx)
		{
			// No sfx for this anim
			sfx=NULL;
		}
		lastAnimNo=_animNo;
	}

	// Are there any sounds for this anim at this frame?
	if(sfx)
	{
		const AnimFrameSfx	*frameSfx;
		int					i;

		ASSERT(sfx->m_numAnimFrameSfx);

		frameSfx=sfx->m_animFrameSfx;
		for(i=0;i<sfx->m_numAnimFrameSfx;i++)
		{
			if(m_animFrame==frameSfx->m_frame)
			{
				CSoundMediator::SFXID sfxId=frameSfx->m_sfxId;
				if(m_squeakyBootsTimer)
				{
					// Ugh.. horrible way to change the sfx when wearing squeaky boots (pkg)
					if(sfxId==CSoundMediator::SFX_SPONGEBOB_WALK_1)sfxId=CSoundMediator::SFX_SPONGEBOB_SQUEAKY_SHOES_1;
					else if(sfxId==CSoundMediator::SFX_SPONGEBOB_WALK_2)sfxId=CSoundMediator::SFX_SPONGEBOB_SQUEAKY_SHOES_2;
				}
				CSoundMediator::playSfx(sfxId);
				break;
			}
			if(m_animFrame<frameSfx->m_frame)
			{
				break;
			}
			frameSfx++;
		}
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayer::respawn()
{
	// Strip any items that the player might be holding
	if(m_currentMode!=PLAYER_MODE_BASICUNARMED)
	{
		setMode(PLAYER_MODE_FULLUNARMED);
	}
	else
	{
		setMode(PLAYER_MODE_BASICUNARMED);
	}

	m_health=MAX_HEALTH;
	m_healthReactFrames=0;
	m_invincibleFrameCount=INVINCIBLE_FRAMES__START;
	Pos=m_respawnPos;
	m_cameraLookOffset=0;
	m_currentCamFocusPoint.vx=Pos.vx+MAP2D_CENTRE_X;
	m_currentCamFocusPoint.vy=Pos.vy+MAP2D_CENTRE_Y;
	m_padLookAroundTimer=0;
	m_ledgeLookAhead=m_lastLedgeLookAhead=0;
	m_ledgeLookOffset=0;
	m_ledgeLookTimer=0;

	m_glassesFlag=0;
	m_squeakyBootsTimer=0;
	m_invincibilityRingTimer=0;
	m_bubbleAmmo=0;
	m_jellyAmmo=0;

	
	clearPlatform();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayer::renderSb(DVECTOR *_pos,int _animNo,int _animFrame)
{
	m_actorGfx->Render(*_pos,_animNo,_animFrame,m_facing==FACING_RIGHT?0:1);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:	Says whether SB can do the look up/down thing
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int	CPlayer::canDoLookAround()
{
	return m_currentPlayerModeClass->canDoLookAround();
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int invincibleSponge=false;		// NB: This is for debugging purposes only so don't try and use it for a permenant cheat mode..
void CPlayer::takeDamage(DAMAGE_TYPE _damage)
{
	if(m_invincibleFrameCount==0&&			// Don't take damage if still recovering from the last hit
	   m_invincibilityRingTimer==0&&		// Or if we have the invincibility ring on
	   m_currentMode!=PLAYER_MODE_DEAD)		// Or already dead! :)
	{
		int	ouchThatHurt=true;

		// Check if we are currently immune to this damage type
		switch(_damage)
		{
			case DAMAGE__NONE:
				break;

			case DAMAGE__ELECTROCUTION:
			case DAMAGE__SHOCK_ENEMY:
				if(m_squeakyBootsTimer)
				{
					ouchThatHurt=false;
				}
				break;

			case DAMAGE__FALL:
			case DAMAGE__LAVA:
			case DAMAGE__HIT_ENEMY:
			case DAMAGE__GAS_ENEMY:
			case DAMAGE__POISON_ENEMY:
			case DAMAGE__SWALLOW_ENEMY:
			case DAMAGE__PINCH_ENEMY:
			case DAMAGE__SQUASH_ENEMY:
			case DAMAGE__BURN_ENEMY:
			case DAMAGE__BITE_ENEMY:
				break;
		}

		if(ouchThatHurt)
		{
			if(invincibleSponge){m_invincibleFrameCount=INVINCIBLE_FRAMES__HIT;return;}
			if(m_health)
			{
				m_invincibleFrameCount=INVINCIBLE_FRAMES__HIT;
				m_healthReactFrames=10;
				m_health--;
			}
			else
			{
				CSoundMediator::playSfx(CSoundMediator::SFX_SPONGEBOB_DEFEATED_JINGLE);
				setMode(PLAYER_MODE_DEAD);
			}
		}
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayer::updatePadInput()
{
	m_lastPadInput=m_padInput;
	m_padInput=readPadInput();
	m_padInputDown=(PLAYERINPUT)(m_padInput&(m_lastPadInput^-1));
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
PLAYERINPUT CPlayer::readPadInput()
{
	PLAYERINPUT	input;
	int			pad;

	input=PI_NONE;
	pad=PadGetHeld(0);
	
	if(pad&CPadConfig::getButton(CPadConfig::PAD_CFG_UP))
	{
		input=(PLAYERINPUT)(input|PI_UP);
	}
	if(pad&CPadConfig::getButton(CPadConfig::PAD_CFG_DOWN))
	{
		input=(PLAYERINPUT)(input|PI_DOWN);
	}
	if(pad&CPadConfig::getButton(CPadConfig::PAD_CFG_LEFT))
	{
		input=(PLAYERINPUT)(input|PI_LEFT);
	}
	if(pad&CPadConfig::getButton(CPadConfig::PAD_CFG_RIGHT))
	{
		input=(PLAYERINPUT)(input|PI_RIGHT);
	}
	if(pad&CPadConfig::getButton(CPadConfig::PAD_CFG_JUMP))
	{
		input=(PLAYERINPUT)(input|PI_JUMP);
	}
	if(pad&CPadConfig::getButton(CPadConfig::PAD_CFG_ACTION))
	{
		input=(PLAYERINPUT)(input|PI_ACTION);
	}


#ifdef _RECORD_DEMO_MODE_
	CDemoPlayer::demoPlayerControl	*crnt;
	PLAYERINPUT						lastInput;
	crnt=&s_demoControls[s_demoSize];
	if(s_demoFrameCount==0)
	{
		crnt->m_inputValue=input;
	}
	lastInput=(PLAYERINPUT)crnt->m_inputValue;
	if(crnt->m_length==255)
	{
		lastInput=(PLAYERINPUT)(input-1);
	}
	if(lastInput==input)
	{
		crnt->m_length++;
	}
	else
	{
		s_demoSize++;
		ASSERT(s_demoSize<MAX_DEMO_SIZE);
		crnt++;
		crnt->m_inputValue=input;
		crnt->m_length=1;
	}
	s_demoFrameCount++;
	if(s_demoFrameCount==MAX_DEMO_TIME_IN_FRAMES)
	{
		writeDemoControls();
		ASSERT(!"DEMO ENDED");
	}
#endif

	return input;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayer::shove( DVECTOR move )
{
	int		colHeight;

	// X movement
	colHeight=m_layerCollision->getHeightFromGround(Pos.vx+move.vx,Pos.vy,5);
	if(colHeight<0)
	{
		// Stop at the edge of the obstruction
		int	dir,vx,cx,i;
		if(move.vx<0)
		{
			dir=-1;
			vx=move.vx;
		}
		else
		{
			dir=+1;
			vx=move.vx;
		}
		cx=Pos.vx;
		for(i=0;i<vx;i++)
		{
			if(m_layerCollision->getHeightFromGround(cx,Pos.vy)<0)
			{
				break;
			}
			cx+=dir;
		}
		if(i)
			Pos.vx=cx-dir;
	}
	else
	{
		// No obstruction
		Pos.vx+=move.vx;
	}

	// Y movement
	colHeight=m_layerCollision->getHeightFromGround(Pos.vx,Pos.vy+move.vy,5);
	if(colHeight<0)
	{
		// Stop at the edge of the obstruction
		int	dir,vy,cy,i;
		if(move.vy<0)
		{
			dir=-1;
			vy=move.vy;
		}
		else
		{
			dir=+1;
			vy=move.vy;
		}
		cy=Pos.vy;
		for(i=0;i<vy;i++)
		{
			if(m_layerCollision->getHeightFromGround(Pos.vx,cy)<0)
			{
				break;
			}
			cy+=dir;
		}
		if(i)
			Pos.vy=cy-dir;
	}
	else
	{
		// No obstruction
		Pos.vy+=move.vy;
	}
}

void	CPlayer::setPlatform(CThing *_newPlatform)
{
	m_platform=_newPlatform;
}
void	CPlayer::clearPlatform()
{
	m_platform=NULL;
}

void	CPlayer::setHasPlatformCollided( bool newVal )
{
	m_hasPlatformCollided = newVal;
}

bool	CPlayer::getHasPlatformCollided()
{
	return( m_hasPlatformCollided );
}




/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int		CPlayer::moveVertical(int _moveDistance)
{
	DVECTOR			pos;
	int				hitGround;
//	int				colHeight;

	pos=Pos;
	hitGround=false;

	// Are we falling?
	if(_moveDistance>0)
	{
		int colHeightBefore,colHeightAfter;

		// Yes.. Check to see if we're about to hit/go through the ground
		colHeightBefore=getHeightFromGround(pos.vx,pos.vy,16);
		colHeightAfter=getHeightFromGround(pos.vx,pos.vy+_moveDistance,16);
		if(colHeightBefore>=0&&colHeightAfter<=0)
		{
			// Stick at ground level
			pos.vy+=colHeightAfter+_moveDistance;
			_moveDistance=0;
			hitGround=true;
		}
	}
/*
	}
	else// if(getHeightFromGround(pos.vx,pos.vy+_moveDistance,1))
	{
		// Must be below ground
		// Are we jumping into an impassable block?
		if(_moveDistance>0&&
		   (m_layerCollision->getCollisionBlock(pos.vx,pos.vy+_moveDistance)&COLLISION_TYPE_MASK)!=COLLISION_TYPE_FLAG_NORMAL)
		{
			pos.vy=(pos.vy&0xfff0);
			_moveDistance=0;
			hitGround=true;
		}
		else if(isOnPlatform()&&_moveDistance>=0)
		{
			pos.vy+=colHeight;
			hitGround=true;
		}
	}
*/
	pos.vy+=_moveDistance;
	setPlayerPos(&pos);

	return hitGround;


	/*
	DVECTOR			pos;
	int				hitGround;
	int				colHeight;

	pos=Pos;
	hitGround=false;
	colHeight=getHeightFromGround(pos.vx,pos.vy,1);
	if(colHeight>=0)
	{
		// Above or on the ground
		// Are we falling?
		if(_moveDistance>0)
		{
			// Yes.. Check to see if we're about to hit/go through the ground
			colHeight=getHeightFromGround(pos.vx,pos.vy+_moveDistance,16);

			if(colHeight<=0)
			{
				// Stick at ground level
				pos.vy+=colHeight+_moveDistance;
				_moveDistance=0;
				hitGround=true;
			}
		}
	}
	else// if(getHeightFromGround(pos.vx,pos.vy+_moveDistance,1))
	{
		// Must be below ground
		// Are we jumping into an impassable block?
		if(_moveDistance>0&&
		   (m_layerCollision->getCollisionBlock(pos.vx,pos.vy+_moveDistance)&COLLISION_TYPE_MASK)!=COLLISION_TYPE_FLAG_NORMAL)
		{
			pos.vy=(pos.vy&0xfff0);
			_moveDistance=0;
			hitGround=true;
		}
		else if(isOnPlatform()&&_moveDistance>=0)
		{
			pos.vy+=colHeight;
			hitGround=true;
		}
	}

	pos.vy+=_moveDistance;
	setPlayerPos(&pos);

	return hitGround;
	*/
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int		CPlayer::moveHorizontal(int _moveDistance)
{
	int	hitWall;

	hitWall=false;
	if(_moveDistance)
	{
		CLayerCollision	*collision;
		DVECTOR			pos;
		int				colHeight;

		collision=getLayerCollision();
		pos=getPlayerPos();
		colHeight=getHeightFromGround(pos.vx,pos.vy,5);
		if(colHeight==0)
		{
			// Ok.. we're on the ground. What happens if we move left/right
			colHeight=getHeightFromGround(pos.vx+_moveDistance,pos.vy);
			if(colHeight<-8)
			{
				// Big step up. Stop at the edge of the obstruction
				int	dir,vx,cx,i;
				if(_moveDistance<0)
				{
					dir=-1;
					vx=-_moveDistance;
				}
				else
				{
					dir=+1;
					vx=_moveDistance;
				}
				cx=pos.vx;
				for(i=0;i<vx;i++)
				{
					if(getHeightFromGround(cx,pos.vy)<-8)
					{
						break;
					}
					cx+=dir;
				}
				if(i)
					pos.vx=cx-dir;

				hitWall=true;
				_moveDistance=0;
				
				// Get the height at this new position and then try the step-up code below.
				// Without this, there are problems when you run up a slope and hit a wall at the same time
				colHeight=getHeightFromGround(pos.vx,pos.vy);
			}
			if(colHeight&&colHeight>=-8&&colHeight<=8)
			{
				// Small step up/down. Follow the contour of the level
				pos.vy+=colHeight;
			}
		}
		else
		{
			// In the air
			/*
			if((getLayerCollision()->getCollisionBlock(pos.vx+_moveDistance,pos.vy)&COLLISION_TYPE_MASK)==(6<<COLLISION_TYPE_FLAG_SHIFT))
			{
				// Hit an impassable block
				pos.vx&=0xfff0;
				if(_moveDistance>0)
				{
					pos.vx+=15;
				}
				_moveDistance=0;
			}
			else */if(colHeight>=0) // Lets you jump through platforms from below
			{
				colHeight=getHeightFromGround(pos.vx+_moveDistance,pos.vy,5);
				if(colHeight<0)
				{
					// Stop at the edge of the obstruction
					int	dir,vx,cx,i;
					if(_moveDistance<0)
					{
						dir=-1;
						vx=_moveDistance;
					}
					else
					{
						dir=+1;
						vx=_moveDistance;
					}
					cx=pos.vx;
					for(i=0;i<vx;i++)
					{
						if(getHeightFromGround(cx,pos.vy)<0)
						{
							break;
						}
						cx+=dir;
					}
					if(i)
						pos.vx=cx-dir;
					_moveDistance=0;
				}
			}
		}
		pos.vx+=_moveDistance;
		setPlayerPos(&pos);
	}
	
	return hitWall;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayer::resetPlayerCollisionSizeToBase()
{
	setPlayerCollisionSize(0,-COLSIZE_BASE_HEIGHT/2,COLSIZE_BASE_WIDTH,COLSIZE_BASE_HEIGHT);
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayer::setPlayerCollisionSize(int _x,int _y,int _w,int _h)
{
	setCollisionSize(_w,_h);
	setCollisionCentreOffset(_x,_y);
}

/*===========================================================================
end */
