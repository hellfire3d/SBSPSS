/*=========================================================================

	tcamlock.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

/*----------------------------------------------------------------------
	Includes
	-------- */

#include "triggers\trigger.h"

#include "triggers\tcamlock.h"

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
void	CCameraLockTrigger::collidedWith(CThing *_thisThing)
{
	CPlayer::CameraBox	camBox={m_boxX1,m_boxY1,m_boxX2,m_boxY2};

	switch( _thisThing->getThingType() )
	{
		case TYPE_PLAYER:
		{
			((CPlayer*)_thisThing)->setCameraBox(camBox);
			break;
		}

		default:
			break;
	}
}

/*===========================================================================
end */
