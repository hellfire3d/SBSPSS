/*=========================================================================

	tspeech.cpp

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

#ifndef	__GAME_CONVO_H__
#include "game\convo.h"
#endif





// Makes the speech triggers you proper conversation boxes
#define __SPEECH_TRIGGERS_AS_CONVERSATIONS__





#ifdef	__SPEECH_TRIGGERS_AS_CONVERSATIONS__
int		CSpeechTrigger::s_speechRef[9] =
{
	SCRIPTS_TRIGGERSPEECH_151_DAT,
	SCRIPTS_TRIGGERSPEECH_152_DAT,
	SCRIPTS_TRIGGERSPEECH_153_DAT,
	SCRIPTS_TRIGGERSPEECH_154_DAT,
	SCRIPTS_TRIGGERSPEECH_155_DAT,
	SCRIPTS_TRIGGERSPEECH_156_DAT,
	SCRIPTS_TRIGGERSPEECH_157_DAT,
	SCRIPTS_TRIGGERSPEECH_158_DAT,
	SCRIPTS_TRIGGERSPEECH_159_DAT,
};
#else
int		CSpeechTrigger::s_speechRef[9] =
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

#ifdef	__SPEECH_TRIGGERS_AS_CONVERSATIONS__
			CConversation::trigger((FileEquate)s_speechRef[m_data]);
#else
			CSoundMediator::playSpeech( s_speechRef[m_data] );
#endif
			setToShutdown();

			break;
		}

		default:
			break;
	}
}

/*===========================================================================
end */
