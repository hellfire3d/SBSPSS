/*=========================================================================

	tsecret.cpp

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
#include "triggers\tsecret.h"

#ifndef	__SOUND_SOUND_H__
#include "sound\sound.h"
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
void	CSecretAreaTrigger::collidedWith(CThing *_thisThing)
{
	switch( _thisThing->getThingType() )
	{
		case TYPE_PLAYER:
		{
			CSoundMediator::playSfx(CSoundMediator::SFX_SECRET_AREA);
			shutdown();
			delete this;
			break;
		}

		default:
			break;
	}
}

/*===========================================================================
end */
