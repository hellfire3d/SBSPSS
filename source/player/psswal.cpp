/*=========================================================================

	psswal.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/


/*----------------------------------------------------------------------
	Includes
	-------- */

#include "player\psswal.h"

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

CPlayerStateSwallow			s_stateSwallow;

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayerStateSwallow::enter(CPlayerModeBase *_playerMode)
{
	_playerMode->setAnimNo(ANIM_SPONGEBOB_DONOTDRAW);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayerStateSwallow::think(CPlayerModeBase *_playerMode)
{
	_playerMode->setAnimNo(ANIM_SPONGEBOB_DONOTDRAW);
}


/*===========================================================================
 end */
