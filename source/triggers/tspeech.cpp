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

#ifndef	__SOUND_SOUND_H__
#include "sound\sound.h"
#endif


int		CSpeechTrigger::m_speechRef[9] =
{
	SPEECH_151,
	SPEECH_152,
	SPEECH_153,
	SPEECH_154,
	SPEECH_155,
	SPEECH_156,
	SPEECH_157,
	SPEECH_158,
	SPEECH_159,
};

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
			CSoundMediator::playSpeech( m_speechRef[m_data] );
			setToShutdown();

			break;
		}

		default:
			break;
	}
}

/*===========================================================================
end */
