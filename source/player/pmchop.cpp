/*=========================================================================

	pmchop.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

/*----------------------------------------------------------------------
	Includes
	-------- */

#include "player\pmchop.h"

#ifndef __GFX_SPRBANK_H__
#include "gfx\sprbank.h"
#endif

#ifndef __GAME_GAME_H__
#include "game\game.h"
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
void	CPlayerModeChop::enter()
{
	CPlayerModeBase::enter();
	m_chopping=false;
	m_timer=0;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int chopcsx=-40;
int chopcsy=-30;
int chopcsw=110;
int chopcsh=60;
void	CPlayerModeChop::think()
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
	if(!m_chopping&&getPadInputDown()&PI_FIRE&&canAttackFromThisState())
	{
		m_chopFrame=0;
		getPlayerCollisionSize(&m_savedCSX,&m_savedCSY,&m_savedCSW,&m_savedCSH);
		m_chopping=true;
	}

	// Chopping?
	if(m_chopping)
	{
		int facing;
		facing=getFacing();
		if(m_chopFrame==CHOP_ATTACK_START_FRAME)
		{
			m_chopping=false;	// Oops..
			setPlayerCollisionSize(chopcsx*facing,chopcsy,chopcsw,chopcsh);
			m_chopping=true;	// Oops..
		}
		else if(m_chopFrame==CHOP_ATTACK_END_FRAME+1)
		{
			m_chopping=false;	// Oops..
			setPlayerCollisionSize(m_savedCSX,m_savedCSY,m_savedCSW,m_savedCSH);
			m_chopping=true;	// Oops..
		}

		m_player->setAnimNo(ANIM_SPONGEBOB_KARATE);
		m_player->setAnimFrame(m_chopFrame);
		m_chopFrame++;
		if(m_chopFrame>=m_player->getAnimFrameCount())
		{
			m_player->setAnimNo(m_savedAnimNo);
			m_player->setAnimFrame(m_savedAnimFrame);
			m_chopping=false;
			setPlayerCollisionSize(m_savedCSX,m_savedCSY,m_savedCSW,m_savedCSH);
		}
	}

	if(++m_timer>CHOP_TIMEOUT)
	{
		m_player->setMode(PLAYER_MODE_BASICUNARMED);
	}
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CPlayerModeChop::renderModeUi()
{
	if(m_timer<CHOP_TIMEOUT-CHOP_FLASH_TIME||m_timer&2)
	{
		SpriteBank	*sb;
		sFrameHdr	*fh;

		sb=CGameScene::getSpriteBank();
		fh=sb->getFrameHeader(FRM__GLOVE);

		sb->printFT4(fh,CPlayer::POWERUPUI_ICONX,CPlayer::POWERUPUI_ICONY,0,0,CPlayer::POWERUPUI_OT);
	}
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CPlayerModeChop::setAnimNo(int _animNo)
{
	CPlayerModeBase::setAnimNo(_animNo);
	m_savedAnimNo=_animNo;
}

void	CPlayerModeChop::setAnimFrame(int _animFrame)
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
ATTACK_STATE	CPlayerModeChop::getAttackState()
{
	if(m_chopping&&m_chopFrame>=CHOP_ATTACK_START_FRAME&&m_chopFrame<=CHOP_ATTACK_END_FRAME)
	{
		return ATTACK_STATE__KARATE;
	}
	else
	{
		return CPlayerModeBase::getAttackState();
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CPlayerModeChop::setPlayerCollisionSize(int _x,int _y,int _w,int _h)
{
	if(m_chopping)
	{
		m_savedCSX=_x;
		m_savedCSY=_y;
		m_savedCSW=_w;
		m_savedCSY=_h;
	}
	else
	{
		CPlayerModeBase::setPlayerCollisionSize(_x,_y,_w,_h);
	}
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int	CPlayerModeChop::canAttackFromThisState()
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

/*===========================================================================
end */
