/*=========================================================================

	pmbloon.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

/*----------------------------------------------------------------------
	Includes
	-------- */

#include "player\pmbloon.h"

#ifndef __GFX_SPRBANK_H__
#include "gfx\sprbank.h"
#endif

#ifndef __LEVEL_LEVEL_H__
#include "level\level.h"
#endif


/*	Std Lib
	------- */

/*	Data
	---- */

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

static PlayerMetrics	s_playerMetrics=
{	{
	DEFAULT_PLAYER_JUMP_VELOCITY,			// PM__JUMP_VELOCITY
	DEFAULT_PLAYER_MAX_JUMP_FRAMES,			// PM__MAX_JUMP_FRAMES
	DEFAULT_PLAYER_MAX_SAFE_FALL_FRAMES,	// PM__MAX_SAFE_FALL_FRAMES
	DEFAULT_PLAYER_MAX_RUN_VELOCITY,		// PM__MAX_RUN_VELOCITY
	DEFAULT_PLAYER_RUN_SPEEDUP,				// PM__RUN_SPEEDUP
	DEFAULT_PLAYER_RUN_REVERSESLOWDOWN,		// PM__RUN_REVERSESLOWDOWN
	DEFAULT_PLAYER_RUN_SLOWDOWN,			// PM__RUN_SLOWDOWN
	DEFAULT_PLAYER_PLAYER_GRAVITY/3,		// PM__GRAVITY
	DEFAULT_PLAYER_TERMINAL_VELOCITY/3,		// PM__TERMINAL_VELOCITY
}	};


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CPlayerModeBalloon::initialise(class CPlayer *_player)
{
	CPlayerModeBase::initialise(_player);
	m_sprites=new ("BalloonSprite") SpriteBank();
	m_sprites->load(INGAMEFX_INGAMEFX_SPR);
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CPlayerModeBalloon::shutdown()
{
	m_sprites->dump();		delete m_sprites;
	CPlayerModeBase::shutdown();
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CPlayerModeBalloon::enter()
{
	CPlayerModeBase::enter();
	CSoundMediator::playSfx(CSoundMediator::SFX_BALLOON_INFLATE);
	m_balloonTimer=BALLOON_TIME;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CPlayerModeBalloon::think()
{
	CPlayerModeBase::think();
	if(--m_balloonTimer==0||getPadInputDown()&PI_ACTION)
	{
		PAUL_DBGMSG("*pop*");
		CSoundMediator::playSfx(CSoundMediator::SFX_BALLOON_POP);
		m_player->setMode(PLAYER_MODE_FULLUNARMED);
	}
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int balloonx=-14;
int balloony=-90;
void	CPlayerModeBalloon::render()
{
	DVECTOR	ofs,pos;
	CPlayerModeBase::render();

	if(m_balloonTimer>BALLOON_FLASH_TIME||
	   m_balloonTimer&2)
	{
		ofs=CLevel::getCameraPos();
		pos=m_player->getPlayerPos();
		pos.vx+=balloonx-ofs.vx;
		pos.vy+=balloony-ofs.vy;
		m_sprites->printFT4(FRM__BALLOON,pos.vx,pos.vy,0,0,0);
	}
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
const struct PlayerMetrics	*CPlayerModeBalloon::getPlayerMetrics()
{
	return &s_playerMetrics;
}

/*===========================================================================
end */
