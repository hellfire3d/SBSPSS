/*=========================================================================

	thazard.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

/*----------------------------------------------------------------------
	Includes
	-------- */

#include "triggers\trigger.h"
#include "triggers\thazard.h"

#ifndef __GAME_GAME_H__
#include "game\game.h"
#endif

#ifndef	__PLAYER_PLAYER_H__
#include "player\player.h"
#endif

	
/*	Std Lib
	------- */

/*	Data
	---- */

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
void	CHazardTrigger::init()
{
	CTrigger::init();

	m_timeout = 0;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CHazardTrigger::think(int _frames)
{
	CTrigger::think( _frames );

	if ( m_timeout > 0 )
	{
		m_timeout -= _frames;
	}
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CHazardTrigger::collidedWith(CThing *_thisThing)
{
	if ( m_timeout <= 0 )
	{
		switch( _thisThing->getThingType() )
		{
			case TYPE_PLAYER:
			{
				CPlayer *player = (CPlayer *) _thisThing;
				ATTACK_STATE playerState = player->getAttackState();

				if ( playerState == ATTACK_STATE__BUTT_BOUNCE )
				{
					m_hazard->trigger();

					m_timeout = GameState::getOneSecondInFrames();
				}

				break;
			}

			default:
				break;
		}
	}
}

/*===========================================================================
end */
