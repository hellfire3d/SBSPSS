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

#ifndef __PAD_VIBE_H__
#include "pad\vibe.h"
#endif

// States
#ifndef __PLAYER__PSLOOK_H__
#include "player\pslook.h"
#endif

#ifndef __PLAYER__PSJUMP_H__
#include "player\psjump.h"
#endif

#ifndef __PLAYER__PSJMPBCK_H__
#include "player\psjmpbck.h"
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

#ifndef __PLAYER__PSSPRING_H__
#include "player\psspring.h"
#endif

#ifndef __PLAYER_PSCART_H__
#include "player\pscart.h"
#endif

#ifndef __PLAYER_PSSWAL_H__
#include "player\psswal.h"
#endif

#ifndef __PLAYER_PSFLOAT_H__
#include "player\psfloat.h"
#endif

#ifndef __PLATFORM_PLATFORM_H__
#include "platform\platform.h"
#endif

#ifndef __PLAYER_PSDANCE_H__
#include "player\psdance.h"
#endif

#ifndef __GAME_GAME_H__
#include "game/game.h"
#endif

#ifndef __FX_FX_H__
#include "fx\fx.h"
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
	&s_stateSpring,							// STATE_SPRINGUP
	&s_stateRun,							// STATE_RUN
	&s_stateFall,							// STATE_FALL
	&s_stateFallFar,						// STATE_FALLFAR
	&s_stateHitGround,						// STATE_HITGROUND
	&s_stateButtBounce,						// STATE_BUTTBOUNCE
	&s_stateButtBounceFall,					// STATE_BUTTFALL
	&s_stateButtBounceLand,					// STATE_BUTTLAND
	&s_stateButtBounceUp,					// STATE_BUTTBOUNCEUP
	&s_stateSoakUp,							// STATE_SOAKUP
	&s_stateGetUp,							// STATE_GETUP
	&s_stateLookDown,						// STATE_LOOKDOWN
	&s_stateLookDownRelax,					// STATE_LOOKDOWNRELAX
	&s_stateLookUp,							// STATE_LOOKUP
	&s_stateLookUpRelax,					// STATE_LOOKUPRELAX
	&s_stateJumpBack,						// STATE_JUMPBACK
	&s_stateCart,							// STATE_CART
	&s_stateFloat,							// STATE_FLOAT
	&s_stateDance,							// STATE_CELEBRATE
	&s_stateSwallow,						// STATE_SWALLOW
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
	DEFAULT_HITREACT_XVELOCITY,				// PM__HITREACT_XVELOCITY
	DEFAULT_HITREACT_YVELOCITY,				// PM__HITREACT_YVELOCITY
	DEFAULT_HITREACT_FRAMES,				// PM__HITREACT_FRAMES
}	};




int checkx=8;
int checky=15;
int checkycanmove=16;
int checkdist=16;


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
extern int	checkx;

int		CPlayerMode::getHeightFromGound()
{
	int				height[2];
	DVECTOR	const &pos=getPlayerPos();

	height[0]=m_player->getHeightFromGround(pos.vx-checkx,pos.vy);
	height[1]=m_player->getHeightFromGround(pos.vx+checkx,pos.vy);

	return height[0]<height[1]?height[0]:height[1];
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CPlayerMode::inSoakUpState()					{m_player->inSoakUpState();}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
DVECTOR	const &CPlayerMode::getPlayerPos()						{return m_player->getPlayerPos();}
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
		case STATE_LOOKDOWN:
		case STATE_LOOKUP:
			ret=true;
			break;

		case STATE_JUMP:
		case STATE_SPRINGUP:
		case STATE_RUN:
		case STATE_FALL:
		case STATE_FALLFAR:
		case STATE_HITGROUND:
		case STATE_BUTTBOUNCE:
		case STATE_BUTTFALL:
		case STATE_BUTTLAND:
		case STATE_BUTTBOUNCEUP:
		case STATE_GETUP:
		case STATE_LOOKDOWNRELAX:
		case STATE_LOOKUPRELAX:
		case STATE_JUMPBACK:
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
		case STATE_BUTTBOUNCEUP:
			ret=ATTACK_STATE__BUTT_BOUNCE;
			break;

		case STATE_IDLE:
		case STATE_IDLETEETER:
		case STATE_JUMP:
		case STATE_SPRINGUP:
		case STATE_RUN:
		case STATE_FALL:
		case STATE_FALLFAR:
		case STATE_BUTTBOUNCE:
		case STATE_SOAKUP:
		case STATE_GETUP:
		case STATE_LOOKDOWN:
		case STATE_LOOKDOWNRELAX:
		case STATE_LOOKUP:
		case STATE_LOOKUPRELAX:
		case STATE_JUMPBACK:
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
		if(m_currentState!=STATE_CELEBRATE)
			playerHasHitGround();
		DVECTOR const &pos=m_player->getPlayerPos();
		if(m_player->getHeightFromGround(pos.vx,pos.vy,5)==0&&
		   (CGameScene::getCollision()->getCollisionBlock(pos.vx,pos.vy)&COLLISION_TYPE_MASK)==COLLISION_TYPE_FLAG_SOAKUP)
		{
			// Hit water - Make a splash!
			DVECTOR pos;
			CSoundMediator::playSfx(CSoundMediator::SFX_SPONGEBOB_SPLASH_INTO_WATER);
			pos=getPlayerPos();
			pos.vy-=40;
			CFX::Create(CFX::FX_TYPE_SPLASH_WATER,pos);

			if(!m_player->getIsHelmetSoFullThatIDontNeedToSoakUp())
			{
				// Not got full water levels - Go into soakup mode
				setState(STATE_SOAKUP);
			}
		}
	}
	else if(m_currentState!=STATE_FALL&&m_currentState!=STATE_FALLFAR&&				// Hmm.. (pkg)
			m_currentState!=STATE_BUTTFALL&&m_currentState!=STATE_BUTTBOUNCE&&
			m_currentState!=STATE_JUMP&&m_currentState!=STATE_SPRINGUP&&
			m_currentState!=STATE_JUMPBACK&&m_currentState!=STATE_BUTTBOUNCEUP&&
			m_currentState!=STATE_FLOAT&&m_currentState!=STATE_CELEBRATE)
	{
		DVECTOR	const &pos=m_player->getPlayerPos();

		if(m_player->getHeightFromGround(pos.vx,pos.vy,1)!=0&&
		   m_player->getHeightFromGround(pos.vx-checkx,pos.vy,1)!=0&&
		   m_player->getHeightFromGround(pos.vx+checkx,pos.vy,1)!=0
		   )
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

	m_player->setLockoutPlatform( NULL );

	if(m_currentState==STATE_BUTTFALL)
	{
		// Landed from a butt bounce
		setState(STATE_BUTTLAND);
	}
	else if(m_currentState==STATE_FALLFAR)
	{
		// Landed from a painfully long fall
		// This no longer hurts as per THQs request..
		setState(STATE_HITGROUND);
#ifdef __USER_paul__
		m_player->takeDamage(DAMAGE__FALL,REACT__NO_REACTION);
#else
		CPadVibrationManager::setVibration(0,CPadVibrationManager::VIBE_SHORT);
#endif

		moveVel.vx=0;
		CGameScene::setCameraShake(0,8);
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
	Purpose:	Sets player state.
				NB: When you re-enter a state due to changing modes ( ie: when picking up
				a weapon ) then strange things can happen :/
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int		CPlayerModeBase::setState(int _state)
{
	int				ret=false;

	if(m_currentState==STATE_CELEBRATE&&_state!=STATE_IDLE)
	{
		ret=true;
	}
	else
	{
		CPlayerState	*nextState;

		nextState=getStateTable()[_state];
		if(nextState)
		{
			m_player->resetPlayerCollisionSizeToBase();
			m_currentStateClass=nextState;
			m_currentStateClass->enter(this);
			m_currentState=(PLAYER_STATE)_state;
			ret=true;
		}
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
	CLayerCollision	*collision=CGameScene::getCollision();
	int	ret;

	DVECTOR const &pos=m_player->getPlayerPos();
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
	DVECTOR	const &pos=m_player->getPlayerPos();
	return m_player->getHeightFromGround(pos.vx-checkx-1,pos.vy,checkdist)>-checkycanmove?true:false;

}

int CPlayerModeBase::canMoveRight()
{
	DVECTOR	const &pos=m_player->getPlayerPos();
	return m_player->getHeightFromGround(pos.vx+checkx+1,pos.vy,checkdist)>-checkycanmove?true:false;
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
void	CPlayerModeBase::getPlayerCollisionSize(int *_x,int *_y,int *_w,int *_h)
{
	m_player->getPlayerCollisionSize(_x,_y,_w,_h);
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
	m_player->moveLeft();
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
	m_player->moveRight();
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
int spr=5;
void	CPlayerModeBase::spring()
{
	DVECTOR				moveVel;
	moveVel=*m_player->getMoveVelocity();
//	moveVel.vy=-getPlayerMetrics()->m_metric[PM__JUMP_VELOCITY]<<VELOCITY_SHIFT;
	moveVel.vy-=spr;
	if(moveVel.vy<-getPlayerMetrics()->m_metric[PM__JUMP_VELOCITY]<<VELOCITY_SHIFT)
	{
		moveVel.vy=-getPlayerMetrics()->m_metric[PM__JUMP_VELOCITY]<<VELOCITY_SHIFT;
	}
	setMoveVelocity(&moveVel);
}
void	CPlayerModeBase::jumpback()
{
	DVECTOR				moveVel;
	int					xvel;
	moveVel=*m_player->getMoveVelocity();
	moveVel.vy=-getPlayerMetrics()->m_metric[PM__HITREACT_YVELOCITY]<<VELOCITY_SHIFT;
	xvel=getPlayerMetrics()->m_metric[PM__HITREACT_XVELOCITY]<<VELOCITY_SHIFT;
	if(moveVel.vx<0)
	{
		moveVel.vx=-xvel;
	}
	else if(moveVel.vx>0)
	{
		moveVel.vx=xvel;
	}
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
	m_player->fall();
}
// whatever..
void	CPlayerModeBase::fallToDance()
{
	const PlayerMetrics	*metrics;
	DVECTOR				moveVel;

	metrics=getPlayerMetrics();
	moveVel=*m_player->getMoveVelocity();

	moveVel.vy+=getPlayerMetrics()->m_metric[PM__GRAVITY];
	if(moveVel.vy>=metrics->m_metric[PM__TERMINAL_VELOCITY]<<VELOCITY_SHIFT)
	{
		moveVel.vy=metrics->m_metric[PM__TERMINAL_VELOCITY]<<VELOCITY_SHIFT;
	}
	setMoveVelocity(&moveVel);
	m_player->fall();
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
	m_player->buttFall();
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
