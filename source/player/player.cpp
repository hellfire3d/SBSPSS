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

#ifndef	__UTILS_HEADER__
#include "utils\utils.h"
#endif


// to be removed
//#include "fileio\fileio.h"
#include "gfx\tpage.h"
//#include "gfx\prim.h"


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

#define ANIM_IDLE_SHORT		15
#define ANIM_IDLE_LONG		16
#define ANIM_RUNSTART		29
#define ANIM_RUN			26
#define ANIM_RUNSTOP		30


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
void	CPlayer::init()
{
	CThing::init();
	
	sActor3dHdr	*Hdr=m_skel.Load(ACTORS_SPONGEBOB_A3D);		
	m_skel.Init(Hdr);
	TPLoadTex(ACTORS_SPONGEBOB_TEX);
	m_skel.setAnimDatabase(CAnimDB::GetPlayerAnimBank());

	setState(STATE_IDLE);
	m_runVel=0;
	setFacing(FACING_RIGHT);
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

	int	padInput;

if(_frames>=3)_frames=2;

	padInput=getPadInput();

	switch(m_state)
	{
		case STATE_IDLE:
			if(padInput&(PAD_LEFT|PAD_RIGHT))
			{
				if(padInput&PAD_LEFT)
				{
					setFacing(FACING_LEFT);
				}
				else
				{
					setFacing(FACING_RIGHT);
				}
				setState(STATE_RUNSTART);
				m_runVel=RUN_SPEEDUP;
			}
			break;

		case STATE_RUNSTART:
		case STATE_RUN:
			if(padInput&(PAD_LEFT|PAD_RIGHT))
			{
				if(m_facing==FACING_LEFT)
				{
					// Running left
					if(padInput&PAD_LEFT)
					{
						m_runVel+=RUN_SPEEDUP;
						if(m_runVel>MAX_RUN_VELOCITY)
						{
							m_runVel=MAX_RUN_VELOCITY;
						}
					}	
					else
					{
						m_runVel-=RUN_REVERSESLOWDOWN;
						if(m_runVel<1)
						{
							m_runVel=0;
							setFacing(FACING_RIGHT);
						}
					}
				}
				else
				{
					// Running right
					if(padInput&PAD_RIGHT)
					{
						m_runVel+=RUN_SPEEDUP;
						if(m_runVel>MAX_RUN_VELOCITY)
						{
							m_runVel=MAX_RUN_VELOCITY;
						}
					}	
					else
					{
						m_runVel-=RUN_REVERSESLOWDOWN;
						if(m_runVel<1)
						{
							m_runVel=0;
							setFacing(FACING_LEFT);
						}
					}
				}
			}
			else
			{
				m_runVel-=RUN_SLOWDOWN;
				if(m_runVel<1)
				{
					m_runVel=0;
					setState(STATE_RUNSTOP);
				}
			}
			break;

		case STATE_RUNSTOP:
			break;

		case NUM_STATES:
			break;
	}

	Pos.vx-=m_runVel*m_facing*_frames;
	if(Pos.vx<0)Pos.vx=0;


	// Animate
	m_frame+=_frames;
	if(m_frame>=m_skel.getFrameCount())
	{
		// PKG - Overlap..!?
		m_frame=0;
		finishedAnim();
	}











	if(padInput&PAD_UP)
	{
		Pos.vy-=5;
	}
	else if(padInput&PAD_DOWN)
	{
		Pos.vy+=5;
	}
	if(Pos.vy<0)Pos.vy=0;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CPlayer::render()
{
	CThing::render();
	
	// Render
	m_skel.setFrame(m_frame);
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
int	CPlayer::s_stateAnims[NUM_STATES]=
{
	ANIM_IDLE_SHORT,			// STATE_IDLE
	ANIM_RUNSTART,				// STATE_RUNSTART
	ANIM_RUN,					// STATE_RUN
	ANIM_RUNSTOP,				// STATE_RUNSTOP
};

int panim=-1;
void	CPlayer::setState(PLAYER_STATE _state)
{
	m_state=_state;
	m_animNo=s_stateAnims[_state];
if(panim!=-1)m_animNo=panim;
	m_frame=0;	
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayer::setFacing(int _facing)
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

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayer::finishedAnim()
{
	switch(m_state)
	{
		case STATE_IDLE:
			if(getRndRange(100)<95)
				m_animNo=ANIM_IDLE_SHORT;
			else
				m_animNo=ANIM_IDLE_LONG;
			break;
		case STATE_RUNSTART:
			setState(STATE_RUN);
			break;
		case STATE_RUN:
			break;
		case STATE_RUNSTOP:
			setState(STATE_IDLE);
			break;
		case NUM_STATES:
			break;
	}
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int CPlayer::getPadInput()
{
	return PadGetHeld(0);
}

/*===========================================================================
end */
