/*=========================================================================

	pscart.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/


/*----------------------------------------------------------------------
	Includes
	-------- */

#include "player\pscart.h"

#ifndef __PLAYER_PLAYER_H__
#include "player\player.h"
#endif

#ifndef __PLAYER_PMODES_H__
#include "player\pmodes.h"
#endif

#ifndef __SOUND_SOUND_H__
#include "sound\sound.h"
#endif

#ifndef	__PLATFORM_PLATFORM_H__
#include "platform\platform.h"
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

CPlayerStateCart			s_stateCart;

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayerStateCart::enter(CPlayerModeBase *_playerMode)
{
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayerStateCart::think(CPlayerModeBase *_playerMode)
{
	int controlDown, controlHeld;

	if ( _playerMode->advanceAnimFrameAndCheckForEndOfAnim() )
	{
		_playerMode->setAnimNo( ANIM_SPONGEBOB_IDLEBREATH );
	}
	controlDown=_playerMode->getPadInputDown();
	controlHeld=_playerMode->getPadInputHeld();

	CNpcPlatform *platform;
	platform = (CNpcPlatform *) _playerMode->getPlayer()->isOnPlatform();

	if ( platform )
	{
		if(controlDown&PI_JUMP)
		{
			platform->jump();
		}

		if ( controlHeld & PI_LEFT )
		{
			platform->slowDown();
		}
		else if ( controlHeld & PI_RIGHT )
		{
			platform->speedUp();
		}
	}
}


/*===========================================================================
 end */
