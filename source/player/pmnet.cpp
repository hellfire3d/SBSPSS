/*=========================================================================

	pmnet.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

/*----------------------------------------------------------------------
	Includes
	-------- */

#include "player\pmnet.h"

#ifndef __GFX_SPRBANK_H__
#include "gfx\sprbank.h"
#endif


/*	Std Lib
	------- */

/*	Data
	---- */

#ifndef	__ANIM_SPONGEBOB_HEADER__
#include <ACTOR_SPONGEBOB_ANIM.h>
#endif

#ifndef __SPR_INGAMEFX_H__
#include <ingamefx.h>
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
void	CPlayerModeNet::enter()
{
	m_netting=false;
	m_netState=NET_STATE__EMPTY;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int netstate;
void	CPlayerModeNet::think()
{
	// If we're netting then restore the 'real' anim number/frame before
	// doing the think so that the rest of the code doesn't know what
	// is going on ;)
	if(m_netting)
	{
		setAnimNo(m_savedAnimNo);
		setAnimFrame(m_savedAnimFrame);
	}
	CPlayerModeBase::think();

	// Start to net?
	if(!m_netting&&getPadInputDown()&PI_ACTION&&canSwingNetFromThisState())
	{
		m_netFrame=0;
		m_netting=true;
		switch(m_netState)
		{
			case NET_STATE__EMPTY:
				m_netState=NET_STATE__FULL;
				break;
			case NET_STATE__FULL:
				m_netState=NET_STATE__EMPTY;
				break;
		}
	}

	// Netting?
	if(m_netting)
	{
//!!!		m_player->setAnimNo(ANIM_SPONGEBOB_KARATE);
		m_player->setAnimNo(ANIM_SPONGEBOB_FIRE);
		m_player->setAnimFrame(m_netFrame);
		m_netFrame++;
		if(m_netFrame>=m_player->getAnimFrameCount())
		{
			m_player->setAnimNo(m_savedAnimNo);
			m_player->setAnimFrame(m_savedAnimFrame);
			m_netting=false;
		}
	}
	netstate=m_netState;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CPlayerModeNet::renderModeUi()
{
	SpriteBank	*sb;
	sFrameHdr	*fh;

	sb=m_player->getSpriteBank();
	fh=sb->getFrameHeader(FRM__NET);
	if(m_netState==NET_STATE__FULL)
	{
		// Net has a jellyfish inside
		sb->printFT4Scaled(fh,CPlayer::POWERUPUI_ICONX-(fh->W/2),CPlayer::POWERUPUI_ICONY-(fh->H/2),0,0,CPlayer::POWERUPUI_OT,256+128);
	}
	else
	{
		sb->printFT4(fh,CPlayer::POWERUPUI_ICONX-(fh->W/2),CPlayer::POWERUPUI_ICONY-(fh->H/2),0,0,CPlayer::POWERUPUI_OT);
	}
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CPlayerModeNet::setAnimNo(int _animNo)
{
	CPlayerModeBase::setAnimNo(_animNo);
	m_savedAnimNo=_animNo;
}

void	CPlayerModeNet::setAnimFrame(int _animFrame)
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
int		CPlayerModeNet::canSwingNetFromThisState()
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
