/*=========================================================================

	psidle.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/


/*----------------------------------------------------------------------
	Includes
	-------- */

#include "player\psidle.h"

#ifndef __PLAYER_PLAYER_H__
#include "player\player.h"
#endif

#ifndef	__UTILS_HEADER__
#include "utils\utils.h"
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


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayerStateBaseIdle::thinkControl(CPlayer *_player)
{
	int	controlDown,controlHeld;
	controlDown=getPadInputDown(_player);
	controlHeld=getPadInputHeld(_player);

	if(controlDown&PI_JUMP)
	{
		setState(_player,STATE_JUMP);
	}
	else if(controlHeld&PI_LEFT)
	{
		if(canMoveLeft(_player))
			setState(_player,STATE_RUN);
	}
	else if(controlHeld&PI_RIGHT)
	{
		if(canMoveRight(_player))
			setState(_player,STATE_RUN);
	}
	else if(controlDown&PI_ACTION)
	{
		setState(_player,STATE_ATTACK);
	}
	else if(controlHeld&PI_DOWN)
	{
		setState(_player,STATE_DUCK);
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayerStateTeeterIdle::enter(CPlayer *_player)
{
	int	edgeType,dir;
	int	anim;
	
	edgeType=isOnEdge(_player);
	dir=getFacing(_player);
	if(edgeType==FACING_LEFT)
	{
		anim=dir==FACING_LEFT?ANIM_SPONGEBOB_TEETERFRONT:ANIM_SPONGEBOB_TEETERBACK;
	}
	else
	{
		anim=dir==FACING_RIGHT?ANIM_SPONGEBOB_TEETERFRONT:ANIM_SPONGEBOB_TEETERBACK;
	}

	setAnimNo(_player,anim);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayerStateTeeterIdle::think(CPlayer *_player)
{
	advanceAnimFrameAndCheckForEndOfAnim(_player);
	thinkControl(_player);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayerStateIdle::enter(CPlayer *_player)
{
	m_idleTime=0;
	m_currentIdleAnim=0;
	m_animState=ANIMSTATE_END;

	setNextIdleAnim(_player);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayerStateIdle::think(CPlayer *_player)
{
	thinkControl(_player);
	
	if(advanceAnimFrameAndCheckForEndOfAnim(_player))
	{
		setNextIdleAnim(_player);
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayerStateIdle::setNextIdleAnim(CPlayer *_player)
{
	IdleAnims	*anims;
	int				finished=false;

	anims=getIdleAnimsDb(m_currentIdleAnim);
	switch(m_animState)
	{
		case ANIMSTATE_START:
			m_animState=ANIMSTATE_LOOP;
			setAnimNo(_player,anims->m_loopFrame);
			break;		
		case ANIMSTATE_LOOP:
			if(--m_loopCount<=0)
			{
				if(anims->m_endFrame==-1)
				{
					finished=true;
				}
				else
				{
					m_animState=ANIMSTATE_END;
					setAnimNo(_player,anims->m_endFrame);
				}
			}
			else
			{
				setAnimNo(_player,anims->m_loopFrame);
			}
			break;		
		case ANIMSTATE_END:
			finished=true;
			break;		
	}

	if(finished)
	{
		int	animNo;
		if(m_idleTime<5)
		{
			// Run the default idle anim a number of times before
			// starting to do weird things..
			m_currentIdleAnim=0;
		}
		else
		{
			// Alternate random idle anims with the default one
			if(m_currentIdleAnim==0&&getNumIdleAnims()>1)
			{
				m_currentIdleAnim=getRndRange(getNumIdleAnims());
			}
			else
			{
				m_currentIdleAnim=0;
			}
		}

		// Start playing the anim
		anims=getIdleAnimsDb(m_currentIdleAnim);
		if(anims->m_startFrame==-1)
		{
			// No start anim - go straight into loop
			animNo=anims->m_loopFrame;
			m_animState=ANIMSTATE_LOOP;
		}
		else
		{
			// Play start anim for this idle
			animNo=anims->m_startFrame;
			m_animState=ANIMSTATE_START;
		}
		m_loopCount=anims->m_loopCount;
		setAnimNo(_player,animNo);

		m_idleTime++;
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
static IdleAnims s_unarmedIdleAnims[]=
{
	//	start frame						loop frame						end frame						loop count
	{	-1,								ANIM_SPONGEBOB_IDLEBREATHE,	-1,								4	},	// default
	{	-1,								ANIM_SPONGEBOB_IDLEBREATHE,	-1,								10	},
	{	ANIM_SPONGEBOB_FACEFRONT,		ANIM_SPONGEBOB_IDLEHOOLA,		ANIM_SPONGEBOB_FACEBACK,		5	},
	{	ANIM_SPONGEBOB_FACEFRONT,		ANIM_SPONGEBOB_IDLEWIGGLEARM,	ANIM_SPONGEBOB_FACEBACK,		5	},
	{	-1,								ANIM_SPONGEBOB_IDLELOOK,		-1,								1	},
	{	-1,								ANIM_SPONGEBOB_IDLEWIND,		-1,								1	},
};
//idlekick
//idlescratch
//? - idlesleep* - wakeup
//idlestretch
//idlewind
static int s_numUnarmedIdleAnims=sizeof(s_unarmedIdleAnims)/sizeof(IdleAnims);
IdleAnims *CPlayerStateUnarmedIdle::getIdleAnimsDb(int _animNo)
{
	ASSERT(_animNo<getNumIdleAnims());
	return &s_unarmedIdleAnims[_animNo];
}
int CPlayerStateUnarmedIdle::getNumIdleAnims()
{
	return s_numUnarmedIdleAnims;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
static IdleAnims s_coralBlowerIdleAnims[]=
{
	//	start frame						loop frame						end frame						loop count
	{	-1,								ANIM_SPONGEBOB_IDLEBREATHE,	-1,								4	}, // default
	{	-1,								ANIM_SPONGEBOB_IDLEBREATHE,	-1,								10	},
	{	-1,								ANIM_SPONGEBOB_IDLELOOK,		-1,								1	},
};
static int s_numCoralBlowerIdleAnims=sizeof(s_coralBlowerIdleAnims)/sizeof(IdleAnims);
IdleAnims *CPlayerStateCoralBlowerIdle::getIdleAnimsDb(int _animNo)
{
	ASSERT(_animNo<getNumIdleAnims());
	return &s_coralBlowerIdleAnims[_animNo];
}
int CPlayerStateCoralBlowerIdle::getNumIdleAnims()
{
	return s_numCoralBlowerIdleAnims;
}


/*===========================================================================
 end */
