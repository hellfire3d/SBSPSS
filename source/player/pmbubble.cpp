/*=========================================================================

	pmbubble.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

/*----------------------------------------------------------------------
	Includes
	-------- */

#include "player\pmbubble.h"


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
void	CPlayerModeBubbleMixture::enter()
{
	m_chopping=false;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CPlayerModeBubbleMixture::think()
{
	// If we're chopping then restore the 'real' anim number/frame before
	// doing the think so that the rest of the code doesn't know what
	// is going on ;)
	if(m_chopping)
	{
		setAnimNo(m_savedAnimNo);
		setAnimFrame(m_savedAnimFrame);
	}
	CPlayerModeBase::think();

	// Start to chop?
	if(!m_chopping&&getPadInputDown()&PI_ACTION&&canAttackFromThisState())
	{
		m_chopFrame=0;
		m_chopping=true;
	}

	// Chopping?
	if(m_chopping)
	{
		m_player->setAnimNo(ANIM_SPONGEBOB_KARATE);
		m_player->setAnimFrame(m_chopFrame);
		m_chopFrame++;
		if(m_chopFrame>=m_player->getAnimFrameCount())
		{
			m_player->setAnimNo(m_savedAnimNo);
			m_player->setAnimFrame(m_savedAnimFrame);
			m_chopping=false;
		}
	}
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CPlayerModeBubbleMixture::setAnimNo(int _animNo)
{
	CPlayerModeBase::setAnimNo(_animNo);
	m_savedAnimNo=_animNo;
}

void	CPlayerModeBubbleMixture::setAnimFrame(int _animFrame)
{
	CPlayerModeBase::setAnimFrame(_animFrame);
	m_savedAnimFrame=_animFrame;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int		CPlayerModeBubbleMixture::isInAttackState()
{
	return m_chopping||CPlayerModeBase::isInAttackState();
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int		CPlayerModeBubbleMixture::canAttackFromThisState()
{
	int	ret=false;

	switch(getState())
	{
		case STATE_IDLE:
		case STATE_IDLETEETER:
		case STATE_JUMP:
		case STATE_RUN:
		case STATE_FALL:
			ret=true;
			break;

		case STATE_FALLFAR:
		case STATE_BUTTBOUNCE:
		case STATE_BUTTFALL:
		case STATE_BUTTLAND:
		case STATE_DUCK:
		case STATE_SOAKUP:
		case STATE_GETUP:
			break;
	}

	return ret;
}

/*===========================================================================
end */
