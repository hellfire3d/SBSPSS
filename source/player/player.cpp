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


// to be removed
#include "gfx\tpage.h"


/*	Std Lib
	------- */

/*	Data
	---- */

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

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
#include "player\pstates.h"
#include "player\psjump.h"
#include "player\psrun.h"
#include "player\psfall.h"
#include "player\psbutt.h"
#include "player\pschop.h"
#include "player\psduck.h"
CPlayerStateIdle			stateIdle;
CPlayerStateJump			stateJump;
CPlayerStateRun				stateRun;
CPlayerStateFall			stateFall;
CPlayerStateFallFar			stateFallFar;
CPlayerStateButtBounce		stateButtBounce;
CPlayerStateButtBounceFall	stateButtBounceFall;
CPlayerStateButtBounceLand	stateButtBounceLand;
CPlayerStateChop			stateChop;
CPlayerStateRunChop			stateRunChop;
CPlayerStateDuck			stateDuck;
CPlayerStateSoakUp			stateSoackUp;
CPlayerStateGetUp			stateGetup;


void	CPlayer::init()
{
	CThing::init();
	
	sActor3dHdr	*Hdr=m_skel.Load(ACTORS_SPONGEBOB_A3D);		
	m_skel.Init(Hdr);
	TPLoadTex(ACTORS_SPONGEBOB_TEX);
	m_skel.setAnimDatabase(CAnimDB::GetPlayerAnimBank());

m_animNo=0;
m_animFrame=0;
	setState(STATE_IDLE);
	m_moveVel.vx=0;
	m_moveVel.vy=0;
	setFacing(FACING_RIGHT);

#ifdef __USER_paul__
	Pos.vx=50;
	Pos.vy=200;
#else
	Pos.vx=10;
	Pos.vy=10;
#endif
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CPlayer::shutdown()
{
	CThing::shutdown();
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CPlayer::think(int _frames)
{
	CThing::think(_frames);

#ifndef __USER_paul__
	int	padInput=PadGetHeld(0);
	int	move=7*_frames;
	if(padInput&PAD_UP)		Pos.vy-=move;
	if(padInput&PAD_DOWN)	Pos.vy+=move;
	if(padInput&PAD_LEFT)	Pos.vx-=move;
	if(padInput&PAD_RIGHT)	Pos.vx+=move;
#else
	if(_frames>=3)_frames=2;

	for(int i=0;i<_frames;i++)
	{
		// Think
		m_currentState->think(this);
		Pos.vx+=m_moveVel.vx>>VELOCITY_SHIFT;
		Pos.vy+=m_moveVel.vy>>VELOCITY_SHIFT;

		// Ground collision
		if(m_moveVel.vy&&isOnSolidGround())
		{
			if(m_state==STATE_BUTTFALL)
			{
				setState(STATE_BUTTLAND);
			}
			else if(m_state==STATE_FALLFAR)
			{
				setState(STATE_IDLE);
			}
			else if(m_moveVel.vx)
			{
				setState(STATE_RUN);
			}
			else
			{
				setState(STATE_IDLE);
			}
			m_moveVel.vy=0;
		}
	}
#endif
	if(Pos.vx<0)Pos.vx=0;
	if(Pos.vy<0)Pos.vy=0;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int panim=-1;
void	CPlayer::render()
{
	CThing::render();
	
	// Render
	m_skel.setFrame(m_animFrame);
	if(panim!=-1)
		m_skel.setAnimNo(panim);
	else
		m_skel.setAnimNo(m_animNo);
	m_skel.Animate(this);
	m_skel.Render(this);
}






/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
PlayerMetrics s_normalPlayerMetrics=
{
	{
		8,		// PM__JUMP_VELOCITY
		10,		// PM__MAX_JUMP_FRAMES
		20,		// PM__MAX_SAFE_FALL_FRAMES
		4,		// PM__GRAVITY_VALUE
		8,		// PM__TERMINAL_VELOCITY
		8,		// PM__MAX_RUN_VELOCITY
		4,		// PM__RUN_SPEEDUP
		2,		// PM__RUN_REVERSESLOWDOWN
		1,		// PM__RUN_SLOWDOWN
	}
};
PlayerMetrics *CPlayer::getPlayerMetrics()
{
	return &s_normalPlayerMetrics;
}





/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
CPlayerState *CPlayer::s_states[NUM_STATES]=
{
	&stateIdle,				// STATE_IDLE
	&stateJump,				// STATE_JUMP
	&stateRun,				// STATE_RUN
	&stateFall,				// STATE_FALL
	&stateFallFar,			// STATE_FALLFAR
	&stateButtBounce,		// STATE_BUTTBOUNCE
	&stateButtBounceFall,	// STATE_BUTTFALL
	&stateButtBounceLand,	// STATE_BUTTLAND
	&stateChop,				// STATE_CHOP
	&stateRunChop,			// STATE_RUNCHOP
	&stateDuck,				// STATE_DUCK
	&stateSoackUp,			// STATE_SOAKUP
	&stateGetup,			// STATE_GETUP
};

void	CPlayer::setState(PLAYER_STATE _state)
{
	m_currentState=s_states[_state];
	m_currentState->enter(this);
	m_state=_state;
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
		switch(_facing)
		{
			case FACING_LEFT:
				m_facing=FACING_LEFT;
				m_skel.setAng(512);
				break;
			case FACING_RIGHT:
				m_facing=FACING_RIGHT;
				m_skel.setAng(3096+512);
				break;
			default:
				ASSERT(0);
				break;
		}
	}
}
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
	return m_skel.getFrameCount();
}
int CPlayer::getAnimNo()
{
	return m_animNo;
}
void CPlayer::setAnimNo(int _animNo)
{
	m_animNo=_animNo;
	m_animFrame=0;
}
DVECTOR CPlayer::getMoveVelocity()
{
	return m_moveVel;
}
void CPlayer::setMoveVelocity(DVECTOR *_moveVel)
{
	m_moveVel=*_moveVel;
}
int CPlayer::getPadInput()
{
	return PadGetHeld(0);
}



/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */







int CPlayer::isOnSolidGround()
{
	return Pos.vy>400;
}




void CPlayer::moveLeft()
{
	PlayerMetrics	*metrics;
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
}
void CPlayer::moveRight()
{
	PlayerMetrics	*metrics;
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
}
void CPlayer::slowdown()
{
	PlayerMetrics	*metrics;
	metrics=getPlayerMetrics();
	
	if(m_moveVel.vx<0)
	{
		m_moveVel.vx+=metrics->m_metric[PM__RUN_SLOWDOWN];
		if(m_moveVel.vx>0)m_moveVel.vx=0;
	}
	else if(m_moveVel.vx>0)
	{
		m_moveVel.vx-=metrics->m_metric[PM__RUN_SLOWDOWN];
		if(m_moveVel.vx<0)m_moveVel.vx=0;
	}
}

void CPlayer::jump()
{
}
void CPlayer::fall()
{
}


/*===========================================================================
end */
