/*=========================================================================

	tspeech.h

	Author:		CRB
	Created:	
	Project:	Spongebob
	Purpose:	

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__TRIGGERS_TSPEECH_H__
#include "triggers\tspeech.h"
#endif



/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CSpeechTrigger::collidedWith(CThing *_thisThing)
{
	switch( _thisThing->getThingType() )
	{
		case TYPE_PLAYER:
		{
			setToShutdown();

			break;
		}

		default:
			break;
	}
}

/*===========================================================================
end */
