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

#ifndef __ANIM_HEADER__
#include "gfx\anim.h"
#endif

#ifndef __PAD_PADS_H__
#include "pad\pads.h"
#endif

#ifndef	__GAME_GAMESLOT_H__
#include "game\gameslot.h"
#endif

#ifndef __GAME_GAME_H__
#include "game\game.h"
#endif

#ifndef __LAYER_COLLISION_H__
#include "level\collision.h"
#endif

// to be removed
#include "gfx\tpage.h"


/*	Std Lib
	------- */

/*	Data
	---- */

#ifndef	__ANIM_SPONGEBOB_HEADER__
#include <ACTOR_SPONGEBOB_ANIM.h>
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
static const char *s_stateText[NUM_STATES]=
{
	"IDLE",
	"IDLETEETER",
	"JUMP",
	"RUN",
	"FALL",
	"FALLFAR",
	"BUTTBOUNCE",
	"BUTTFALL",
	"BUTTLAND",
	"ATTACK",
	"RUNATTACK",
	"AIRATTACK",
	"DUCK",
	"SOAKUP",
	"GETUP",
	"DEAD",
};
static const char *s_modeText[NUM_PLAYERMODES]=
{
	"BASICUNARMED",
	"FULLUNARMED",
	"BALLOON",
	"NET",
	"CORALBLOWER",
};
#include "gfx\font.h"	
FontBank	s_debugFont;
#endif


int s_health;
int s_screenPos;
DVECTOR	m_cameraScrollPos={0,600};

int SCREEN_GEOM_CENTRE_X=248;
int SCREEN_GEOM_CENTRE_Y=129;
int SCREEN_GEOM_PLAYER_OFS_X=9;
int SCREEN_GEOM_PLAYER_OFS_Y=-26;

int MAP2D_CENTRE_X=-256;
int MAP2D_CENTRE_Y=-136;
int MAP2D_BLOCKSTEPSIZE=16;

int CAMERA_SCROLLLIMIT=8;				// SB is this many tiles off centre at most
int CAMERA_SCROLLTHRESHOLD=6;			// If SB moves when more than this many tiles off-centre, the camera will *always* scroll
int CAMERA_STARTMOVETHRESHOLD=20;		// If SB moves faster than this then the camera starts scrolling
int CAMERA_STOPMOVETHRESHOLD=10;		// If SB moves slower than this then the camera stops scrolling
int CAMERA_SCROLLSPEED=60;				// Speed of the scroll ( 60=1 tile scrolled every 4 and a bit frames )





/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CPlayer::init()
{
	CPlayerThing::init();

	m_layerCollision=NULL;
	
	sActorHdr	*Hdr=m_skel.Load(ACTORS_SPONGEBOB_A3D);		
	m_skel.Init(Hdr);
	TPLoadTex(ACTORS_ACTOR_SPONGEBOB_TEX);
	m_skel.setAnimDatabase(CAnimDB::Load(ACTORS_SPONGEBOB_ABK));


// Temporary default respawn pos... should realy be set with setRespawnPos() from the level (pkg)
m_respawnPos.vx=23*16;
m_respawnPos.vy=10*16;


m_animNo=0;
m_animFrame=0;
	m_currentMode=PLAYER_MODE_BASICUNARMED;
	setFacing(FACING_RIGHT);
	respawn();

	m_lives=CGameSlotManager::getSlotData().m_lives;

	m_cameraOffset.vx=0;
	m_cameraOffset.vy=0;
	m_cameraScrollDir=0;

	m_lastPadInput=m_padInput=PI_NONE;

	s_screenPos=128;

	m_skel.setAng(512);
//m_skel.setAngInc(678);

#ifdef _STATE_DEBUG_
	s_debugFont.initialise(&standardFont);
	s_debugFont.setJustification(FontBank::JUST_LEFT);
#endif

	setCollisionSize(25,50);
	setCollisionCentreOffset(0,-25);

	m_glassesFlag=0;
	m_squeakyBootsTimer=0;
	m_invinvibilityRingTimer=0;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CPlayer::shutdown()
{
#ifdef _STATE_DEBUG_
	s_debugFont.dump();
#endif
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
	
	CPlayerThing::think(_frames);


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
		m_currentStateClass->think(this);
		thinkVerticalMovement();
		thinkHorizontalMovement();

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


#ifdef _STATE_DEBUG_
sprintf(posBuf,"%03d (%02d) ,%03d (%02d) = dfg:%+02d",Pos.vx,Pos.vx&0x0f,Pos.vy,Pos.vy&0x0f,m_layerCollision->getHeightFromGround(Pos.vx,Pos.vy));
#endif


if(Pos.vx<64)Pos.vx=64;
else if(Pos.vx>m_mapEdge.vx-64)Pos.vx=m_mapEdge.vx-64;
if(Pos.vy<64)Pos.vy=64;
else if(Pos.vy>m_mapEdge.vy-64)Pos.vy=m_mapEdge.vy-64;

		// Teeter if on an edge
		if(m_currentState==STATE_IDLE&&isOnEdge())
		{
			setState(STATE_IDLETEETER);
		}

		// Look around
		int	pad=getPadInputHeld();
if(PadGetDown(0)&PAD_CIRCLE)
{
	m_skel.blink();
}


		// Camera scroll..
		if(m_cameraScrollDir==-1)
		{
			if(m_cameraScrollPos.vx>-CAMERA_SCROLLLIMIT<<8)
			{
				m_cameraScrollPos.vx-=CAMERA_SCROLLSPEED;
				if(m_cameraScrollPos.vx<-CAMERA_SCROLLLIMIT<<8)
				{
					m_cameraScrollPos.vx=-CAMERA_SCROLLLIMIT<<8;
					m_cameraScrollDir=0;
				}
			}
		}
		else if(m_cameraScrollDir==+1)
		{
			if(m_cameraScrollPos.vx<(CAMERA_SCROLLLIMIT<<8))
			{
				m_cameraScrollPos.vx+=CAMERA_SCROLLSPEED;
				if(m_cameraScrollPos.vx>CAMERA_SCROLLLIMIT<<8)
				{
					m_cameraScrollPos.vx=CAMERA_SCROLLLIMIT<<8;
					m_cameraScrollDir=0;
				}
			}
		}

		s_modes[m_currentMode].m_modeControl->think(this);
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
		m_cameraScrollDir=0;
	}
	else if(m_cameraPos.vx>m_mapCameraEdges.vx)
	{
		m_playerScreenGeomPos.vx-=m_mapCameraEdges.vx-m_cameraPos.vx;
		m_cameraPos.vx=m_mapCameraEdges.vx;
		m_cameraScrollDir=0;
	}
	if(m_cameraPos.vy<0)
	{
		m_playerScreenGeomPos.vy+=m_cameraPos.vy;
		m_cameraPos.vy=0;
		m_cameraScrollDir=0;
	}
	else if(m_cameraPos.vy>m_mapCameraEdges.vy)
	{
		m_playerScreenGeomPos.vy-=m_mapCameraEdges.vy-m_cameraPos.vy;
		m_cameraPos.vy=m_mapCameraEdges.vy;
		m_cameraScrollDir=0;
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayer::thinkVerticalMovement()
{
	int	colHeight;
	colHeight=m_layerCollision->getHeightFromGround(Pos.vx,Pos.vy,1);
	if(colHeight>=0)
	{
		// Above or on the ground
		// Are we falling?
		if(m_moveVel.vy>0)
		{
			// Yes.. Check to see if we're about to hit/go through the ground
			colHeight=m_layerCollision->getHeightFromGround(Pos.vx,Pos.vy+(m_moveVel.vy>>VELOCITY_SHIFT),PLAYER_TERMINAL_VELOCITY+1);
			if(colHeight<=0)
			{
				// Just hit the ground
				// Stick at ground level
				Pos.vy+=(m_moveVel.vy>>VELOCITY_SHIFT)+colHeight;
				m_moveVel.vy=0;
				m_fallFrames=0;
				if(m_currentMode!=PLAYER_MODE_BALLOON)
				{
					if(m_currentState==STATE_BUTTFALL)
					{
						// Landed from a butt bounce
						setState(STATE_BUTTLAND);
					}
					else if(m_currentState==STATE_FALLFAR)
					{
						// Landed from a painfully long fall
						setState(STATE_IDLE);
						takeDamage(DAMAGE__FALL);
						m_moveVel.vx=0;
						CSoundMediator::playSfx(CSoundMediator::SFX_SPONGEBOB_LAND_AFTER_FALL);
					}
					else if(m_moveVel.vx)
					{
						// Landed from a jump with x movement
						setState(STATE_RUN);
					}
					else
					{
						// Landed from a jump with no x movement
						setState(STATE_IDLE);
						setAnimNo(ANIM_SPONGEBOB_JUMPEND);
					}
				}
			}
		}
		else if(colHeight)
		{
			if(m_currentState!=STATE_FALL&&m_currentState!=STATE_FALLFAR&&
			   m_currentState!=STATE_BUTTFALL&&m_currentState!=STATE_BUTTBOUNCE&&
			   m_currentState!=STATE_JUMP)
			{
				// Was floating in the air.. fall!
				setState(STATE_FALL);
			}
		}
	}
	else
	{
/*
		// Below ground
		// Perhaps we should be falling?
		if(m_currentState!=STATE_FALL&&m_currentState!=STATE_FALLFAR&&
		   m_currentState!=STATE_BUTTFALL&&m_currentState!=STATE_BUTTBOUNCE&&
		   m_currentState!=STATE_JUMP)
		{
			setState(STATE_FALL);
		}
*/
	}

	Pos.vy+=m_moveVel.vy>>VELOCITY_SHIFT;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayer::thinkHorizontalMovement()
{
	if(m_moveVel.vx)
	{
		int colHeight;
		colHeight=m_layerCollision->getHeightFromGround(Pos.vx,Pos.vy,5);
		if(colHeight==0)
		{
			// Ok.. we're on the ground. What happens if we move left/right
			colHeight=m_layerCollision->getHeightFromGround(Pos.vx+(m_moveVel.vx>>VELOCITY_SHIFT),Pos.vy);
			if(colHeight<-8)
			{
				// Big step up. Stop at the edge of the obstruction
				int	dir,vx,cx,i;
				if(m_moveVel.vx<0)
				{
					dir=-1;
					vx=-m_moveVel.vx>>VELOCITY_SHIFT;
				}
				else
				{
					dir=+1;
					vx=m_moveVel.vx>>VELOCITY_SHIFT;
				}
				cx=Pos.vx;
				for(i=0;i<vx;i++)
				{
					if(m_layerCollision->getHeightFromGround(cx,Pos.vy)<-8)
					{
						break;
					}
					cx+=dir;
				}
				Pos.vx=cx-dir;

				// If running then go to idle, otherwise leave in same state
				if(m_currentState==STATE_RUN)
				{
					setState(STATE_IDLE);
				}
				m_moveVel.vx=0;
				
				// Get the height at this new position and then try the step-up code below.
				// Without this, there are problems when you run up a slope and hit a wall at the same time
				colHeight=m_layerCollision->getHeightFromGround(Pos.vx,Pos.vy);
			}
			if(colHeight&&colHeight>=-8&&colHeight<=8)
			{
				// Small step up/down. Follow the contour of the level
				Pos.vy+=colHeight;
			}
		}
		else
		{
			// In the air
//			if(!(colHeight<0&&m_currentState==STATE_JUMP)) // Lets you jump through platforms from below
			if(colHeight>=0) // Lets you jump through platforms from below
			{
				colHeight=m_layerCollision->getHeightFromGround(Pos.vx+(m_moveVel.vx>>VELOCITY_SHIFT),Pos.vy,5);
				if(colHeight<0)
				{
					// Stop at the edge of the obstruction
					int	dir,vx,cx,i;
					if(m_moveVel.vx<0)
					{
						dir=-1;
						vx=-m_moveVel.vx>>VELOCITY_SHIFT;
					}
					else
					{
						dir=+1;
						vx=m_moveVel.vx>>VELOCITY_SHIFT;
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
					Pos.vx=cx-dir;
					m_moveVel.vx=0;
				}
			}
		}
		Pos.vx+=m_moveVel.vx>>VELOCITY_SHIFT;
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int panim=-1;
#include "gfx\prim.h"	// (pkg)
int healthx=100;
int healthy=27;
int healthw=10;
int healthh=10;
int healthg=2;
int livesx=162;
int livesy=28;


#ifdef __USER_paul__
int mouth=-1,eyes=-1;
#endif
void	CPlayer::render()
{
	CPlayerThing::render();
	
#ifdef _STATE_DEBUG_
s_debugFont.print(40,40,posBuf);
#endif

	// Render
	if(m_invincibleFrameCount==0||m_invincibleFrameCount&2)
	{
#ifdef __USER_paul__
if(mouth!=-1)
{
	m_skel.setMouthTex(mouth);
	mouth=-1;
}
if(eyes!=-1)
{
	m_skel.setEyeTex(eyes);
	eyes=-1;
}
#endif

//int xval=(255-(MAP2D_BLOCKSTEPSIZE*(-m_cameraScrollPos.vx>>8)));
//DrawLine(xval-7,0,xval-7,255,0,128,255,0);
//DrawLine(xval+7,0,xval+7,255,0,128,255,0);

		SetGeomOffset(SCREEN_GEOM_CENTRE_X+m_playerScreenGeomPos.vx,SCREEN_GEOM_CENTRE_Y+m_playerScreenGeomPos.vy);
		if(panim!=-1)
			m_skel.setAnimNo(panim);
		else
			m_skel.setAnimNo(m_animNo);
		m_skel.setFrame(m_animFrame);
		m_skel.Animate(this);
		m_skel.Render(this);
		SetGeomOffset(SCREEN_GEOM_CENTRE_X,SCREEN_GEOM_CENTRE_Y);
	}


#ifdef _STATE_DEBUG_
	char	buf[128];
	sprintf(buf,"STATE: %s",s_stateText[m_currentState]);
	s_debugFont.print(40,200,buf);
	sprintf(buf,"MODE:  %s",s_modeText[m_currentMode]);
	s_debugFont.print(40,210,buf);
#endif


	// Temporary health/lives thing
#ifdef __USER_paul__
	int i,x;
	x=healthx;
	for(i=0;i<5;i++)
	{
		POLY_F4	*f4;
		f4=GetPrimF4();
		setXYWH(f4,x,healthy,healthw,healthh);
		if(i<s_health)
		{
			setRGB0(f4,0,255,0);
		}
		else
		{
			setRGB0(f4,255,0,0);
		}
		setSemiTrans(f4,true);
		AddPrimToList(f4,0);

		f4=GetPrimF4();
		setXYWH(f4,x+1,healthy+1,healthw,healthh);
		setRGB0(f4,0,0,0);
		setSemiTrans(f4,true);
		AddPrimToList(f4,1);

		x+=healthw+healthg;
	}
	char lifebuf[5];
	sprintf(lifebuf,"x%d",m_lives);
	s_debugFont.print(livesx,livesy,lifebuf);
#endif
}



/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
DVECTOR CPlayer::getCameraPos()
{
	return m_cameraPos;
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
void CPlayer::addHealth(int _health)
{
	s_health+=_health;
	if(s_health>MAX_HEALTH)
	{
		s_health=MAX_HEALTH;
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
const PlayerMetrics *CPlayer::getPlayerMetrics()
{
	return &s_modes[m_currentMode].m_metrics;
}





/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int CPlayer::setState(PLAYER_STATE _state)
{
	CPlayerState	*nextState;
	int				ret=false;

	nextState=s_modes[m_currentMode].m_states[_state];
	if(nextState)
	{
		m_currentStateClass=nextState;
		m_currentStateClass->enter(this);
		m_currentState=_state;
		ret=true;
	}
	return ret;
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
// Need to do something about this setState() for when the new mode doesn't have that state (pkg)
	if(!setState(m_currentState))
	{
		m_moveVel.vx=0;
		m_moveVel.vy=0;
		setState(STATE_IDLE);
	}
	s_modes[m_currentMode].m_modeControl->enter(this);
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
	if(m_facing!=_facing)
	{
		m_facing=_facing;
		m_skel.setDir(_facing);
	}
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
	return m_skel.getFrameCount();
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
DVECTOR CPlayer::getMoveVelocity()
{
	return m_moveVel;
}
void CPlayer::setMoveVelocity(DVECTOR *_moveVel)
{
	m_moveVel=*_moveVel;
}
DVECTOR CPlayer::getPlayerPos()
{
	return Pos;
}
void CPlayer::setPlayerPos(DVECTOR *_pos)
{
	Pos=*_pos;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
PLAYERINPUT CPlayer::getPadInputHeld()
{
	return m_padInput;
}
PLAYERINPUT CPlayer::getPadInputDown()
{
	return m_padInputDown;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int CPlayer::isOnSlippySurface()
{
	int	ret=false;

	if(m_layerCollision->getHeightFromGround(Pos.vx,Pos.vy,5)==0&&
	   m_layerCollision->getCollisionType(Pos.vx,Pos.vy)&COLLISION_TYPE_FLAG_SLIPPERY)
	{
		ret=true;
	}

	return ret;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:	FACING_LEFT if left half of player is hanging, FACING_RIGHT
				if right half of player is hanging or 0 if no part of the
				player is hanging
  ---------------------------------------------------------------------- */
int csize=5;
int cheight=15;
int CPlayer::isOnEdge()
{
	int	ret=0;

	if(m_layerCollision->getHeightFromGround(Pos.vx-csize,Pos.vy,cheight+1)>cheight)
	{
		ret=FACING_LEFT;
	}
	else if(m_layerCollision->getHeightFromGround(Pos.vx+csize,Pos.vy,cheight+1)>cheight)
	{
		ret=FACING_RIGHT;
	}
	return ret;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int CPlayer::canMoveLeft()
{
	return m_layerCollision->getHeightFromGround(Pos.vx-1,Pos.vy,16)>-8?true:false;
}
int CPlayer::canMoveRight()
{
	return m_layerCollision->getHeightFromGround(Pos.vx+1,Pos.vy,16)>-8?true:false;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayer::moveLeft()
{
	const PlayerMetrics	*metrics;
	metrics=getPlayerMetrics();

	setFacing(FACING_LEFT);
	if(m_moveVel.vx<=0)
	{
		m_moveVel.vx-=metrics->m_metric[PM__RUN_SPEEDUP];
		if(m_moveVel.vx<-metrics->m_metric[PM__MAX_RUN_VELOCITY]<<VELOCITY_SHIFT)
		{
			m_moveVel.vx=-metrics->m_metric[PM__MAX_RUN_VELOCITY]<<VELOCITY_SHIFT;
		}
	}
	else
	{
		m_moveVel.vx-=metrics->m_metric[PM__RUN_REVERSESLOWDOWN];
	}

	if(m_moveVel.vx<-CAMERA_STARTMOVETHRESHOLD||m_cameraScrollPos.vx<-CAMERA_SCROLLTHRESHOLD<<8)
	{
		m_cameraScrollDir=+1;
	}
	else if(m_moveVel.vx>-CAMERA_STOPMOVETHRESHOLD)
	{
		m_cameraScrollDir=0;
	}
}
void CPlayer::moveRight()
{
	const PlayerMetrics	*metrics;
	metrics=getPlayerMetrics();
	
	setFacing(FACING_RIGHT);
	if(m_moveVel.vx>=0)
	{
		m_moveVel.vx+=metrics->m_metric[PM__RUN_SPEEDUP];
		if(m_moveVel.vx>metrics->m_metric[PM__MAX_RUN_VELOCITY]<<VELOCITY_SHIFT)
		{
			m_moveVel.vx=metrics->m_metric[PM__MAX_RUN_VELOCITY]<<VELOCITY_SHIFT;
		}
	}
	else
	{
		m_moveVel.vx+=metrics->m_metric[PM__RUN_REVERSESLOWDOWN];
	}

	if(m_moveVel.vx>CAMERA_STARTMOVETHRESHOLD||m_cameraScrollPos.vx>CAMERA_SCROLLTHRESHOLD<<8)
	{
		m_cameraScrollDir=-1;
	}
	else if(m_moveVel.vx<CAMERA_STOPMOVETHRESHOLD)
	{
		m_cameraScrollDir=0;
	}
}
void CPlayer::slowdown()
{
	const PlayerMetrics	*metrics;
	int					stopSpeed;
	metrics=getPlayerMetrics();
	if(isOnSlippySurface())
	{
		stopSpeed=SLIPSPEED;
	}
	else
	{
		stopSpeed=0;
	}

	if(m_moveVel.vx<0)
	{
		if(-stopSpeed<m_moveVel.vx)
		{
			stopSpeed=-m_moveVel.vx;
		}
		m_moveVel.vx+=metrics->m_metric[PM__RUN_SLOWDOWN];
		if(m_moveVel.vx>-stopSpeed)
		{
			m_moveVel.vx=-stopSpeed;
			if(m_currentState==STATE_RUN)
			{
				setState(STATE_IDLE);
			}
		}
	}
	else if(m_moveVel.vx>0)
	{
		if(stopSpeed>m_moveVel.vx)
		{
			stopSpeed=m_moveVel.vx;
		}
		m_moveVel.vx-=metrics->m_metric[PM__RUN_SLOWDOWN];
		if(m_moveVel.vx<stopSpeed)
		{
			m_moveVel.vx=stopSpeed;
			if(m_currentState==STATE_RUN)
			{
				setState(STATE_IDLE);
			}
		}
	}
}

void CPlayer::jump()
{
}
void CPlayer::fall()
{
	const PlayerMetrics	*metrics;
	metrics=getPlayerMetrics();
	m_moveVel.vy+=PLAYER_GRAVITY;
	if(m_moveVel.vy>=PLAYER_TERMINAL_VELOCITY<<VELOCITY_SHIFT)
	{
		m_moveVel.vy=PLAYER_TERMINAL_VELOCITY<<VELOCITY_SHIFT;
		m_fallFrames++;
		if(m_currentState!=STATE_BUTTFALL)
		{
			if(m_fallFrames>metrics->m_metric[PM__MAX_SAFE_FALL_FRAMES])
			{
				setState(STATE_FALLFAR);
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
void CPlayer::respawn()
{
	setState(STATE_IDLE);

	// Strip any items that the player might be holding
	if(m_currentMode!=PLAYER_MODE_BASICUNARMED)
	{
		setMode(PLAYER_MODE_FULLUNARMED);
	}
	else
	{
		setMode(PLAYER_MODE_BASICUNARMED);
	}

	s_health=MAX_HEALTH;
	m_invincibleFrameCount=INVIBCIBLE_FRAMES__START;
	Pos=m_respawnPos;
	m_moveVel.vx=0;
	m_moveVel.vy=0;
	m_fallFrames=0;
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
			case DAMAGE__FALL:
			case DAMAGE__LAVA:
			case DAMAGE__HIT_ENEMY:
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
			if(s_health)
			{
				m_invincibleFrameCount=INVINCIBLE_FRAMES__HIT;
				s_health--;
			}
			else
			{
				CSoundMediator::playSfx(CSoundMediator::SFX_SPONGEBOB_DEFEATED_JINGLE);
				setState(STATE_DEAD);
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
	if(s_demoFrameCount==30*60)
	{
		writeDemoControls();
		ASSERT(!"DEMO ENDED");
	}
#endif

	return input;
}

/*===========================================================================
end */
