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

#ifndef	__ANIM_PLAYER_ANIM_HEADER__
#include <player_anim.h>
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

int s_health;

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CPlayer::init()
{
	CThing::init();

	m_layerCollision=NULL;
	
	sActor3dHdr	*Hdr=m_skel.Load(ACTORS_SPONGEBOB_A3D);		
	m_skel.Init(Hdr);
	TPLoadTex(ACTORS_SPONGEBOB_TEX);
	m_skel.setAnimDatabase(CAnimDB::GetPlayerAnimBank());

m_animNo=0;
m_animFrame=0;
	m_currentMode=PLAYER_MODE_BASICUNARMED;
	setState(STATE_IDLE);
	m_moveVel.vx=0;
	m_moveVel.vy=0;
	setFacing(FACING_RIGHT);

	m_lives=CGameSlotManager::getSlotData().m_lives;

	m_invincibleFrameCount=INVIBCIBLE_FRAMES__START;

#ifdef __USER_paul__
	Pos.vx=23*16;
	Pos.vy=10*16;
#else
	Pos.vx=10;
	Pos.vy=10;
#endif

	m_cameraOffsetTarget.vx=0;
	m_cameraOffsetTarget.vy=0;
	m_cameraOffset.vx=0;
	m_cameraOffset.vy=0;
	m_cameraLookYOffset=0;
	m_cameraLookTimer=0;

	m_lastPadInput=m_padInput=0;

	s_health=5;
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
#ifdef __USER_paul__
DVECTOR ofs={-248,-136};		// nearly -256,-128 ;)
int newmode=-1;
#else
DVECTOR ofs={0,0}; //temporary
#endif
void	CPlayer::think(int _frames)
{
	int	i;
	
	CThing::think(_frames);


#ifdef __USER_paul__
if(PadGetHeld(0)&PAD_L1&&PadGetHeld(0)&PAD_L2)
{
	Pos.vx=23*16;
	Pos.vy=10*16;
}
if(newmode!=-1)
{
	setMode((PLAYER_MODE)newmode);
	newmode=-1;
}
#endif

#ifndef __USER_paul__
	int	padInput=PadGetHeld(0);
	int	move=7*_frames;
	if(padInput&PAD_UP)		Pos.vy-=move;
	if(padInput&PAD_DOWN)	Pos.vy+=move;
	if(padInput&PAD_LEFT)	Pos.vx-=move;
	if(padInput&PAD_RIGHT)	Pos.vx+=move;
	m_invincibleFrameCount=0;

	if ( padInput & CPadConfig::getButton(CPadConfig::PAD_CFG_UP) ) // not sure where you want to put this, Paul (Charles)
	{
		GameScene.sendEvent( USER_REQUEST_TALK_EVENT, this );
	}
#else
	if(_frames>=3)_frames=2;

	for(i=0;i<_frames;i++)
	{
		// Think
		updatePadInput();
		m_currentStateClass->think(this);


		// Horizontal movement
		if(m_layerCollision->Get((Pos.vx+(m_moveVel.vx>>VELOCITY_SHIFT))>>4,(Pos.vy-1)>>4))
		{
			// Move flush with the edge of the obstruction
			int	dir,vx,cx,y,i;
			if(m_moveVel.vx<0)
			{
				dir=-1;
				vx=-m_moveVel.vx;
			}
			else
			{
				dir=+1;
				vx=m_moveVel.vx;
			}
			cx=Pos.vx;
			y=(Pos.vy-1)>>4;
			for(i=0;i<vx;i++)
			{
				if(m_layerCollision->Get(cx>>4,y))
				{
					break;
				}
				cx+=dir;
			}
			Pos.vx=cx-dir;

			// If running then idle, otherwise leave in same state
			if(m_currentState==STATE_RUN)
			{
				setState(STATE_IDLE);
			}
			m_moveVel.vx=0;
		}
		else
		{
			Pos.vx+=m_moveVel.vx>>VELOCITY_SHIFT;
		}
		if(m_currentState==STATE_IDLE&&isOnEdge())
		{
			setState(STATE_IDLETEETER);
		}


		// Vertical movement
		Pos.vy+=m_moveVel.vy>>VELOCITY_SHIFT;
		if(isOnSolidGround())
		{
//stick to ground (PKG)
//Pos.vy=23*16+1;//16*15;
int colHeight=16;
Pos.vy=((Pos.vy-16)&0xfffffff0)+colHeight;

			if(m_moveVel.vy)
			{
				// Was falling.. so we've just hit the ground
				if(m_currentState==STATE_BUTTFALL)
				{
					// landed from a btt bounce
					setState(STATE_BUTTLAND);
				}
				else if(m_currentState==STATE_FALLFAR)
				{
					// Landed from a painful long fall
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
					// Landed from a standing jump
					setState(STATE_IDLE);
					setAnimNo(ANIM_PLAYER_ANIM_JUMPEND);
				}
				m_moveVel.vy=0;
				m_fallFrames=0;
			}
		}
		else
		{
			if(m_currentState!=STATE_FALL&&m_currentState!=STATE_FALLFAR&&
			   m_currentState!=STATE_BUTTFALL&&m_currentState!=STATE_BUTTBOUNCE&&
			   m_currentState!=STATE_JUMP)
			{
				setState(STATE_FALL);
			}
		}

		// Flashing..
		if(m_invincibleFrameCount)
		{
			m_invincibleFrameCount--;
		}

		// Look around
		int	pad=getPadInputHeld();
if(getPadInputDown()&PAD_CIRCLE)
{
	m_skel.blink();
}
/*
		if(pad&CPadConfig::getButton(CPadConfig::PAD_CFG_UP))
		{
			if(m_cameraLookTimer<=-LOOKAROUND_DELAY)
			{
				m_cameraLookYOffset-=LOOKAROUND_SCROLLSPEED;
				if(m_cameraLookYOffset<-LOOKAROUND_MAXSCROLL)
				{
					m_cameraLookYOffset=-LOOKAROUND_MAXSCROLL;
				}
			}
			else
			{
				m_cameraLookTimer--;
			}
		}
		else if(pad&CPadConfig::getButton(CPadConfig::PAD_CFG_DOWN))
		{
			if(m_cameraLookTimer>=LOOKAROUND_DELAY)
			{
				m_cameraLookYOffset+=LOOKAROUND_SCROLLSPEED;
				if(m_cameraLookYOffset>LOOKAROUND_MAXSCROLL)
				{
					m_cameraLookYOffset=LOOKAROUND_MAXSCROLL;
				}
			}
			else
			{
				m_cameraLookTimer++;
			}
		}
		else
		{
			m_cameraLookTimer=0;
			if(m_cameraLookYOffset<0)
			{
				m_cameraLookYOffset+=LOOKAROUND_RESETSPEED;
				if(m_cameraLookYOffset>0)
				{
					m_cameraLookYOffset=0;
				}
			}
			else if(m_cameraLookYOffset>0)
			{
				m_cameraLookYOffset-=LOOKAROUND_RESETSPEED;
				if(m_cameraLookYOffset<0)
				{
					m_cameraLookYOffset=0;
				}
			}
		}
*/
	}


#endif
	// Move the camera offset
m_cameraOffsetTarget=ofs;		
m_cameraOffset=ofs;
/*
	for(i=0;i<_frames;i++)
	{
		int moveDelta;
		moveDelta=(m_cameraOffset.vx-m_cameraOffsetTarget.vx);
		if(moveDelta)
		{
		if(moveDelta<0)
		{
					moveDelta>>=2;
					if(moveDelta==0)moveDelta=1;
					}
					else if(moveDelta>0)
					{
					moveDelta>>=2;
					if(moveDelta==0)moveDelta=-1;
					}
					m_cameraOffset.vx+=moveDelta;
					}
	}
*/
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
DVECTOR ppos={0,500};
#ifdef __USER_paul__
int mouth=-1,eyes=-1;
#endif
void	CPlayer::render()
{
	CThing::render();
	
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
		m_skel.setPos(ppos);
		if(panim!=-1)
			m_skel.setAnimNo(panim);
		else
			m_skel.setAnimNo(m_animNo);
		m_skel.setFrame(m_animFrame);
		m_skel.Animate(this);
		m_skel.Render(this);
	}
}



/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
DVECTOR CPlayer::getCameraPos()
{
	DVECTOR	cameraPos;
	cameraPos.vx=Pos.vx+m_cameraOffset.vx;
	cameraPos.vy=Pos.vy+m_cameraOffset.vy;//+m_cameraLookYOffset;
	return cameraPos;
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
	setState(m_currentState);
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
				CSoundMediator::playSfx(frameSfx->m_sfxId);
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


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int CPlayer::getPadInputHeld()
{
	return m_padInput;
}
int CPlayer::getPadInputDown()
{
	return m_padInputDown;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int CPlayer::isOnSolidGround()
{
	ASSERT(m_layerCollision);
	return m_layerCollision->Get(Pos.vx>>4,Pos.vy>>4);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:	FACING_LEFT if left half of player is hanging, FACING_RIGHT
				if right half of player is hanging or 0 if no part of the
				player is hanging
  ---------------------------------------------------------------------- */
int csize=20;
int CPlayer::isOnEdge()
{
	int	ret=0;

	ASSERT(m_layerCollision);
	if(!m_layerCollision->Get((Pos.vx-csize)>>4,Pos.vy>>4))
	{
		ret=FACING_LEFT;
	}
	else if(!m_layerCollision->Get((Pos.vx+csize)>>4,Pos.vy>>4))
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
	ASSERT(m_layerCollision);
	return m_layerCollision->Get((Pos.vx-1)>>4,(Pos.vy-1)>>4)==0;
}
int CPlayer::canMoveRight()
{
	ASSERT(m_layerCollision);
	return m_layerCollision->Get((Pos.vx+1)>>4,(Pos.vy-1)>>4)==0;
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
}
void CPlayer::slowdown()
{
	const PlayerMetrics	*metrics;
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
#ifdef __VERSION_DEBUG__
int invincibleSponge=false;		// NB: This is for debugging purposes only
#endif
void CPlayer::takeDamage(DAMAGE_TYPE _damage)
{
	// Don't take damage if still recovering from the last hit
	if(!m_invincibleFrameCount)
	{
		int	ouchThatHurt=true;

		// Check if we are currently immune to this damage type
		switch(_damage)
		{
			case DAMAGE__FALL:
			case DAMAGE__LAVA:
				break;

			case DAMAGE__ELECTROCUTION:
				// if squeaky boots then ouchThatHurt=false;
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
	m_padInputDown=m_padInput&(m_lastPadInput^-1);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int CPlayer::readPadInput()
{
	return PadGetHeld(0);
}


/*===========================================================================
end */
