/*=========================================================================

	pmcoral.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

/*----------------------------------------------------------------------
	Includes
	-------- */

#include "player\pmcoral.h"

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

static PlayerMetrics	s_playerMetrics=
{	{
	DEFAULT_PLAYER_JUMP_VELOCITY/2,			// PM__JUMP_VELOCITY
	DEFAULT_PLAYER_MAX_JUMP_FRAMES/2,		// PM__MAX_JUMP_FRAMES
	DEFAULT_PLAYER_MAX_SAFE_FALL_FRAMES,	// PM__MAX_SAFE_FALL_FRAMES
	DEFAULT_PLAYER_MAX_RUN_VELOCITY/3,		// PM__MAX_RUN_VELOCITY
	DEFAULT_PLAYER_RUN_SPEEDUP/2,			// PM__RUN_SPEEDUP
	DEFAULT_PLAYER_RUN_REVERSESLOWDOWN,		// PM__RUN_REVERSESLOWDOWN
	DEFAULT_PLAYER_RUN_SLOWDOWN,			// PM__RUN_SLOWDOWN
	DEFAULT_PLAYER_PLAYER_GRAVITY,			// PM__GRAVITY
	DEFAULT_PLAYER_TERMINAL_VELOCITY,		// PM__TERMINAL_VELOCITY
}	};


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CPlayerModeCoralBlower::enter()
{
	m_blowerState=BLOWER_STATE__FULL;//BLOWER_STATE__EMPTY;
	CSoundMediator::playSfx(CSoundMediator::SFX_ITEM__CORAL_BLOWER);
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int cbstate=0;
void	CPlayerModeCoralBlower::think()
{
	CPlayerModeBase::think();

	switch(m_blowerState)
	{
		case BLOWER_STATE__EMPTY:
			if(getPadInputDown()&PI_ACTION&&getState()==STATE_IDLE)
			{
				m_blowerState=BLOWER_STATE__SUCKING;
			}
			break;
		case BLOWER_STATE__SUCKING:
			if(!(getPadInputHeld()&PI_ACTION&&getState()==STATE_IDLE))
			{
				m_blowerState=BLOWER_STATE__EMPTY;
			}
			break;
		case BLOWER_STATE__FULL:
			if(getPadInputDown()&PI_ACTION&&getState()==STATE_IDLE)
			{
				m_blowerState=BLOWER_STATE__AIMING;
			}
			break;
		case BLOWER_STATE__AIMING:
			if(getState()!=STATE_IDLE)
			{
				m_blowerState=BLOWER_STATE__FULL;
			}
			if(!getPadInputHeld()&PI_ACTION)
			{
				// Fire!
				m_blowerState=BLOWER_STATE__EMPTY;
			}
			break;
	}
cbstate=m_blowerState;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CPlayerModeCoralBlower::renderModeUi()
{
	SpriteBank	*sb;
	sFrameHdr	*fh;

	sb=m_player->getSpriteBank();
	fh=sb->getFrameHeader(FRM__BLOWER);
	if(m_blowerState==BLOWER_STATE__FULL||m_blowerState==BLOWER_STATE__AIMING)
	{
		// Blower has a creature/object inside
		sb->printFT4Scaled(FRM__BLOWER,CPlayer::POWERUPUI_ICONX-(fh->W/2),CPlayer::POWERUPUI_ICONY-(fh->H/2),0,0,CPlayer::POWERUPUI_OT,256+128);
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
const struct PlayerMetrics	*CPlayerModeCoralBlower::getPlayerMetrics()
{
	return &s_playerMetrics;
}

/*===========================================================================
end */
