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

#ifndef __PLAYER_PMODES_H__
#include "player\pmodes.h"
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

CPlayerStateUnarmedIdle		s_stateUnarmedIdle;
CPlayerStateTeeterIdle		s_stateTeeterIdle;
CPlayerStateNoAnimIdle		s_stateNoAnimIdle;
CPlayerStateWeaponIdle		s_stateWeaponIdle;
CPlayerStateNetIdle			s_stateNetIdle;


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayerStateBaseIdle::thinkControl(CPlayerModeBase *_playerMode)
{
	int	controlDown,controlHeld;
	controlDown=_playerMode->getPadInputDown();
	controlHeld=_playerMode->getPadInputHeld();

	_playerMode->slowdown();

	if(controlDown&PI_JUMP)
	{
		_playerMode->setState(STATE_JUMP);
	}
	else if(controlHeld&PI_LEFT)
	{
		CPlayer *player = _playerMode->getPlayer();
		player->setMoveLeftRight( player->getMoveLeftRight() - 1 );

		if(_playerMode->canMoveLeft())
			_playerMode->setState(STATE_RUN);
	}
	else if(controlHeld&PI_RIGHT)
	{
		CPlayer *player = _playerMode->getPlayer();
		player->setMoveLeftRight( player->getMoveLeftRight() + 1 );

		if(_playerMode->canMoveRight())
			_playerMode->setState(STATE_RUN);
	}
	else if(controlHeld&PI_DOWN&&_playerMode->canDoLookAround())
	{
		_playerMode->setState(STATE_LOOKDOWN);
	}
	else if(controlHeld&PI_UP&&_playerMode->canDoLookAround())
	{
		_playerMode->setState(STATE_LOOKUP);
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayerStateTeeterIdle::enter(CPlayerModeBase *_playerMode)
{
	int	edgeType,dir;
	int	anim;
	
	edgeType=_playerMode->isOnEdge();
	dir=_playerMode->getFacing();
	if(edgeType==FACING_LEFT)
	{
		anim=dir==FACING_LEFT?ANIM_SPONGEBOB_TEETERFRONT:ANIM_SPONGEBOB_TEETERBACK;
	}
	else
	{
		anim=dir==FACING_RIGHT?ANIM_SPONGEBOB_TEETERFRONT:ANIM_SPONGEBOB_TEETERBACK;
	}

	_playerMode->setAnimNo(anim);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayerStateTeeterIdle::think(CPlayerModeBase *_playerMode)
{
	_playerMode->advanceAnimFrameAndCheckForEndOfAnim();
	thinkControl(_playerMode);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayerStateNoAnimIdle::enter(CPlayerModeBase *_playerMode)
{
	_playerMode->setAnimNo(ANIM_SPONGEBOB_IDLEBREATH);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayerStateNoAnimIdle::think(CPlayerModeBase *_playerMode)
{
	if(_playerMode->advanceAnimFrameAndCheckForEndOfAnim())
	{
		_playerMode->setAnimNo(ANIM_SPONGEBOB_IDLEBREATH);
	}
	thinkControl(_playerMode);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayerStateIdle::enter(CPlayerModeBase *_playerMode)
{
	m_idleTime=0;
	m_currentIdleAnim=0;
	m_animState=ANIMSTATE_END;

	setNextIdleAnim(_playerMode);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayerStateIdle::think(CPlayerModeBase *_playerMode)
{
	if(_playerMode->advanceAnimFrameAndCheckForEndOfAnim())
	{
		setNextIdleAnim(_playerMode);
	}
	thinkControl(_playerMode);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayerStateIdle::setNextIdleAnim(CPlayerModeBase *_playerMode)
{
	IdleAnims	*anims;
	int				finished=false;

	anims=getIdleAnimsDb(m_currentIdleAnim);
	switch(m_animState)
	{
		case ANIMSTATE_START:
			m_animState=ANIMSTATE_LOOP;
			_playerMode->setAnimNo(anims->m_loopFrame);
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
					_playerMode->setAnimNo(anims->m_endFrame);
				}
			}
			else
			{
				_playerMode->setAnimNo(anims->m_loopFrame);
			}
			break;		
		case ANIMSTATE_END:
			finished=true;
			break;		
	}

	if(finished)
	{
		int	animNo;
		if(m_idleTime<2)
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
				m_currentIdleAnim=getRndRange(getNumIdleAnims()-1)+1;
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
		_playerMode->setAnimNo(animNo);

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
	//	start frame						loop frame							end frame						loop count
	{	-1,								ANIM_SPONGEBOB_IDLEBREATH,			ANIM_SPONGEBOB_IDLEBLINK,		4*60	},	// default
	{	-1,								ANIM_SPONGEBOB_IDLEBREATH,			ANIM_SPONGEBOB_IDLEBLINK,		5*60	},
	{	-1,								ANIM_SPONGEBOB_IDLEBREATH,			ANIM_SPONGEBOB_IDLEBLINK,		2*60	},
	{	-1,								ANIM_SPONGEBOB_IDLEFACEUPSIDEDOWN,	-1,								1		},
	{	-1,								ANIM_SPONGEBOB_IDLEBANDAID,			-1,								1		},
	{	-1,								ANIM_SPONGEBOB_IDLEMATURE,			-1,								1		},
	{	-1,								ANIM_SPONGEBOB_IDLEMOUSTACHE,		-1,								1		},
	{	-1,								ANIM_SPONGEBOB_IDLENOFACE,			-1,								1		},
	{	-1,								ANIM_SPONGEBOB_IDLEZORRO,			-1,								1		},
};
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
static IdleAnims s_weaponIdleAnims[]=
{
	//	start frame						loop frame							end frame						loop count
	{	-1,								ANIM_SPONGEBOB_IDLEBREATH,			ANIM_SPONGEBOB_IDLEBLINK,		4*60	}, // default
	{	-1,								ANIM_SPONGEBOB_IDLEBREATH,			ANIM_SPONGEBOB_IDLEBLINK,		5*60	},
	{	-1,								ANIM_SPONGEBOB_IDLEBREATH,			ANIM_SPONGEBOB_IDLEBLINK,		2*60	},
	{	-1,								ANIM_SPONGEBOB_IDLEWEAPON,			-1,								1		},
};
static int s_numWeaponIdleAnims=sizeof(s_weaponIdleAnims)/sizeof(IdleAnims);
IdleAnims *CPlayerStateWeaponIdle::getIdleAnimsDb(int _animNo)
{
	ASSERT(_animNo<getNumIdleAnims());
	return &s_weaponIdleAnims[_animNo];
}
int CPlayerStateWeaponIdle::getNumIdleAnims()
{
	return s_numWeaponIdleAnims;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
static IdleAnims s_netIdleAnims[]=
{
	//	start frame						loop frame							end frame						loop count
	{	-1,								ANIM_SPONGEBOB_IDLEBREATH,			ANIM_SPONGEBOB_IDLEBLINK,		4*60	}, // default
	{	-1,								ANIM_SPONGEBOB_IDLEBREATH,			ANIM_SPONGEBOB_IDLEBLINK,		5*60	},
	{	-1,								ANIM_SPONGEBOB_IDLEBREATH,			ANIM_SPONGEBOB_IDLEBLINK,		2*60	},
	{	-1,								ANIM_SPONGEBOB_IDLEWEAPON,			-1,								1		},
};
static int s_numNetIdleAnims=sizeof(s_netIdleAnims)/sizeof(IdleAnims);
IdleAnims *CPlayerStateNetIdle::getIdleAnimsDb(int _animNo)
{
	ASSERT(_animNo<getNumIdleAnims());
	return &s_netIdleAnims[_animNo];
}
int CPlayerStateNetIdle::getNumIdleAnims()
{
	return s_numNetIdleAnims;
}


/*===========================================================================
 end */
