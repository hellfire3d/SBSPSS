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
	CSoundMediator::playSfx(CSoundMediator::SFX_BUBBLE_WAND);
	m_blowing=false;
	m_bubbleDelay=0;
	m_bubbleCount=BUBBLE_AMMO;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CPlayerModeBubbleMixture::think()
{
	// If we're blowing then restore the 'real' anim number/frame before
	// doing the think so that the rest of the code doesn't know what
	// is going on ;)
	if(m_blowing)
	{
		setAnimNo(m_savedAnimNo);
		setAnimFrame(m_savedAnimFrame);
	}
	CPlayerModeBase::think();

	if(m_bubbleDelay)
	{
		// Delay so that you can't blow all of your bubbles really quickly
		m_bubbleDelay--;
	}
	else
	{
		// Start to blow?
		if(!m_blowing&&getPadInputDown()&PI_ACTION&&canBlowBubbleFromThisState())
		{
			// Spawn the bubbly platform thingy..!
			//	pos is m_player->getPos();
			//	dir is m_player->getFacing();

			// Start the anim off
			m_blowFrame=0;
			m_blowing=true;
		}
	}

	// Blowing?
	if(m_blowing)
	{
		m_player->setAnimNo(ANIM_SPONGEBOB_KARATE);
		m_player->setAnimFrame(m_blowFrame);
		m_blowFrame++;
		if(m_blowFrame>=m_player->getAnimFrameCount())
		{
			m_player->setAnimNo(m_savedAnimNo);
			m_player->setAnimFrame(m_savedAnimFrame);
			m_blowing=false;
			m_bubbleDelay=BUBBLE_DELAY;
			if(--m_bubbleCount==0)
			{
				m_player->setMode(PLAYER_MODE_FULLUNARMED);
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
int		CPlayerModeBubbleMixture::canBlowBubbleFromThisState()
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
