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

#ifndef __SPR_INGAMEFX_H__
#include <ingamefx.h>
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


int s_screenPos;
DVECTOR	m_cameraScrollPos={0,600};

int SCREEN_GEOM_CENTRE_X=248;
int SCREEN_GEOM_CENTRE_Y=165;
int SCREEN_GEOM_PLAYER_OFS_X=9;
int SCREEN_GEOM_PLAYER_OFS_Y=-26;

int MAP2D_CENTRE_X=-256;
int MAP2D_CENTRE_Y=-136;
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

int	sbanimspeed=1;


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
	m_spriteBank->load(INGAMEFX_INGAMEFX_SPR);

	m_layerCollision=NULL;

//	m_onPlatform = false;
//	m_prevOnPlatform = false;
	
	m_actorGfx=CActorPool::GetActor(ACTORS_SPONGEBOB_SBK);

	for(int i=0;i<NUM_PLAYERMODES;i++)
	{
		s_playerModes[i]->initialise(this);
	}
	m_currentPlayerModeClass=NULL;
	setMode(PLAYER_MODE_BASICUNARMED);	//PKG

m_animNo=0;
m_animFrame=0;
	setFacing(FACING_RIGHT);
	respawn();

	m_lives=CGameSlotManager::getSlotData().m_lives;

	m_cameraOffset.vx=0;
	m_cameraOffset.vy=0;

	m_lastPadInput=m_padInput=PI_NONE;

	s_screenPos=128;

	setCollisionSize(25,50);
	setCollisionCentreOffset(0,-25);

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
		if(m_invinvibilityRingTimer)
		{
			m_invinvibilityRingTimer--;
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
	}


	
	// Move the camera offset
	m_playerScreenGeomPos.vx=SCREEN_GEOM_PLAYER_OFS_X+((MAP2D_BLOCKSTEPSIZE*m_cameraScrollPos.vx)>>8);
	m_playerScreenGeomPos.vy=SCREEN_GEOM_PLAYER_OFS_Y+((MAP2D_BLOCKSTEPSIZE*m_cameraScrollPos.vy)>>8);
	m_cameraOffset.vx=MAP2D_CENTRE_X+((MAP2D_BLOCKSTEPSIZE*(-m_cameraScrollPos.vx))>>8);
	m_cameraOffset.vy=MAP2D_CENTRE_Y+((MAP2D_BLOCKSTEPSIZE*(-m_cameraScrollPos.vy))>>8);


	m_cameraPos.vx=Pos.vx+m_cameraOffset.vx;
	m_cameraPos.vy=Pos.vy+m_cameraOffset.vy;


	// Limit camera scroll to the edges of the map
	if(m_cameraPos.vx<0)
	{
		m_playerScreenGeomPos.vx+=m_cameraPos.vx;
		m_cameraPos.vx=0;
	}
	else if(m_cameraPos.vx>m_mapCameraEdges.vx)
	{
		m_playerScreenGeomPos.vx-=m_mapCameraEdges.vx-m_cameraPos.vx;
		m_cameraPos.vx=m_mapCameraEdges.vx;
	}
	if(m_cameraPos.vy<0)
	{
		m_playerScreenGeomPos.vy+=m_cameraPos.vy;
		m_cameraPos.vy=0;
	}
	else if(m_cameraPos.vy>m_mapCameraEdges.vy)
	{
		m_playerScreenGeomPos.vy-=m_mapCameraEdges.vy-m_cameraPos.vy;
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
int healthx=400;
int healthy=30;
int healthr=200;
int healthg=75;
int healthb=75;

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

	// Render
	if(m_invincibleFrameCount==0||m_invincibleFrameCount&2)
	{
		DVECTOR	sbPos=
		{
			SCREEN_GEOM_CENTRE_X+m_playerScreenGeomPos.vx,
			SCREEN_GEOM_CENTRE_Y+m_playerScreenGeomPos.vy
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
void CPlayer::setMode(PLAYER_MODE _mode)
{
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
	const AnimSfx	*sfx;

	m_animFrame=_animFrame;

	// Are there any sfx for this frame?
	sfx=&s_animSfx[m_animNo];
	if(sfx->m_numAnimFrameSfx)
	{
		const AnimFrameSfx	*frameSfx;
		int					i;

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

	m_glassesFlag=0;
	m_squeakyBootsTimer=0;
	m_invinvibilityRingTimer=0;
	m_bubbleAmmo=0;

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
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
#ifdef __VERSION_DEBUG__
int invincibleSponge=false;		// NB: This is for debugging purposes only so don't try and use it for a permenant cheat mode..
#endif
void CPlayer::takeDamage(DAMAGE_TYPE _damage)
{
	if(m_invincibleFrameCount==0&&	// Don't take damage if still recovering from the last hit
	   m_invinvibilityRingTimer==0)	// Or if we have the invincibility ring on
	{
		int	ouchThatHurt=true;

		// Check if we are currently immune to this damage type
		switch(_damage)
		{
			case DAMAGE__NONE:
				break;

			case DAMAGE__FALL:
			case DAMAGE__LAVA:
			case DAMAGE__HIT_ENEMY:
			case DAMAGE__SHOCK_ENEMY:
			case DAMAGE__GAS_ENEMY:
			case DAMAGE__POISON_ENEMY:
			case DAMAGE__SWALLOW_ENEMY:
			case DAMAGE__PINCH_ENEMY:
			case DAMAGE__SQUASH_ENEMY:
			case DAMAGE__BURN_ENEMY:
			case DAMAGE__BITE_ENEMY:
				break;

			case DAMAGE__ELECTROCUTION:
				if(m_squeakyBootsTimer)
				{
					ouchThatHurt=false;
				}
				break;
		}

		if(ouchThatHurt)
		{
#ifdef __VERSION_DEBUG__
			if(invincibleSponge){m_invincibleFrameCount=INVINCIBLE_FRAMES__HIT;return;}
#endif
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


/*===========================================================================
end */
