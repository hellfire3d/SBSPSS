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

#ifndef __PLATFORM_PLATFORM_H__
#include "platform\platform.h"
#endif

#ifndef __GFX_FONT_H__
#include "gfx\font.h"
#endif

#ifndef __GAME_GAME_H__
#include "game/game.h"
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

#ifndef __PLAYER_PSFLOAT_H__
#include "player\psfloat.h"
#endif

#ifndef __PLAYER_PSDANCE_H__
#include "player\psdance.h"
#endif

#ifndef __PLAYER_PSSWAL_H__
#include "player\psswal.h"
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

static	CPlayerState	*s_stateTable[]=
{
	&s_stateWeaponIdle,						// STATE_IDLE
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


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CPlayerModeBubbleMixture::enter()
{
	CPlayerModeBase::enter();
	m_blowing=false;
	m_bubbleDelay=0;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
DVECTOR buboff={-80,-10};
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
			CNpcPlatform	*bubble;
			DVECTOR			pos;
			bubble = CNpcPlatform::Create( CNpcPlatform::NPC_PLAYER_BUBBLE_PLATFORM );
			pos=m_player->getPos();
			pos.vx+=buboff.vx*m_player->getFacing();
			pos.vy+=buboff.vy;
			bubble->setGraphic( (u8) 0 );
			bubble->init(pos,4);
			bubble->postInit();

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

	sb=CGameScene::getSpriteBank();
	fh=sb->getFrameHeader(FRM__SMALL_BUBBLEWAND);
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
		case STATE_SPRINGUP:
			ret=true;
			break;

		case STATE_FALLFAR:
		case STATE_BUTTBOUNCE:
		case STATE_BUTTFALL:
		case STATE_BUTTLAND:
		case STATE_BUTTBOUNCEUP:
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
CPlayerState	**CPlayerModeBubbleMixture::getStateTable()
{
	return s_stateTable;
}

/*===========================================================================
end */