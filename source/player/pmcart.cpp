/*=========================================================================

	pmcart.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

/*----------------------------------------------------------------------
	Includes
	-------- */

#include "player\pmcart.h"

#ifndef __GAME_GAME_H__
#include "game\game.h"
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

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CPlayerModeCart::enter()
{
	m_player->setCartCam(true);
	m_player->setFacing(FACING_RIGHT);
	setState( STATE_CART );
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CPlayerModeCart::think()
{
	DVECTOR newPos;

	CThing	*platform;
	platform=m_player->isOnPlatform();

	if ( platform )
	{
		newPos=platform->getPos();
//		newPos.vx = platform->getPos().vx;
//		newPos.vy = platform->getPos().vy;

		int platformOffset = ( ( CNpcPlatform* ) platform )->getHeightFromPlatformAtPosition( newPos.vx, newPos.vy );
		s16 angle = ( ( CNpcPlatform * ) platform )->getCollisionAngle();
		newPos.vx += ( -platformOffset * rsin( angle ) ) >> 12;
		platformOffset = ( ( CNpcPlatform* ) platform )->getHeightFromPlatformAtPosition( newPos.vx, newPos.vy );
		newPos.vy += platformOffset;

		m_player->setPos( newPos );

		getStateTable()[getState()]->think(this);
	}
	else
	{
		m_player->setMode( PLAYER_MODE_BASICUNARMED );
		setState( STATE_IDLE );
	}
}

/*===========================================================================
end */
