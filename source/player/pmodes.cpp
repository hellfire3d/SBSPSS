/*=========================================================================

	pmodes.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

/*----------------------------------------------------------------------
	Includes
	-------- */

#include "player\pmodes.h"

#ifndef __PLAYER_PLAYER_H__
#include "player\player.h"
#endif

#ifndef __PLAYER_PSTATES_H__
#include "player\pstates.h"
#endif

#ifndef __LAYER_COLLISION_H__
#include "level\layercollision.h"
#endif

// States
#ifndef __PLAYER__PSJUMP_H__
#include "player\psjump.h"
#endif

#ifndef __PLAYER__PSRUN_H__
#include "player\psrun.h"
#endif

#ifndef __PLAYER__PSFALL_H__
#include "player\psfall.h"
#endif

#ifndef __PLAYER__PSIDLE_H__
#include "player\psidle.h"
#endif

#ifndef __PLAYER__PSBUTT_H__
#include "player\psbutt.h"
#endif

#ifndef __PLAYER__PSDUCK_H__
#include "player\psduck.h"
#endif


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

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

/*----------------------------------------------------------------------
	Function Prototypes
	------------------- */

/*----------------------------------------------------------------------
	Vars
	---- */

static CPlayerStateUnarmedIdle		stateUnarmedIdle;
static CPlayerStateTeeterIdle		stateTeeterIdle;
static CPlayerStateJump				stateJump;
static CPlayerStateRun				stateRun;
static CPlayerStateFall				stateFall;
static CPlayerStateFallFar			stateFallFar;
static CPlayerStateDuck				stateDuck;
static CPlayerStateSoakUp			stateSoakUp;
static CPlayerStateGetUp			stateGetUp;
static CPlayerStateButtBounce		stateButtBounce;
static CPlayerStateButtBounceFall	stateButtBounceFall;
static CPlayerStateButtBounceLand	stateButtBounceLand;

/*
static CPlayerStateDead				stateDead;
*/

CPlayerState	*CPlayerModeBasic::s_stateTable[]=
{
	&stateUnarmedIdle,								// STATE_IDLE
	&stateTeeterIdle,								// STATE_IDLETEETER
	&stateJump,										// STATE_JUMP
	&stateRun,										// STATE_RUN
	&stateFall,										// STATE_FALL
	&stateFallFar,									// STATE_FALLFAR
	&stateButtBounce,								// STATE_BUTTBOUNCE
	&stateButtBounceFall,							// STATE_BUTTFALL
	&stateButtBounceLand,							// STATE_BUTTLAND
	&stateDuck,										// STATE_DUCK
	&stateSoakUp,									// STATE_SOAKUP
	&stateGetUp,									// STATE_GETUP
};

PlayerMetrics	CPlayerModeBasic::s_playerMetrics=
{	{
	DEFAULT_PLAYER_JUMP_VELOCITY,			// PM__JUMP_VELOCITY
	DEFAULT_PLAYER_MAX_JUMP_FRAMES,		// PM__MAX_JUMP_FRAMES
	DEFAULT_PLAYER_MAX_SAFE_FALL_FRAMES,	// PM__MAX_SAFE_FALL_FRAMES
	DEFAULT_PLAYER_MAX_RUN_VELOCITY,		// PM__MAX_RUN_VELOCITY
	DEFAULT_PLAYER_RUN_SPEEDUP,			// PM__RUN_SPEEDUP
	DEFAULT_PLAYER_RUN_REVERSESLOWDOWN,	// PM__RUN_REVERSESLOWDOWN
	DEFAULT_PLAYER_RUN_SLOWDOWN,			// PM__RUN_SLOWDOWN
}	};












/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int		CPlayerMode::getPadInputHeld()					{return m_player->getPadInputHeld();}
int		CPlayerMode::getPadInputDown()					{return m_player->getPadInputDown();}
DVECTOR	CPlayerMode::getPlayerPos()							{return m_player->getPlayerPos();}
void	CPlayerMode::setPlayerPos(DVECTOR *_pos)			{m_player->setPlayerPos(_pos);}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CPlayerModeBasic::enter()
{
	m_fallFrames=0;
	setState(STATE_IDLE);
	m_moveVelocity.vx=m_moveVelocity.vy=0;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CPlayerModeBasic::think()
{
	s_stateTable[m_currentState]->think(this);
	thinkVerticalMovement();
	thinkHorizontalMovement();

	// Teeter if on an edge
	if(canTeeter()&&isOnEdge())
	{
		setState(STATE_IDLETEETER);
	}

}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CPlayerModeBasic::render()
{
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CPlayerModeBasic::thinkVerticalMovement()
{
	CLayerCollision	*collision;
	DVECTOR			pos;
	int				colHeight;

	collision=m_player->getLayerCollision();
	pos=m_player->getPlayerPos();

	colHeight=collision->getHeightFromGround(pos.vx,pos.vy,1);

//New collision stuff (pkg)
//if(m_layerCollision->getCollisionType(Pos.vx,Pos.vy+(m_moveVelocity.vy>>VELOCITY_SHIFT))&COLLISION_TYPE_MASK)
//{
//	m_moveVelocity.vy=0;
//	return;
//}
	if(colHeight>=0)
	{
		// Above or on the ground
		// Are we falling?
		if(m_moveVelocity.vy>0)
		{
			// Yes.. Check to see if we're about to hit/go through the ground
			colHeight=collision->getHeightFromGround(pos.vx,pos.vy+(m_moveVelocity.vy>>VELOCITY_SHIFT),PLAYER_TERMINAL_VELOCITY+1);

			if(colHeight<=0)
			{
				// Just hit the ground
				// Stick at ground level
				pos.vy+=(m_moveVelocity.vy>>VELOCITY_SHIFT)+colHeight;
				m_moveVelocity.vy=0;
				m_fallFrames=0;
				if(m_currentState==STATE_BUTTFALL)
				{
					// Landed from a butt bounce
					setState(STATE_BUTTLAND);
				}
				else if(m_currentState==STATE_FALLFAR)
				{
					// Landed from a painfully long fall
					setState(STATE_IDLE);
					m_player->takeDamage(DAMAGE__FALL);
					m_moveVelocity.vx=0;
					CSoundMediator::playSfx(CSoundMediator::SFX_SPONGEBOB_LAND_AFTER_FALL);
				}
				else if(m_moveVelocity.vx)
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
		else if(colHeight)
		{
			if(m_currentState!=STATE_FALL&&m_currentState!=STATE_FALLFAR&&
			   m_currentState!=STATE_BUTTFALL&&m_currentState!=STATE_BUTTBOUNCE&&
			   m_currentState!=STATE_JUMP)
			{
				// Was floating in the air.. fall!

//				if ( !m_onPlatform )
//				{
					setState(STATE_FALL);
//				}
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

	pos.vy+=m_moveVelocity.vy>>VELOCITY_SHIFT;
	m_player->setPlayerPos(&pos);
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CPlayerModeBasic::thinkHorizontalMovement()
{
	if(m_moveVelocity.vx)
	{
		CLayerCollision	*collision;
		DVECTOR			pos;

		collision=m_player->getLayerCollision();
		pos=m_player->getPlayerPos();
//New collision stuff (pkg)
//if(m_layerCollision->getCollisionType(Pos.vx+(m_moveVelocity.vx>>VELOCITY_SHIFT),Pos.vy)&COLLISION_TYPE_MASK)
//{
//	m_moveVelocity.vx=0;
//	return;
//}
		int colHeight;
		colHeight=collision->getHeightFromGround(pos.vx,pos.vy,5);
		if(colHeight==0)
		{
			// Ok.. we're on the ground. What happens if we move left/right
			colHeight=collision->getHeightFromGround(pos.vx+(m_moveVelocity.vx>>VELOCITY_SHIFT),pos.vy);
			if(colHeight<-8)
			{
				// Big step up. Stop at the edge of the obstruction
				int	dir,vx,cx,i;
				if(m_moveVelocity.vx<0)
				{
					dir=-1;
					vx=-m_moveVelocity.vx>>VELOCITY_SHIFT;
				}
				else
				{
					dir=+1;
					vx=m_moveVelocity.vx>>VELOCITY_SHIFT;
				}
				cx=pos.vx;
				for(i=0;i<vx;i++)
				{
					if(collision->getHeightFromGround(cx,pos.vy)<-8)
					{
						break;
					}
					cx+=dir;
				}
				pos.vx=cx-dir;

				// If running then go to idle, otherwise leave in same state
				if(m_currentState==STATE_RUN)
				{
					setState(STATE_IDLE);
				}
				m_moveVelocity.vx=0;
				
				// Get the height at this new position and then try the step-up code below.
				// Without this, there are problems when you run up a slope and hit a wall at the same time
				colHeight=collision->getHeightFromGround(pos.vx,pos.vy);
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
//			if(!(colHeight<0&&m_currentState==STATE_JUMP)) // Lets you jump through platforms from below
			if(colHeight>=0) // Lets you jump through platforms from below
			{
				colHeight=collision->getHeightFromGround(pos.vx+(m_moveVelocity.vx>>VELOCITY_SHIFT),pos.vy,5);
				if(colHeight<0)
				{
					// Stop at the edge of the obstruction
					int	dir,vx,cx,i;
					if(m_moveVelocity.vx<0)
					{
						dir=-1;
						vx=-m_moveVelocity.vx>>VELOCITY_SHIFT;
					}
					else
					{
						dir=+1;
						vx=m_moveVelocity.vx>>VELOCITY_SHIFT;
					}
					cx=pos.vx;
					for(i=0;i<vx;i++)
					{
						if(collision->getHeightFromGround(cx,pos.vy)<0)
						{
							break;
						}
						cx+=dir;
					}
					pos.vx=cx-dir;
					m_moveVelocity.vx=0;
				}
			}
		}
		pos.vx+=m_moveVelocity.vx>>VELOCITY_SHIFT;
		m_player->setPlayerPos(&pos);
	}
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
const struct PlayerMetrics	*CPlayerModeBasic::getPlayerMetrics()
{
	return &s_playerMetrics;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int		CPlayerModeBasic::setState(int _state)
{
	CPlayerState	*nextState;
	int				ret=false;

	nextState=s_stateTable[_state];
	if(nextState)
	{
		m_currentStateClass=nextState;
		m_currentStateClass->enter(this);
		m_currentState=(PLAYER_STATE)_state;
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
int		CPlayerModeBasic::getFacing()								{return m_player->getFacing();}
void	CPlayerModeBasic::setFacing(int _facing)						{m_player->setFacing(_facing);}
void	CPlayerModeBasic::setAnimNo(int _animNo)						{m_player->setAnimNo(_animNo);}
void	CPlayerModeBasic::setAnimFrame(int _animFrame)				{m_player->setAnimFrame(_animFrame);}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int		CPlayerModeBasic::advanceAnimFrameAndCheckForEndOfAnim()
{
	int	animFrame,frameCount;
	int	looped;
	animFrame=m_player->getAnimFrame()+1;
	frameCount=m_player->getAnimFrameCount();
	looped=false;
	if(animFrame>=frameCount)
	{
		looped=true;
		animFrame=0;
	}
	m_player->setAnimFrame(animFrame);
	return looped;

}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
DVECTOR	CPlayerModeBasic::getMoveVelocity()								{return m_moveVelocity;}
void	CPlayerModeBasic::zeroMoveVelocity()							{m_moveVelocity.vx=m_moveVelocity.vy=0;}
void	CPlayerModeBasic::setMoveVelocity(DVECTOR *_moveVel)			{m_moveVelocity=*_moveVel;}

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
int CPlayerModeBasic::isOnEdge()
{
	CLayerCollision	*collision;
	DVECTOR			pos;
	int	ret;

	collision=m_player->getLayerCollision();
	pos=m_player->getPlayerPos();
	ret=0;
	if(collision->getHeightFromGround(pos.vx-csize,pos.vy,cheight+1)>cheight)
	{
		ret=FACING_LEFT;
	}
	else if(collision->getHeightFromGround(pos.vx+csize,pos.vy,cheight+1)>cheight)
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
int CPlayerModeBasic::canMoveLeft()
{
	DVECTOR			pos;
	pos=m_player->getPlayerPos();
	return m_player->getLayerCollision()->getHeightFromGround(pos.vx-1,pos.vy,16)>-8?true:false;
}

int CPlayerModeBasic::canMoveRight()
{
	DVECTOR			pos;
	pos=m_player->getPlayerPos();
	return m_player->getLayerCollision()->getHeightFromGround(pos.vx+1,pos.vy,16)>-8?true:false;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CPlayerModeBasic::moveLeft()
{
	const PlayerMetrics	*metrics;
	metrics=getPlayerMetrics();

	setFacing(FACING_LEFT);
	if(m_moveVelocity.vx<=0)
	{
		m_moveVelocity.vx-=metrics->m_metric[PM__RUN_SPEEDUP];
		if(m_moveVelocity.vx<-metrics->m_metric[PM__MAX_RUN_VELOCITY]<<VELOCITY_SHIFT)
		{
			m_moveVelocity.vx=-metrics->m_metric[PM__MAX_RUN_VELOCITY]<<VELOCITY_SHIFT;
		}
	}
	else
	{
		m_moveVelocity.vx-=metrics->m_metric[PM__RUN_REVERSESLOWDOWN];
	}

	/*
	if(m_moveVelocity.vx<-CAMERA_STARTMOVETHRESHOLD||m_cameraScrollPos.vx<-CAMERA_SCROLLTHRESHOLD<<8)
	{
		m_cameraScrollDir=+1;
	}
	else if(m_moveVelocity.vx>-CAMERA_STOPMOVETHRESHOLD)
	{
		m_cameraScrollDir=0;
	}
	*/
}

void	CPlayerModeBasic::moveRight()
{
	const PlayerMetrics	*metrics;
	metrics=getPlayerMetrics();
	
	setFacing(FACING_RIGHT);
	if(m_moveVelocity.vx>=0)
	{
		m_moveVelocity.vx+=metrics->m_metric[PM__RUN_SPEEDUP];
		if(m_moveVelocity.vx>metrics->m_metric[PM__MAX_RUN_VELOCITY]<<VELOCITY_SHIFT)
		{
			m_moveVelocity.vx=metrics->m_metric[PM__MAX_RUN_VELOCITY]<<VELOCITY_SHIFT;
		}
	}
	else
	{
		m_moveVelocity.vx+=metrics->m_metric[PM__RUN_REVERSESLOWDOWN];
	}

	/*
	if(m_moveVelocity.vx>CAMERA_STARTMOVETHRESHOLD||m_cameraScrollPos.vx>CAMERA_SCROLLTHRESHOLD<<8)
	{
		m_cameraScrollDir=-1;
	}
	else if(m_moveVelocity.vx<CAMERA_STOPMOVETHRESHOLD)
	{
		m_cameraScrollDir=0;
	}
	*/
}
int		CPlayerModeBasic::slowdown()
{
	const PlayerMetrics	*metrics;
	int					ret=false;
	metrics=getPlayerMetrics();

	if(m_moveVelocity.vx<0)
	{
		m_moveVelocity.vx+=metrics->m_metric[PM__RUN_SLOWDOWN];
		if(m_moveVelocity.vx>=0)
		{
			m_moveVelocity.vx=0;
			ret=true;
		}
	}
	else if(m_moveVelocity.vx>0)
	{
		m_moveVelocity.vx-=metrics->m_metric[PM__RUN_SLOWDOWN];
		if(m_moveVelocity.vx<=0)
		{
			m_moveVelocity.vx=0;
			ret=true;
		}
	}
	return ret;
}
void	CPlayerModeBasic::jump()
{
	const PlayerMetrics	*metrics;
	metrics=getPlayerMetrics();
	m_moveVelocity.vy=-metrics->m_metric[PM__JUMP_VELOCITY]<<VELOCITY_SHIFT;
}
void	CPlayerModeBasic::fall()
{
	m_moveVelocity.vy+=PLAYER_GRAVITY;
	if(m_moveVelocity.vy>=PLAYER_TERMINAL_VELOCITY<<VELOCITY_SHIFT)
	{
		m_moveVelocity.vy=PLAYER_TERMINAL_VELOCITY<<VELOCITY_SHIFT;
		if(!canFallForever()&&m_currentState!=STATE_FALLFAR)
		{
			const PlayerMetrics	*metrics;
			metrics=getPlayerMetrics();
			m_fallFrames++;
			if(m_fallFrames>metrics->m_metric[PM__MAX_SAFE_FALL_FRAMES])
			{
				setState(STATE_FALLFAR);
			}
		}
	}
}



/*===========================================================================
end */
