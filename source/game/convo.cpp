/*=========================================================================

	convo.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/


/*----------------------------------------------------------------------
	Includes
	-------- */

#include "game\convo.h"

#ifndef	__MEMORY_HEADER__
#include "mem\memory.h"
#endif

#ifndef	__GUI_GUI_H__
#include "gui\gui.h"
#endif

#ifndef __GFX_SPRBANK_H__
#include "gfx\sprbank.h"
#endif

#ifndef __GFX_FONT_H__
#include "gfx\font.h"
#endif

#ifndef __SCRIPT_SCRIPT_H__
#include "script\script.h"
#endif

#ifndef __PAD_PADS_H__
#include "pad\pads.h"
#endif

#ifndef __LEVEL_LEVEL_H__
#include "level\level.h"
#endif

#ifndef __SOUND_SOUND_H__
#include "sound\sound.h"
#endif

#ifndef __SPEECH_DEFINES_H__
#include "sound\speech.h"
#endif

#ifndef __LOCALE_TEXTDBASE_H__
#include "locale\textdbase.h"
#endif


/*	Std Lib
	------- */

/*	Data
	---- */

#ifndef __SPR_SPRITES_H__
#include <sprites.h>
#endif

#ifndef __STRING_ENUMS__
#include <trans.h>
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
struct	sSpeechTable
{
		int		TextID;
		int		SpeechID;
};

const sSpeechTable	SpeechTable[]=
{
{-1,SPEECH_CH1_BREAD},
{STR__INGAME__CH1__L2__SANDY3		,SPEECH_CH1_HELMET},
{STR__INGAME__CH1__L2__SANDY2		,SPEECH_CH1_I_LIKE_YOU},
{STR__INGAME__CH2__L1__KRUSTY1		,SPEECH_CH1_SPNGEBOB_ARRRG},
{STR__INGAME__CH1__L4__KRUSTY2		,SPEECH_CH1_SPONGEBOB_VACATION},
{STR__INGAME__CH1__L2__SANDY1		,SPEECH_CH1_TAKE_THAT_YOU},
{-1,SPEECH_CH1_YIPPE},
{-1,SPEECH_SPONGEBOB_ANNOYING_LAUGH},
{-1,SPEECH_SPONGEBOB_BREATHING},
{-1,SPEECH_SPONGEBOB_BUBBLEBLOW},
{-1,SPEECH_SPONGEBOB_BURP},
{-1,SPEECH_SPONGEBOB_COUGH_COUGH},
{-1,SPEECH_SPONGEBOB_HIYA},
{-1,SPEECH_SPONGEBOB_IM_READY},
{-1,SPEECH_SPONGEBOB_I_LOVE_BIKIBI_BOTTOM},
{-1,SPEECH_SPONGEBOB_I_NEED_WATER},
{-1,SPEECH_SPONGEBOB_LET_ME_JUST_GRAB},
{-1,SPEECH_SPONGEBOB_OPPPS_PANTS},
{-1,SPEECH_SPONGEBOB_OUCH},
{-1,SPEECH_SPONGEBOB_YIKES},
{-1,SPEECH_SPONGEBOB_YOU_CANT_FOOL_ME},
{-1,SPEECH_CH2_BACK_ALREADY},
{-1,SPEECH_CH2_BACK_TO_MERMAID_MAN},
{-1,SPEECH_CH2_FIND_FALSE_TEETH},
{-1,SPEECH_CH2_GARY_FOLLOW_ME},
{STR__INGAME__CH2__L2__PATRICK1		,SPEECH_CH2_HEY_SPNGEBOB},
{STR__INGAME__CH2__L2__PATRICK2		,SPEECH_CH2_HWERE_USE_MY_BALLON},
{-1,SPEECH_CH2_IM_ON_MY_WAY},
{-1,SPEECH_CH2_IM_READY_OOPS},
{-1,SPEECH_CH2_MOIST},
{STR__INGAME__CH4__L3__KRUSTY1		,SPEECH_CH2_NOW_LISTEN_BOY},
{STR__INGAME__CH2__L1__KRUSTY2		,SPEECH_CH2_USE_THE_CORAL_BLOWER},
{-1,SPEECH_CH2_YIPEE},
{-1,SPEECH_SQUIDWARD_ARE_YOU_INSANE},
{-1,SPEECH_SQUIDWARD_CAN_WE_LOWER_VOLUME},
{-1,SPEECH_SQUIDWARD_HAHAHONKHONK},
{-1,SPEECH_SQUIDWARD_HMM_OH_BOY},
{-1,SPEECH_SQUIDWARD_HOW_DID_I},
{-1,SPEECH_SQUIDWARD_TUT},
{STR__INGAME__CH3__L1__PLANKTON1	,SPEECH_CH3_BLAST_YOU_SPONGEBREATH},
{-1,SPEECH_CH3_BY_THE_POWER_OF_NEPTUNE},
{STR__INGAME__CH3__L3__PATRICK1		,SPEECH_CH3_DADADADADA},
{STR__INGAME__CH3__L3__PATRICK2		,SPEECH_CH3_DIPPED_AND_READY_TO_GO},
{-1,SPEECH_CH3_GASP},
{-1,SPEECH_CH3_HAHAHAHAHAHA},
{-1,SPEECH_CH3_HA_TO_THE_KELP_JUNGLE},
{-1,SPEECH_CH3_LOOK_WHAT_IVE_GOT_FOR_YOU},
{-1,SPEECH_CH3_LOOK_YOU_ARE_BUGGING_ME},
{-1,SPEECH_CH3_PATRICK},
{-1,SPEECH_CH3_SURPRISE_IM_BACK},
{-1,SPEECH_CH3_TARZAN},
{-1,SPEECH_CH3_THAT_IS_AMAZING},
{-1,SPEECH_CH3_UH_OH_THOSE_PLANTS},
{-1,SPEECH_CH3_UNIFORMS},
{-1,SPEECH_CH3_WHATS_THAT_SMELL},
{-1,SPEECH_DUTCHMAN_BET_YOUR_LIVERS},
{-1,SPEECH_DUTCHMAN_HAHA_YOU_SCALLYWAGS},
{-1,SPEECH_DUTCHMAN_IM_GONNA_STEAL_YOUR_SOUL},
{-1,SPEECH_DUTCHMAN_WAHAHA},
{-1,SPEECH_PARTY_HAPPY_BIRTHADAY_PATRICK},
{-1,SPEECH_PARTY_STAND_STILL},
{-1,SPEECH_PARTY_TODAY_IS_MY_BIRTDAY_HMMMM},
{-1,SPEECH_CH4_DOES_A_SNAIL_MEOW},
{-1,SPEECH_CH4_HMM_THATS_FUNNY___I_KNOW},
{-1,SPEECH_CH4_SQUEAKYBOOTS},
{-1,SPEECH_CH4_TARTAR},
{-1,SPEECH_CH4_THIS_ONES_EMPTY_TOO},
{-1,SPEECH_CH4_THIS_ONE_TOO},
{-1,SPEECH_CH4_TV_FIXED},
{STR__INGAME__CH4__L3__KRUSTY2		,SPEECH_CH4_WEAR_YOUR_BOOTS},
{-1,SPEECH_CH4_YIHEEE_IVE_GOT_A_KANDY_BAR},
{-1,SPEECH_PATRICK_ANYONE_WANT_A_DANC3},
{-1,SPEECH_PATRICK_DONT_WORRY_BUDDY},
{-1,SPEECH_PATRICK_HURH_HURH},
{-1,SPEECH_PATRICK_ITS_MY_BIRTDAY},
{-1,SPEECH_PATRICK_MERRY_XMAS},
{-1,SPEECH_PATRICK_TECHNIQUE},
{-1,SPEECH_PATRICK_YIKES},
{-1,SPEECH_PATRICK_YOUR_DOING_FINE},
{-1,SPEECH_CH5_BIRTDAY_SURPRISE},
{-1,SPEECH_CH5_CONGFRATUS},
{-1,SPEECH_CH5_FIND_AN_AERIAL},
{-1,SPEECH_CH5_FREINDSHIP_PRICELESS},
{-1,SPEECH_CH5_NEED_A_TV_FIXED},
{-1,SPEECH_CH5_NEED_MONKEY_WRENCH},
{-1,SPEECH_CH5_NOPE_JUST_FORCE_THAT},
{-1,SPEECH_CH5_OILRIG_IDEA},
{-1,SPEECH_CH5_POURUS_FRTEAK},
{-1,SPEECH_SANDY_HEHEHEHEHEHE},
{-1,SPEECH_SANDY_HIYAH},
{-1,SPEECH_SANDY_HOLD_ON_THERE_DUDE},
{-1,SPEECH_SANDY_I_LIKE_YOU_SPONGEBOB},
{-1,SPEECH_SANDY_TAKE_THAT_YOU_OLD},
{-1,SPEECH_SANDY_YEEHAR},
{-1,SPEECH_SANDY_YOUR_A_FUNNY_LITTLE},
{-1,SPEECH_INTRO_AH_HERE_WE_ARE_AT},
{-1,SPEECH_INTRO_GARY_I_JUST_HAD_AN_IDEA},
{STR__INGAME__CH1__L1__SPONGEY1			,SPEECH_INTRO_HI_SQUIDWOOD_READY_FOR_A},
{-1,SPEECH_INTRO_ITS_MY_BEST_FRIEND},
{-1,SPEECH_INTRO_LIKE_A_CALCULATOR},
{STR__INGAME__CH1__L1__SQUIDWARD2		,SPEECH_INTRO_LOOK_SPONGEBOB},
{-1,SPEECH_INTRO_MEOW},
{-1,SPEECH_INTRO_OH_NO_NOT_YOU_AGAIN},
{-1,SPEECH_INTRO_SEANUT_BUTTER},
{STR__INGAME__CH1__L1__SQUIDWARD1		,SPEECH_INTRO_TODAYS_SUNDAY},
{-1,SPEECH_INTRO_TODAY_HE_IS_IN_SEARCH},
{-1,SPEECH_INTRO_WELL_WE_ARE_A_BIT_BUSY},
{-1,SPEECH_INTRO_YIPPED____NOW},
{-1,SPEECH_KRABS_HELLO_ME_BOY},
{-1,SPEECH_KRABS_ILL_BE_IN_MY_QUARTERS},
{-1,SPEECH_KRABS_WELL_LAD},
{-1,SPEECH_PLANKTON_COLLEGE},
{-1,SPEECH_PLANKTON_GVE_ME_THE_PATTY},
{-1,SPEECH_PLANKTON_HAND_ME_CRABBY_PATTY},
{-1,SPEECH_PLANKTON_MWAHAHHAHA},
{-1,SPEECH_PLANKTON_OUCH},
{-1,SPEECH_PLANKTON_THIS_STINKS},
{-1,SPEECH_PLANKTON_YOU_POROUS_FREAK},
};
const int	SpeechTableSize=sizeof(SpeechTable)/sizeof(sSpeechTable);

CConversation::CHAR_ICON_FRAMES	CConversation::s_characterIconFrames[]=
{
	{	FRM_SPONGEBOB,		},			// CHAR_SPONGEBOB
	{	FRM_SANDY			},			// CHAR_SANDY
	{	FRM_PATRICK			},			// CHAR_PATRICK
	{	FRM_SQUIDWARD		},			// CHAR_SQUIDWARD
	{	FRM_MR_KRABS		},			// CHAR_KRABS
	{	FRM_PLANKTON		},			// CHAR_PLANKTON
	{	FRM_MERMAIDMAN		},			// CHAR_MERMAIDMAN,
	{	FRM_BARNACLEBOY		},			// CHAR_BARNACLEBOY,
	{	-1					},			// CHAR_JACKCUSTARD

};

class FontBank			*CConversation::s_textFontBank=NULL;
class FontBank			*CConversation::s_questionFontBank=NULL;

class CScript			*CConversation::s_registeredScripts[MAX_LOADED_SCRIPTS];
int						CConversation::s_registeredScriptIds[MAX_LOADED_SCRIPTS];
int						CConversation::s_numRegisteredScripts=0;

class CScript			*CConversation::s_currentScript=NULL;
int						CConversation::s_currentState=STATE_INACTIVE;

int	CConversation::s_currentQuestion;
int CConversation::s_currentAnswer;
int CConversation::s_currentSelectedAnswer;

int	CConversation::s_faceFrame;
int	CConversation::s_speechId;
int	CConversation::s_textOffset;
int	CConversation::s_maxTextOffset;

static xmPlayingId	s_playingSfxId;


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CConversation::init()
{
	ASSERT(s_numRegisteredScripts==0);

	s_textFontBank=new ("Conversation Font") FontBank();
	s_textFontBank->initialise(&standardFont);
	s_textFontBank->setOt(0);
	s_textFontBank->setPrintArea(TEXTBOX_X,TEXTBOX_Y,TEXTBOX_WIDTH,TEXTBOX_HEIGHT);

	s_questionFontBank=new ("Conversation Font") FontBank();
	s_questionFontBank->initialise(&standardFont);
	s_questionFontBank->setOt(0);

	s_currentState=STATE_INACTIVE;
	s_currentScript=NULL;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CConversation::shutdown()
{
	s_questionFontBank->dump();				delete s_questionFontBank;
	s_textFontBank->dump();					delete s_textFontBank;
	dumpConversationScripts();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CConversation::think(int _frames)
{
	if(isActive())
	{
		ASSERT(s_currentScript);

		if(s_currentState==STATE_JUST_ACTIVATED)
		{
			// Need to make sure that the conversation box isn't rendered on the first frame
			// ( ie: before it's had time to think ) or it will still be displaying old info
			// from the last time it was up
			s_currentState=STATE_ACTIVE;
		}

		thinkText();
		thinkQuestion();

		s_currentScript->run();
		if(s_currentScript->isFinished())
		{
			s_currentScript=NULL;
			s_currentState=STATE_INACTIVE;
			CSoundMediator::stopSpeech();
			if(s_playingSfxId!=NOT_PLAYING)
			{
				CSoundMediator::stopAndUnlockSfx(s_playingSfxId);
				s_playingSfxId=NOT_PLAYING;
			}
			s_playingSfxId=CSoundMediator::playSfx(CSoundMediator::SFX_FRONT_END__OK);
		}
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CConversation::render()
{
	if(s_currentState==STATE_ACTIVE)
	{
		renderText();
		renderQuestion();
		drawSpeechBubbleBorder(TEXTBOX_X,TEXTBOX_Y,TEXTBOX_WIDTH,TEXTBOX_HEIGHT+TEXTBOX_QUESTIONHEIGHT,0,s_faceFrame);
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CConversation::registerConversationScript(FileEquate _feScript)
{
	ASSERT(s_numRegisteredScripts<MAX_LOADED_SCRIPTS);

	// Load in the script and register its id
	s_registeredScripts[s_numRegisteredScripts]=new ("conversation script") CScript();
	s_registeredScripts[s_numRegisteredScripts]->initialise(_feScript);
	s_registeredScriptIds[s_numRegisteredScripts]=_feScript;
	s_numRegisteredScripts++;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CConversation::trigger(FileEquate _feScript)
{
	int	i;

	ASSERT(!isActive());

	s_playingSfxId=NOT_PLAYING;

	// Is this script already registered?
	for(i=0;i<s_numRegisteredScripts;i++)
	{
		if(s_registeredScriptIds[i]==_feScript)
		{
			// Found it..
			s_currentScript=s_registeredScripts[i];
			s_currentState=STATE_JUST_ACTIVATED;
			s_currentScript->reset();
			return;
		}
	}

	ASSERT(!"Script was not registered");
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int CConversation::isActive()
{
	return s_currentState!=STATE_INACTIVE;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CConversation::setCharacterAndText(int _characterId,int _textId)
{
	s_faceFrame=(s_characterIconFrames[_characterId].m_frame);
	s_speechId=_textId;
	s_textOffset=0;
	s_maxTextOffset=s_textFontBank->getStringHeight((char*)TranslationDatabase::getString(s_speechId))-TEXTBOX_HEIGHT;
	if(s_maxTextOffset<0)s_maxTextOffset=0;

	for (int i=0; i<SpeechTableSize; i++)
	{
		if (SpeechTable[i].TextID==_textId)
		{
			CSoundMediator::playSpeech((SpeechEquate)SpeechTable[i].SpeechID);
			break;
		}
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CConversation::setResponseOptions(int _responseOptions)
{
	s_currentQuestion=_responseOptions;
	s_currentAnswer=ANSWER_NONE;
	s_currentSelectedAnswer=0;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int CConversation::getResponse()
{
	return s_currentAnswer;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CConversation::thinkText()
{
	int	sfx=-1;

	if(PadGetRepeat(0)&PAD_DOWN)
	{
		if(s_textOffset<s_maxTextOffset)
		{
			s_textOffset+=TEXT_STEP_SIZE;
			if(s_textOffset>s_maxTextOffset)
			{
				s_textOffset=s_maxTextOffset;
			}
			sfx=CSoundMediator::SFX_FRONT_END__MOVE_CURSOR;
		}
		else
		{
			sfx=CSoundMediator::SFX_FRONT_END__ERROR;
		}
	}
	else if(PadGetRepeat(0)&PAD_UP)
	{
		if(s_textOffset>0)
		{
			s_textOffset-=TEXT_STEP_SIZE;
			if(s_textOffset<0)
			{
				s_textOffset=0;
			}
			sfx=CSoundMediator::SFX_FRONT_END__MOVE_CURSOR;
		}
		else
		{
			sfx=CSoundMediator::SFX_FRONT_END__ERROR;
		}
	}

	if(sfx!=-1)
	{
		if(s_playingSfxId!=NOT_PLAYING)
		{
			CSoundMediator::stopAndUnlockSfx(s_playingSfxId);
		}
		s_playingSfxId=CSoundMediator::playSfx((CSoundMediator::SFXID)sfx,true);
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CConversation::thinkQuestion()
{
	if(s_currentQuestion!=QUESTION_NONE)
	{
		int	pad=PadGetDown(0);

		if(s_currentQuestion==QUESTION_YES_NO)
		{
			if(pad&CPadConfig::getButton(CPadConfig::PAD_CFG_JUMP))
			{
				s_currentAnswer=s_currentSelectedAnswer==0?ANSWER_YES:ANSWER_NO;
				s_currentQuestion=QUESTION_NONE;
			}
			else if(pad&(CPadConfig::getButton(CPadConfig::PAD_CFG_LEFT)|CPadConfig::getButton(CPadConfig::PAD_CFG_RIGHT)))
			{
				s_currentSelectedAnswer^=1;
			}
		}
		else if(s_currentQuestion==QUESTION_OK)
		{
			if(pad&CPadConfig::getButton(CPadConfig::PAD_CFG_JUMP))
			{
				s_currentAnswer=ANSWER_OK;
				s_currentQuestion=QUESTION_NONE;
			}
		}
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CConversation::renderText()
{
	RECT clipTextRegion={TEXTBOX_X,TEXTBOX_Y,TEXTBOX_WIDTH,TEXTBOX_HEIGHT};

	PrimFullScreen(0);
	s_textFontBank->print(0,-s_textOffset,s_speechId);
	PrimClip(&clipTextRegion,0);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
//drawSpeechBubbleBorder(TEXTBOX_X,TEXTBOX_Y,TEXTBOX_WIDTH,TEXTBOX_HEIGHT+TEXTBOX_QUESTIONHEIGHT,0,s_faceFrame);
void CConversation::renderQuestion()
{
	int	y;

	y=TEXTBOX_Y+TEXTBOX_HEIGHT+TEXTBOX_QUESTIONHEIGHT-s_questionFontBank->getCharHeight();

	switch(s_currentQuestion)
	{
		case QUESTION_OK:
			{
				s_questionFontBank->setColour(SELECT_TEXT_R,SELECT_TEXT_G,SELECT_TEXT_B);
				s_questionFontBank->setJustification(FontBank::JUST_RIGHT);
				s_questionFontBank->print(TEXTBOX_X+TEXTBOX_WIDTH,y,STR__OK);
			}
			break;

		case QUESTION_YES_NO:
			{
				if(s_currentSelectedAnswer==0)
				{
					s_questionFontBank->setColour(SELECT_TEXT_R,SELECT_TEXT_G,SELECT_TEXT_B);
				}
				else
				{
					s_questionFontBank->setColour(UNSELECT_TEXT_R,UNSELECT_TEXT_G,UNSELECT_TEXT_B);
				}
				s_questionFontBank->setJustification(FontBank::JUST_LEFT);
				s_questionFontBank->print(TEXTBOX_X+TEXTBOX_WIDTH,y,STR__YES);

				if(s_currentSelectedAnswer==1)
				{
					s_questionFontBank->setColour(SELECT_TEXT_R,SELECT_TEXT_G,SELECT_TEXT_B);
				}
				else
				{
					s_questionFontBank->setColour(UNSELECT_TEXT_R,UNSELECT_TEXT_G,UNSELECT_TEXT_B);
				}
				s_questionFontBank->setJustification(FontBank::JUST_RIGHT);
				s_questionFontBank->print(TEXTBOX_X,y,STR__NO);
			}
			break;

		case QUESTION_NONE:
		default:
			break;
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CConversation::dumpConversationScripts()
{
	int	i;

	for(i=0;i<s_numRegisteredScripts;i++)
	{
		s_registeredScripts[i]->dump();
		delete s_registeredScripts[i];
	}
	s_currentScript=NULL;
	s_numRegisteredScripts=0;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CConversation::registerConversationLevelScripts(int level)
{
	switch( CLevel::getCurrentChapter() )
	{
		case 1:
		{
			switch ( CLevel::getCurrentChapterLevel() )
			{
				case 1:
				{
					CConversation::registerConversationScript(SCRIPTS_CH1L1_01_DAT);
					CConversation::registerConversationScript(SCRIPTS_CH1L1_02_DAT);
					break;
				}

				case 2:
				{
					CConversation::registerConversationScript(SCRIPTS_CH1L2_01_DAT);
					CConversation::registerConversationScript(SCRIPTS_CH1L2_02_DAT);
					CConversation::registerConversationScript(SCRIPTS_CH1L2_03_DAT);
					break;
				}

				case 4:
				{
					CConversation::registerConversationScript(SCRIPTS_CH1L4_01_DAT);
					CConversation::registerConversationScript(SCRIPTS_CH1L4_02_DAT);
					break;
				}

				default:
					break;
			}

			break;
		}

		case 2:
		{
			switch ( CLevel::getCurrentChapterLevel() )
			{
				case 1:
				{
					CConversation::registerConversationScript(SCRIPTS_CH2L1_01_DAT);
					CConversation::registerConversationScript(SCRIPTS_CH2L1_02_DAT);
					break;
				}

				case 2:
				{
					CConversation::registerConversationScript(SCRIPTS_CH2L2_01_DAT);
					CConversation::registerConversationScript(SCRIPTS_CH2L2_02_DAT);
					break;
				}

				default:
					break;
			}

			break;
		}

		case 3:
		{
			switch ( CLevel::getCurrentChapterLevel() )
			{
				case 1:
				{
					CConversation::registerConversationScript(SCRIPTS_CH3L1_01_DAT);
					break;
				}

				case 3:
				{
					CConversation::registerConversationScript(SCRIPTS_CH3L3_01_DAT);
					CConversation::registerConversationScript(SCRIPTS_CH3L3_02_DAT);
					break;
				}

				default:
					break;
			}

			break;
		}

		case 4:
		{
			switch ( CLevel::getCurrentChapterLevel() )
			{
				case 3:
				{
					CConversation::registerConversationScript(SCRIPTS_CH4L3_01_DAT);
					CConversation::registerConversationScript(SCRIPTS_CH4L3_02_DAT);
					break;
				}

				default:
					break;
			}

			break;
		}

		default:
			break;
	}
}
/*===========================================================================
 end */
