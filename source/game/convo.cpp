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
		int		SpeechID;
		int		TextID;
};

const sSpeechTable	SpeechTable[]=
{
{SPEECH_001,STR__INGAME__FMA_C1_INTRO0},
{SPEECH_002,STR__INGAME__FMA_C1_INTRO1},
{SPEECH_003,STR__INGAME__FMA_C1_SB0},
{SPEECH_004,STR__INGAME__FMA_C1_SB1},
{SPEECH_005,STR__INGAME__FMA_C1_SB2},
{SPEECH_006,STR__INGAME__FMA_C1_BB0},
{SPEECH_007,STR__INGAME__FMA_C1_BB1},
{SPEECH_008,STR__INGAME__FMA_C1_MM0},
{SPEECH_009,STR__INGAME__GARY_MEOW},
{SPEECH_010,STR__INGAME__CH1__L1__SPONGEY1},
{SPEECH_011,STR__INGAME__CH1__L1__SQUIDWARD1},
{SPEECH_012,STR__INGAME__CH1__L1__SQUIDWARD2},
{SPEECH_013,STR__INGAME__CH1__L1__SBCELEBRATE},
{SPEECH_014,STR__INGAME__CH1__L2__SANDY1},
{SPEECH_015,STR__INGAME__CH1__L2__SANDY2},
{SPEECH_016,STR__INGAME__CH1__L2__SANDY3},
{SPEECH_017,STR__INGAME__CH1__L2__SBCELEBRATE},
{SPEECH_018,STR__INGAME__CH1__L3__SBCELEBRATE},
{SPEECH_019,STR__INGAME__CH1__L4__KRUSTY1},
{SPEECH_020,STR__INGAME__CH1__L4__KRUSTY2},
{SPEECH_021,STR__INGAME__FMA_C2_MM_ZZZ},
{SPEECH_022,STR__INGAME__FMA_C2_BB_ZZZ},
{SPEECH_023,STR__INGAME__FMA_C2_SB0},
{SPEECH_024,STR__INGAME__FMA_C2_BB0},
{SPEECH_025,STR__INGAME__FMA_C2_SB1},
{SPEECH_026,STR__INGAME__CH2__L1__KRUSTY1},
{SPEECH_027,STR__INGAME__CH2__L1__KRUSTY2},
{SPEECH_028,STR__INGAME__TRIGGERSPEECH_028},
{SPEECH_029,-1},	// Another Meow
{SPEECH_030,STR__INGAME__CH2__L1__SBCELEBRATE},
{SPEECH_031,STR__INGAME__CH2__L2__PATRICK1},
{SPEECH_032,STR__INGAME__CH2__L2__PATRICK2},
{SPEECH_033,STR__INGAME__CH2__L2__SBCELEBRATE},
{SPEECH_034,STR__INGAME__CH2__L3__SBCELEBRATE},
{SPEECH_035,STR__INGAME__CH2__L4__SBCELEBRATE},
{SPEECH_036,STR__INGAME__FMA_C3_SB0},
{SPEECH_037,STR__INGAME__FMA_C3_BB0},
{SPEECH_038,STR__INGAME__FMA_C3_SB1},
{SPEECH_039,STR__INGAME__FMA_C3_BB1},
{SPEECH_040,STR__INGAME__FMA_C3_SB2},
{SPEECH_041,STR__INGAME__FMA_PLANKTON},
{SPEECH_042,STR__INGAME__CH3__L1__SBCELEBRATE},
{SPEECH_043,STR__INGAME__CH3__L2__SBCELEBRATE},
{SPEECH_044,STR__INGAME__CH3__L3__PATRICK1},
{SPEECH_045,STR__INGAME__CH3__L3__PATRICK2},
{SPEECH_046,STR__INGAME__CH3__L3__SBCELEBRATE},
{SPEECH_047,STR__INGAME__CH3__L4__SBCELEBRATE},
{SPEECH_048,STR__INGAME__FMA_C4_SB0},
{SPEECH_049,STR__INGAME__FMA_C4_MM_GASP},
{SPEECH_050,STR__INGAME__FMA_C4_BB_GASP},
{SPEECH_051,STR__INGAME__FMA_C4_SB1},
{SPEECH_052,STR__INGAME__FMA_C4_BB0},
{SPEECH_053,STR__INGAME__CH4__L1__SBCELEBRATE},
{SPEECH_054,STR__INGAME__CH4__L2__SBCELEBRATE},
{SPEECH_055,STR__INGAME__CH4__L3__KRUSTY1},
{SPEECH_056,STR__INGAME__CH4__L3__KRUSTY2},
{SPEECH_057,STR__INGAME__CH4__L3__SBCELEBRATE},
{SPEECH_058,STR__INGAME__CH4__L4__SBCELEBRATE},
{SPEECH_059,STR__INGAME__FMA_C5_SB0},
{SPEECH_060,STR__INGAME__FMA_C5_BB0},
{SPEECH_061,STR__INGAME__FMA_C5_SB1},
{SPEECH_062,STR__INGAME__CH5__L1__SBCELEBRATE},
{SPEECH_063,STR__INGAME__CH5__L2__SBCELEBRATE},
{SPEECH_064,STR__INGAME__CH5__L3__SBCELEBRATE},
{SPEECH_065,STR__INGAME__CH5__L4__SBCELEBRATE},
{SPEECH_066,STR__INGAME__FMA_C6_SB0},
{SPEECH_067,STR__INGAME__FMA_C6_SB1},
{SPEECH_068,STR__INGAME__FMA_C6_BB0},
{SPEECH_069,STR__INGAME__FMA_C6_SB2},
{SPEECH_070,STR__INGAME__FMA_C6_BB1},
{SPEECH_071,STR__INGAME__FMA_C6_BB_MM0},
{SPEECH_072,-1},
{SPEECH_073,STR__INGAME__FMA_PARTY_SB0},
{SPEECH_074,STR__INGAME__FMA_PARTY_PAT0},
{SPEECH_075,STR__INGAME__FMA_PARTY_VO0},
{SPEECH_076,STR__INGAME__FMA_PARTY_SB},
{SPEECH_077,STR__INGAME__FMA_PARTY_MM},
{SPEECH_078,STR__INGAME__FMA_PARTY_BB},
{SPEECH_079,STR__INGAME__FMA_PARTY_GARY},
{SPEECH_080,STR__INGAME__FMA_PARTY_PLANKTON},
{SPEECH_081,STR__INGAME__FMA_PARTY_PAT},
{SPEECH_082,STR__INGAME__FMA_PARTY_KRABS},
{SPEECH_083,STR__INGAME__FMA_PARTY_SQUIDWARD},
{SPEECH_084,STR__INGAME__FMA_PARTY_SANDY},
{SPEECH_085,-1},
{SPEECH_086,-1},
{SPEECH_087,-1},
{SPEECH_088,-1},
{SPEECH_089,-1},
{SPEECH_090,STR__INGAME__CH3__L2__SANDY1},
{SPEECH_091,STR__INGAME__CH3__L2__SANDY2},
{SPEECH_092,-1},
{SPEECH_093,-1},
{SPEECH_094,-1},
{SPEECH_095,-1},
{SPEECH_096,-1},
{SPEECH_097,-1},
{SPEECH_098,-1},
{SPEECH_099,-1},
{SPEECH_100,-1},
{SPEECH_101,-1},
{SPEECH_102,-1},
{SPEECH_103,-1},
{SPEECH_104,-1},
{SPEECH_105,-1},
{SPEECH_106,-1},
{SPEECH_107,-1},
{SPEECH_108,-1},
{SPEECH_109,-1},
{SPEECH_110,-1},
{SPEECH_111,-1},
{SPEECH_112,-1},
{SPEECH_113,-1},
{SPEECH_114,-1},
{SPEECH_115,-1},
{SPEECH_116,-1},
{SPEECH_117,-1},
{SPEECH_118,-1},
{SPEECH_119,-1},
{SPEECH_120,-1},
{SPEECH_121,-1},
{SPEECH_122,-1},
{SPEECH_123,-1},
{SPEECH_124,-1},
{SPEECH_125,-1},
{SPEECH_126,-1},
{SPEECH_127,-1},
{SPEECH_128,-1},
{SPEECH_129,-1},
{SPEECH_130,-1},
{SPEECH_131,-1},
{SPEECH_132,-1},
{SPEECH_133,-1},
{SPEECH_134,-1},
{SPEECH_135,-1},
{SPEECH_136,-1},
{SPEECH_137,-1},
{SPEECH_138,-1},
{SPEECH_139,-1},
{SPEECH_140,-1},
{SPEECH_141,-1},
{SPEECH_142,-1},
{SPEECH_143,-1},
{SPEECH_144,-1},
{SPEECH_145,-1},
{SPEECH_146,-1},
{SPEECH_147,-1},
{SPEECH_148,-1},
{SPEECH_149,-1},
{SPEECH_150,-1},
{SPEECH_151,STR__INGAME__TRIGGERSPEECH_151},
{SPEECH_152,STR__INGAME__TRIGGERSPEECH_152},
{SPEECH_153,STR__INGAME__TRIGGERSPEECH_153},
{SPEECH_154,STR__INGAME__TRIGGERSPEECH_154},
{SPEECH_155,STR__INGAME__TRIGGERSPEECH_155},
{SPEECH_156,STR__INGAME__TRIGGERSPEECH_156},
{SPEECH_157,STR__INGAME__TRIGGERSPEECH_157},
{SPEECH_158,STR__INGAME__TRIGGERSPEECH_158},
{SPEECH_159,STR__INGAME__TRIGGERSPEECH_159},
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
	{	FRM_GARY			},			// CHAR_GARY

};

class FontBank			*CConversation::s_textFontBank=NULL;
class FontBank			*CConversation::s_questionFontBank=NULL;

class CScript			*CConversation::s_registeredScripts[MAX_LOADED_SCRIPTS];
int						CConversation::s_registeredScriptIds[MAX_LOADED_SCRIPTS];
int						CConversation::s_numRegisteredScripts=0;

class CScript			*CConversation::s_currentScript=NULL;
int						CConversation::s_currentState=STATE_INACTIVE;

int						CConversation::s_currentQuestion;
int						CConversation::s_currentAnswer;
int						CConversation::s_currentSelectedAnswer;
						
int						CConversation::s_faceFrame;
int						CConversation::s_speechId;
int						CConversation::s_textPageOffset;
int						CConversation::s_maxTextPageOffset;
SpriteBank				*CConversation::s_sprites;

int						CConversation::s_ignoreNewlyPressedButtonsOnPadThisThink;


static xmPlayingId		s_playingSfxId;


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
	s_textFontBank->setColour(TEXT_R,TEXT_G,TEXT_B);

	s_questionFontBank=new ("Conversation Font") FontBank();
	s_questionFontBank->initialise(&standardFont);
	s_questionFontBank->setOt(0);

	s_currentState=STATE_INACTIVE;
	s_currentScript=NULL;

	s_sprites=new ("ConvoSprites") SpriteBank();
	s_sprites->load(SPRITES_SPRITES_SPR);

	s_ignoreNewlyPressedButtonsOnPadThisThink=false;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CConversation::shutdown()
{
	s_sprites->dump();						delete s_sprites;
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

		if(!s_ignoreNewlyPressedButtonsOnPadThisThink)
		{
			thinkText();
			thinkQuestion();
		}

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

		s_ignoreNewlyPressedButtonsOnPadThisThink=false;
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
		if(s_faceFrame!=-1)
		{
			drawSpeechBubbleBorder(TEXTBOX_X,TEXTBOX_Y,TEXTBOX_WIDTH,TEXTBOX_HEIGHT+TEXTBOX_QUESTIONHEIGHT,0,s_faceFrame);
		}
		else
		{
			drawSpeechBubbleBorder(TEXTBOX_X_FOR_NARRATOR,TEXTBOX_Y,TEXTBOX_WIDTH_FOR_NARRATOR,TEXTBOX_HEIGHT+TEXTBOX_QUESTIONHEIGHT,0,-1);
		}
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
	char	buf[1024],*pBuf;
	RECT	clipTextRegion;
	int		i;

	s_faceFrame=(s_characterIconFrames[_characterId].m_frame);
	s_speechId=_textId;

	sprintf(buf,TranslationDatabase::getString(s_speechId));
	pBuf=buf;
	clipTextRegion=getTextRegion();
	s_textFontBank->setPrintArea(clipTextRegion.x,clipTextRegion.y,clipTextRegion.w,clipTextRegion.h);
	i=-1;
	while(*pBuf)
	{
		// yes.. i know it's gay but it works..
		pBuf+=s_textFontBank->printTillEndOfLine(0,-90,pBuf);
		i++;
	}

	s_maxTextPageOffset=i/TEXTBOX_FONT_NUM_LINES_IN_BOX;
	s_textPageOffset=0;

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
		if(s_textPageOffset<s_maxTextPageOffset)
		{
			s_textPageOffset++;
			if(s_textPageOffset>s_maxTextPageOffset)
			{
				s_textPageOffset=s_maxTextPageOffset;
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
		if(s_textPageOffset>0)
		{
			s_textPageOffset--;
			if(s_textPageOffset<0)
			{
				s_textPageOffset=0;
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
			if(pad&PAD_CROSS)
			{
				s_currentAnswer=s_currentSelectedAnswer==0?ANSWER_YES:ANSWER_NO;
				s_currentQuestion=QUESTION_NONE;
			}
			else if(pad&(PAD_LEFT|PAD_RIGHT))
			{
				s_currentSelectedAnswer^=1;
			}
		}
		else if(s_currentQuestion==QUESTION_OK)
		{
			if(pad&PAD_CROSS)
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
	RECT	clipTextRegion;
	char	buf[1024],*pBuf;
	int		i;

	clipTextRegion=getTextRegion();

	sprintf(buf,TranslationDatabase::getString(s_speechId));
	pBuf=buf;
	for(i=0;i<s_textPageOffset*TEXTBOX_FONT_NUM_LINES_IN_BOX;i++)
	{
		// yes.. i know it's gay but it works..
		pBuf+=s_textFontBank->printTillEndOfLine(0,-90,pBuf);
	}
	for(i=0;i<TEXTBOX_FONT_NUM_LINES_IN_BOX&&pBuf;i++)
	{
		pBuf+=s_textFontBank->printTillEndOfLine(0,i*TEXTBOX_FONT_LINE_SPACING,pBuf);
	}

	// Render up/down button hints
	if(s_textPageOffset!=0)
	{
		s_sprites->printFT4(FRM__BUTU,clipTextRegion.x+TEXTBOX_BUTTONS_XOFF,TEXTBOX_Y+TEXTBOX_BUTTONS_YOFF,0,0,0);
	}
	if(s_textPageOffset<s_maxTextPageOffset)
	{
		s_sprites->printFT4(FRM__BUTD,clipTextRegion.x+TEXTBOX_BUTTONS_XOFF+TEXTBOX_BUTTONS_GAP,TEXTBOX_Y+TEXTBOX_BUTTONS_YOFF,0,0,0);
	}

	// Render X button hint
	int xofs;
	if(s_faceFrame!=-1)
	{
		xofs=clipTextRegion.x+TEXTBOX_BUTTONS_XOFF+TEXTBOX_WIDTH-TEXTBOX_XBUTTON_XOFFSET;
	}
	else
	{
		xofs=clipTextRegion.x+TEXTBOX_BUTTONS_XOFF+TEXTBOX_WIDTH_FOR_NARRATOR-TEXTBOX_XBUTTON_XOFFSET;
	}
	s_sprites->printFT4(FRM__BUTX,xofs,TEXTBOX_Y+TEXTBOX_BUTTONS_YOFF,0,0,0);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
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
	Purpose:	God knows..
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
					registerConversationScript(SCRIPTS_TRIGGERSPEECH_151_DAT);	// firstnet
					registerConversationScript(SCRIPTS_CH1L1_01_DAT);
					registerConversationScript(SCRIPTS_CH1L1_02_DAT);
					registerConversationScript(SCRIPTS_C1L1_CELEBRATE_DAT);
					break;
				}

				case 2:
				{
					registerConversationScript(SCRIPTS_CH1L2_01_DAT);
					registerConversationScript(SCRIPTS_CH1L2_02_DAT);
					registerConversationScript(SCRIPTS_CH1L2_03_DAT);
					registerConversationScript(SCRIPTS_C1L2_CELEBRATE_DAT);
					break;
				}

				case 3:
				{
					registerConversationScript(SCRIPTS_C1L3_CELEBRATE_DAT);
					break;
				}

				case 4:
				{
					registerConversationScript(SCRIPTS_CH1L4_01_DAT);
					registerConversationScript(SCRIPTS_CH1L4_02_DAT);
					registerConversationScript(SCRIPTS_C1L4_CELEBRATE_DAT);
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
					registerConversationScript(SCRIPTS_TRIGGERSPEECH_152_DAT);	// firstcoral
					registerConversationScript(SCRIPTS_TRIGGERSPEECH_153_DAT);	// garych2l1
					registerConversationScript(SCRIPTS_CH2L1_01_DAT);
					registerConversationScript(SCRIPTS_CH2L1_02_DAT);
					registerConversationScript(SCRIPTS_C2L1_CELEBRATE_DAT);
					registerConversationScript(SCRIPTS_TRIGGERSPEECH_028_DAT);	// gary
					break;
				}

				case 2:
				{
					registerConversationScript(SCRIPTS_TRIGGERSPEECH_155_DAT);	// bubblegeyser
					registerConversationScript(SCRIPTS_CH2L2_01_DAT);
					registerConversationScript(SCRIPTS_CH2L2_02_DAT);
					registerConversationScript(SCRIPTS_C2L2_CELEBRATE_DAT);
					break;
				}

				case 3:
				{
					registerConversationScript(SCRIPTS_TRIGGERSPEECH_157_DAT);	// firstbubble
					registerConversationScript(SCRIPTS_C2L3_CELEBRATE_DAT);
					break;
				}
				
				case 4:
				{
					registerConversationScript(SCRIPTS_C2L4_CELEBRATE_DAT);
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
					registerConversationScript(SCRIPTS_TRIGGERSPEECH_154_DAT);	// breakfloor
					registerConversationScript(SCRIPTS_CH4L3_01_DAT);
					registerConversationScript(SCRIPTS_CH4L3_02_DAT);
					registerConversationScript(SCRIPTS_C3L1_CELEBRATE_DAT);
					break;
				}

				case 2:
				{
					registerConversationScript(SCRIPTS_CH3L2_01_DAT);
					registerConversationScript(SCRIPTS_C3L2_CELEBRATE_DAT);
					break;
				}

				case 3:
				{
					registerConversationScript(SCRIPTS_TRIGGERSPEECH_158_DAT);	// usebubble
					registerConversationScript(SCRIPTS_CH3L3_01_DAT);
					registerConversationScript(SCRIPTS_CH3L3_02_DAT);
					registerConversationScript(SCRIPTS_C3L3_CELEBRATE_DAT);
					break;
				}

				case 4:
				{
					registerConversationScript(SCRIPTS_C3L4_CELEBRATE_DAT);
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
				case 1:
				{
					registerConversationScript(SCRIPTS_C4L1_CELEBRATE_DAT);
					break;
				}

				case 2:
				{
					registerConversationScript(SCRIPTS_C4L2_CELEBRATE_DAT);
					break;
				}

				case 3:
				{
					registerConversationScript(SCRIPTS_C4L3_CELEBRATE_DAT);
					break;
				}

				case 4:
				{
					registerConversationScript(SCRIPTS_C4L4_CELEBRATE_DAT);
					break;
				}

				default:
					break;
			}

			break;
		}

		case 5:
		{
			switch ( CLevel::getCurrentChapterLevel() )
			{
				case 1:
				{
					registerConversationScript(SCRIPTS_C5L1_CELEBRATE_DAT);
					break;
				}

				case 2:
				{
					registerConversationScript(SCRIPTS_C5L2_CELEBRATE_DAT);
					break;
				}

				case 3:
				{
					registerConversationScript(SCRIPTS_C5L3_CELEBRATE_DAT);
					break;
				}

				case 4:
				{
					registerConversationScript(SCRIPTS_C5L4_CELEBRATE_DAT);
					break;
				}

				case 5:
				{
					registerConversationScript(SCRIPTS_TRIGGERSPEECH_159_DAT);	// weight
					registerConversationScript(SCRIPTS_TRIGGERSPEECH_028_DAT);	// gary
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

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
RECT CConversation::getTextRegion()
{
	RECT clipRegion={TEXTBOX_X,TEXTBOX_Y-3,TEXTBOX_WIDTH,TEXTBOX_HEIGHT};

	if(s_faceFrame==-1)
	{
		// Narrators box..
		clipRegion.x=TEXTBOX_X_FOR_NARRATOR;
		clipRegion.w=TEXTBOX_WIDTH_FOR_NARRATOR;
	}

	return clipRegion;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CConversation::ignoreNewlyPressedButtonsOnPadThisThink()
{
	s_ignoreNewlyPressedButtonsOnPadThisThink=true;
}

/*===========================================================================
 end */
