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
	SCRIPTS_TRIGGERSPEECH_FIRSTNET_DAT,//SPEECH_151,
	SCRIPTS_TRIGGERSPEECH_FIRSTCORAL_DAT,//SPEECH_152,
	SCRIPTS_TRIGGERSPEECH_GARYCH2L1_DAT,//SPEECH_153,
	SCRIPTS_TRIGGERSPEECH_BREAKFLOOR_DAT,//SPEECH_154,
	SCRIPTS_TRIGGERSPEECH_BUBBLEGEYSER_DAT,//SPEECH_155,
	0,//SPEECH_156,
	SCRIPTS_TRIGGERSPEECH_USEBUBBLE_DAT,//SPEECH_157,
	SCRIPTS_TRIGGERSPEECH_FIRSTBUBBLE_DAT,//SPEECH_158,
	SCRIPTS_TRIGGERSPEECH_WEIGHT_DAT,//SPEECH_159,
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
