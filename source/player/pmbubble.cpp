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

#ifndef __PLATFORM_PPLAYER_H__
#include "platform\pplayer.h"
#endif

#ifndef __GFX_FONT_H__
#include "gfx\font.h"
#endif


/*	Std Lib
	------- */

/*	Data
	---- */

#ifndef	__ANIM_SPONGEBOB_HEADER__
#include <ACTOR_SPONGEBOB_ANIM.h>
#endif

#ifndef __SPR_SPRITES_H__
#include <sprites.h>
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
	m_player->giveBubbleAmmo();
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
DVECTOR buboff={-40,-40};
int bubam;
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
		if(!m_blowing&&getPadInputDown()&PI_FIRE&&canBlowBubbleFromThisState()&&m_player->getBubbleAmmo())
		{
			// Spawn the bubbly platform thingy..!
			CNpcPlayerBubblePlatform	*bubble;
			DVECTOR			pos;
			bubble=new ("bubble platform") CNpcPlayerBubblePlatform;
			bubble->setType( CNpcPlatform::NPC_PLAYER_BUBBLE_PLATFORM );
			bubble->setGraphic( (u8) 0 );
			bubble->setTiltable( false );
			pos=m_player->getPos();
			pos.vx+=buboff.vx*m_player->getFacing();
			pos.vy+=buboff.vy;
			bubble->init(pos,4);

			// Start the blowing anim off
			m_blowFrame=0;
			m_blowing=true;

			// One less bubble..
			m_player->useOneBubble();
		}
	}

	// Blowing?
	if(m_blowing)
	{
		m_player->setAnimNo(ANIM_SPONGEBOB_BLOWBUBBLE);
		m_player->setAnimFrame(m_blowFrame);
		m_blowFrame++;
		if(m_blowFrame>=m_player->getAnimFrameCount())
		{
			m_player->setAnimNo(m_savedAnimNo);
			m_player->setAnimFrame(m_savedAnimFrame);
			m_blowing=false;
			m_bubbleDelay=BUBBLE_DELAY;
		}
	}
	bubam=m_player->getBubbleAmmo();
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
void	CPlayerModeBubbleMixture::renderModeUi()
{
	SpriteBank	*sb;
	sFrameHdr	*fh;
	char		buf[4];
	FontBank	*fb;

	sb=m_player->getSpriteBank();
	fh=sb->getFrameHeader(FRM__BUBBLEWAND);
	sb->printFT4(fh,CPlayer::POWERUPUI_ICONX,CPlayer::POWERUPUI_ICONY,0,0,CPlayer::POWERUPUI_OT);

	fb=m_player->getFontBank();
	sprintf(buf,"x%d",m_player->getBubbleAmmo());
	fb->print(CPlayer::POWERUPUI_TEXTX,CPlayer::POWERUPUI_TEXTY,buf);
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

		case STATE_SPRINGUP:
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