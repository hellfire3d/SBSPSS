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

// States
#ifndef __PLAYER__PSJUMP_H__
#include "player\psjump.h"
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

#ifndef __PLAYER__PSDUCK_H__
#include "player\psduck.h"
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

static	CPlayerState	*s_stateTable[]=
{
	&s_stateCoralBlowerIdle,				// STATE_IDLE
	&s_stateCoralBlowerIdle,				// STATE_IDLETEETER
	&s_stateJump,							// STATE_JUMP
	&s_stateWalk,							// STATE_RUN
	&s_stateFall,							// STATE_FALL
	&s_stateFallFar,						// STATE_FALLFAR
	&s_stateHitGround,						// STATE_HITGROUND
	NULL,									// STATE_BUTTBOUNCE
	NULL,									// STATE_BUTTFALL
	NULL,									// STATE_BUTTLAND
	NULL,									// STATE_DUCK
	NULL,									// STATE_SOAKUP
	&s_stateGetUp,							// STATE_GETUP
};



static	PlayerMetrics	s_playerMetrics=
{	{
	DEFAULT_PLAYER_JUMP_VELOCITY/2,			// PM__JUMP_VELOCITY
	DEFAULT_PLAYER_MAX_JUMP_FRAMES,			// PM__MAX_JUMP_FRAMES
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
	m_blowerState=BLOWER_STATE__EMPTY;
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
			else if(!(getPadInputHeld()&PI_ACTION))
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

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
CPlayerState	**CPlayerModeCoralBlower::getStateTable()
{
	return s_stateTable;
}

/*===========================================================================
end */
