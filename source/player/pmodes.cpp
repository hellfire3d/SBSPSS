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

#ifndef __PLAYER__PSHITGND_H__
#include "player\pshitgnd.h"
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

static	CPlayerState	*s_stateTable[]=
{
	&s_stateUnarmedIdle,					// STATE_IDLE
	&s_stateTeeterIdle,						// STATE_IDLETEETER
	&s_stateJump,							// STATE_JUMP
	&s_stateRun,							// STATE_RUN
	&s_stateFall,							// STATE_FALL
	&s_stateFallFar,						// STATE_FALLFAR
	&s_stateHitGround,						// STATE_HITGROUND
	&s_stateButtBounce,						// STATE_BUTTBOUNCE
	&s_stateButtBounceFall,					// STATE_BUTTFALL
	&s_stateButtBounceLand,					// STATE_BUTTLAND
	&s_stateDuck,							// STATE_DUCK
	&s_stateSoakUp,							// STATE_SOAKUP
	&s_stateGetUp,							// STATE_GETUP
};

static	PlayerMetrics	s_playerMetrics=
{	{
	DEFAULT_PLAYER_JUMP_VELOCITY,			// PM__JUMP_VELOCITY
	DEFAULT_PLAYER_MAX_JUMP_FRAMES,			// PM__MAX_JUMP_FRAMES
	DEFAULT_PLAYER_MAX_SAFE_FALL_FRAMES,	// PM__MAX_SAFE_FALL_FRAMES
	DEFAULT_PLAYER_MAX_RUN_VELOCITY,		// PM__MAX_RUN_VELOCITY
	DEFAULT_PLAYER_RUN_SPEEDUP,				// PM__RUN_SPEEDUP
	DEFAULT_PLAYER_RUN_REVERSESLOWDOWN,		// PM__RUN_REVERSESLOWDOWN
	DEFAULT_PLAYER_RUN_SLOWDOWN,			// PM__RUN_SLOWDOWN
	DEFAULT_PLAYER_PLAYER_GRAVITY,			// PM__GRAVITY
	DEFAULT_PLAYER_TERMINAL_VELOCITY,		// PM__TERMINAL_VELOCITY
	DEFAULT_BUTT_FALL_VELOCITY,				// PM__BUTT_FALL_VELOCITY
}	};


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int		CPlayerMode::getPadInputHeld()					{return m_player->getPadInputHeld();}
int		CPlayerMode::getPadInputDown()					{return m_player->getPadInputDown();}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
DVECTOR	CPlayerMode::getPlayerPos()						{return m_player->getPlayerPos();}
void	CPlayerMode::setPlayerPos(DVECTOR *_pos)		{m_player->setPlayerPos(_pos);}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CPlayerMode::respawn()							{m_player->respawn();}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CPlayerModeBase::enter()
{
	m_fallFrames=0;
	setState(STATE_IDLE);
	zeroMoveVelocity();
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CPlayerModeBase::think()
{
	getStateTable()[m_currentState]->think(this);
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
int		CPlayerModeBase::canDoLookAround()
{
	int	ret=false;

	switch(getState())
	{
		case STATE_IDLE:
		case STATE_IDLETEETER:
		case STATE_SOAKUP:
			ret=true;
			break;

		case STATE_JUMP:
		case STATE_RUN:
		case STATE_FALL:
		case STATE_FALLFAR:
		case STATE_HITGROUND:
		case STATE_BUTTBOUNCE:
		case STATE_BUTTFALL:
		case STATE_BUTTLAND:
		case STATE_DUCK:
		case STATE_GETUP:
			break;
	}

	return ret;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
ATTACK_STATE	CPlayerModeBase::getAttackState()
{
	ATTACK_STATE	ret=ATTACK_STATE__NONE;

	switch(getState())
	{
		case STATE_BUTTFALL:
		case STATE_BUTTLAND:
			ret=ATTACK_STATE__BUTT_BOUNCE;
			break;

		case STATE_IDLE:
		case STATE_IDLETEETER:
		case STATE_JUMP:
		case STATE_RUN:
		case STATE_FALL:
		case STATE_FALLFAR:
		case STATE_BUTTBOUNCE:
		case STATE_DUCK:
		case STATE_SOAKUP:
		case STATE_GETUP:
			break;
	}

	return ret;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CPlayerModeBase::thinkVerticalMovement()
{
	if(m_player->moveVertical(m_player->getMoveVelocity()->vy>>VELOCITY_SHIFT))
	{
		playerHasHitGround();
	}
	else if(m_currentState!=STATE_FALL&&m_currentState!=STATE_FALLFAR&&
			m_currentState!=STATE_BUTTFALL&&m_currentState!=STATE_BUTTBOUNCE&&
			m_currentState!=STATE_JUMP)
	{
		DVECTOR	pos;
		pos=m_player->getPlayerPos();
		if(m_player->getHeightFromGround(pos.vx,pos.vy,1)!=0)
		{
			// Was floating in the air.. fall!
			setState(STATE_FALL);
		}
	}
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CPlayerModeBase::thinkHorizontalMovement()
{
	DVECTOR	moveVel;

	moveVel=*m_player->getMoveVelocity();
	if(m_player->moveHorizontal(moveVel.vx>>VELOCITY_SHIFT))
	{
		// If running then go to idle, otherwise leave in same state
		if(m_currentState==STATE_RUN)
		{
			setState(STATE_IDLE);
		}
		moveVel.vx=0;
		setMoveVelocity(&moveVel);
	}
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CPlayerModeBase::playerHasHitGround()
{
	DVECTOR	moveVel;

	moveVel=*m_player->getMoveVelocity();
	moveVel.vy=0;
	m_fallFrames=0;
	if(m_currentState==STATE_BUTTFALL)
	{
		// Landed from a butt bounce
		setState(STATE_BUTTLAND);
	}
	else if(m_currentState==STATE_FALLFAR)
	{
		// Landed from a painfully long fall
		setState(STATE_HITGROUND);
		m_player->takeDamage(DAMAGE__FALL);
		moveVel.vx=0;
	}
	else if(moveVel.vx)
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
	setMoveVelocity(&moveVel);
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
const struct PlayerMetrics	*CPlayerModeBase::getPlayerMetrics()
{
	return &s_playerMetrics;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int		CPlayerModeBase::setState(int _state)
{
	CPlayerState	*nextState;
	int				ret=false;

	nextState=getStateTable()[_state];
	if(nextState)
	{
		m_player->resetPlayerCollisionSizeToBase();
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
int		CPlayerModeBase::getFacing()								{return m_player->getFacing();}
void	CPlayerModeBase::setFacing(int _facing)						{m_player->setFacing(_facing);}
int		CPlayerModeBase::getAnimNo()								{return m_player->getAnimNo();}
void	CPlayerModeBase::setAnimNo(int _animNo)						{m_player->setAnimNo(_animNo);setAnimFrame(0);}
int		CPlayerModeBase::getAnimFrame()								{return m_player->getAnimFrame();}
int		CPlayerModeBase::getAnimFrameCount()						{return m_player->getAnimFrameCount();}
void	CPlayerModeBase::setAnimFrame(int _animFrame)				{m_player->setAnimFrame(_animFrame);}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int		CPlayerModeBase::advanceAnimFrameAndCheckForEndOfAnim()
{
	int	animFrame,frameCount;
	int	looped;
	animFrame=getAnimFrame()+1;
	frameCount=getAnimFrameCount();
	looped=false;
	if(animFrame>=frameCount)
	{
		looped=true;
		animFrame=0;
	}
	setAnimFrame(animFrame);
	return looped;

}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
DVECTOR	CPlayerModeBase::getMoveVelocity()							{return *m_player->getMoveVelocity();}
void	CPlayerModeBase::zeroMoveVelocity()							{DVECTOR v={0,0};setMoveVelocity(&v);}
void	CPlayerModeBase::setMoveVelocity(DVECTOR *_moveVel)			{m_player->setMoveVelocity(_moveVel);}

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
int CPlayerModeBase::isOnEdge()
{
	CLayerCollision	*collision;
	DVECTOR			pos;
	int	ret;

	collision=m_player->getLayerCollision();
	pos=m_player->getPlayerPos();
	ret=0;
	if(m_player->getHeightFromGround(pos.vx-csize,pos.vy,cheight+1)>cheight)
	{
		ret=FACING_LEFT;
	}
	else if(m_player->getHeightFromGround(pos.vx+csize,pos.vy,cheight+1)>cheight)
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
int CPlayerModeBase::canMoveLeft()
{
	DVECTOR			pos;
	pos=m_player->getPlayerPos();
	return m_player->getHeightFromGround(pos.vx-1,pos.vy,16)>-8?true:false;
}

int CPlayerModeBase::canMoveRight()
{
	DVECTOR			pos;
	pos=m_player->getPlayerPos();
	return m_player->getHeightFromGround(pos.vx+1,pos.vy,16)>-8?true:false;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CPlayerModeBase::setPlayerCollisionSize(int _x,int _y,int _w,int _h)
{
	m_player->setPlayerCollisionSize(_x,_y,_w,_h);
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CPlayerModeBase::moveLeft()
{
	const PlayerMetrics	*metrics;
	DVECTOR				moveVel;

	metrics=getPlayerMetrics();
	moveVel=*m_player->getMoveVelocity();
	setFacing(FACING_LEFT);
	if(moveVel.vx<=0)
	{
		moveVel.vx-=metrics->m_metric[PM__RUN_SPEEDUP];
		if(moveVel.vx<-metrics->m_metric[PM__MAX_RUN_VELOCITY]<<VELOCITY_SHIFT)
		{
			moveVel.vx=-metrics->m_metric[PM__MAX_RUN_VELOCITY]<<VELOCITY_SHIFT;
		}
	}
	else
	{
		moveVel.vx-=metrics->m_metric[PM__RUN_REVERSESLOWDOWN];
	}
	setMoveVelocity(&moveVel);
}

void	CPlayerModeBase::moveRight()
{
	const PlayerMetrics	*metrics;
	DVECTOR				moveVel;

	metrics=getPlayerMetrics();
	moveVel=*m_player->getMoveVelocity();
	setFacing(FACING_RIGHT);
	if(moveVel.vx>=0)
	{
		moveVel.vx+=metrics->m_metric[PM__RUN_SPEEDUP];
		if(moveVel.vx>metrics->m_metric[PM__MAX_RUN_VELOCITY]<<VELOCITY_SHIFT)
		{
			moveVel.vx=metrics->m_metric[PM__MAX_RUN_VELOCITY]<<VELOCITY_SHIFT;
		}
	}
	else
	{
		moveVel.vx+=metrics->m_metric[PM__RUN_REVERSESLOWDOWN];
	}
	setMoveVelocity(&moveVel);
}
int		CPlayerModeBase::slowdown()
{
	const PlayerMetrics	*metrics;
	DVECTOR				moveVel;
	int					ret=false;

	metrics=getPlayerMetrics();
	moveVel=*m_player->getMoveVelocity();

	if(moveVel.vx<0)
	{
		moveVel.vx+=metrics->m_metric[PM__RUN_SLOWDOWN];
		if(moveVel.vx>=0)
		{
			moveVel.vx=0;
			ret=true;
		}
	}
	else if(moveVel.vx>0)
	{
		moveVel.vx-=metrics->m_metric[PM__RUN_SLOWDOWN];
		if(moveVel.vx<=0)
		{
			moveVel.vx=0;
			ret=true;
		}
	}
	else
	{
		// Hmm.. was already stopped(?)
		// This should probly be considered a bug.. (pkg)
		ret=true;
	}
	setMoveVelocity(&moveVel);
	return ret;
}
void	CPlayerModeBase::jump()
{
	DVECTOR				moveVel;
	moveVel=*m_player->getMoveVelocity();
	moveVel.vy=-getPlayerMetrics()->m_metric[PM__JUMP_VELOCITY]<<VELOCITY_SHIFT;
	setMoveVelocity(&moveVel);
}
void	CPlayerModeBase::fall()
{
	const PlayerMetrics	*metrics;
	DVECTOR				moveVel;

	metrics=getPlayerMetrics();
	moveVel=*m_player->getMoveVelocity();

	moveVel.vy+=getPlayerMetrics()->m_metric[PM__GRAVITY];
	if(moveVel.vy>=metrics->m_metric[PM__TERMINAL_VELOCITY]<<VELOCITY_SHIFT)
	{
		moveVel.vy=metrics->m_metric[PM__TERMINAL_VELOCITY]<<VELOCITY_SHIFT;
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
	setMoveVelocity(&moveVel);
}
int buttfallspeed=9;
void	CPlayerModeBase::buttFall()
{
	const PlayerMetrics	*metrics;
	DVECTOR				moveVel;

	metrics=getPlayerMetrics();
	moveVel=*m_player->getMoveVelocity();
//	moveVel.vy=metrics->m_metric[PM__BUTT_FALL_VELOCITY]<<(VELOCITY_SHIFT+1);
	moveVel.vy=metrics->m_metric[buttfallspeed]<<VELOCITY_SHIFT;
	setMoveVelocity(&moveVel);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
CPlayerState	**CPlayerModeBase::getStateTable()
{
	return s_stateTable;
}

/*===========================================================================
end */
