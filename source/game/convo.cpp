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

int	CConversation::s_currentQuestion=QUESTION_NONE;
int CConversation::s_currentAnswer=ANSWER_NONE;
int CConversation::s_currentSelectedAnswer=0;

static int	s_faceFrame=-1;
static int	s_speechId=-1;
static int	s_TextOffset=0;


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
	s_textFontBank->setPrintArea(125,140,357,60);

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
		drawSpeechBubbleBorder(125,140,357,80,0,s_faceFrame);
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
	s_TextOffset=0;
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
	if(PadGetRepeat(0)&PAD_DOWN)
	{
		s_TextOffset-=s_textFontBank->getCharHeight();
	}
	else if(PadGetRepeat(0)&PAD_UP)
	{
		s_TextOffset+=s_textFontBank->getCharHeight();
		if(s_TextOffset>0)
		{
			s_TextOffset=0;
		}
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
			if(pad&CPadConfig::getButton(CPadConfig::PAD_CFG_ACTION))
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
			if(pad&CPadConfig::getButton(CPadConfig::PAD_CFG_ACTION))
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
	RECT clipTextRegion={125,140,357,60};

	PrimFullScreen(0);
	s_textFontBank->print(0,s_TextOffset,s_speechId);
	PrimClip(&clipTextRegion,0);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CConversation::renderQuestion()
{
	int	xbase,y;

	xbase=(512-FRAME_WIDTH)/2;
	y=256-FRAME_BOTTOM_OFFSET-(TEXT_BORDER/2)-(s_questionFontBank->getCharHeight()/2);

	switch(s_currentQuestion)
	{
		case QUESTION_OK:
			{
				s_questionFontBank->setColour(SELECT_TEXT_R,SELECT_TEXT_G,SELECT_TEXT_B);
				s_questionFontBank->setJustification(FontBank::JUST_RIGHT);
				s_questionFontBank->print(xbase+FRAME_WIDTH-TEXT_BORDER,y,STR__OK);
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
				s_questionFontBank->print(xbase+FRAME_HEIGHT,y,STR__YES);

				if(s_currentSelectedAnswer==1)
				{
					s_questionFontBank->setColour(SELECT_TEXT_R,SELECT_TEXT_G,SELECT_TEXT_B);
				}
				else
				{
					s_questionFontBank->setColour(UNSELECT_TEXT_R,UNSELECT_TEXT_G,UNSELECT_TEXT_B);
				}
				s_questionFontBank->setJustification(FontBank::JUST_RIGHT);
				s_questionFontBank->print(xbase+FRAME_WIDTH-TEXT_BORDER,y,STR__NO);
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
	switch( level )
	{
		case 0:
		{
			CConversation::registerConversationScript(SCRIPTS_CH1L1_01_DAT);
			CConversation::registerConversationScript(SCRIPTS_CH1L1_02_DAT);

			// temporary
			//CConversation::registerConversationScript(SCRIPTS_CH1L2_01_DAT);
			//CConversation::registerConversationScript(SCRIPTS_CH1L2_02_DAT);
			//CConversation::registerConversationScript(SCRIPTS_CH1L2_03_DAT);
			//CConversation::registerConversationScript(SCRIPTS_CH1L4_01_DAT);
			//CConversation::registerConversationScript(SCRIPTS_CH1L4_02_DAT);
			//CConversation::registerConversationScript(SCRIPTS_CH2L1_01_DAT);
			//CConversation::registerConversationScript(SCRIPTS_CH2L1_02_DAT);
			//CConversation::registerConversationScript(SCRIPTS_CH2L2_01_DAT);
			//CConversation::registerConversationScript(SCRIPTS_CH2L2_02_DAT);
			//CConversation::registerConversationScript(SCRIPTS_CH3L3_01_DAT);
			//CConversation::registerConversationScript(SCRIPTS_CH3L3_02_DAT);
			CConversation::registerConversationScript(SCRIPTS_CH4L3_01_DAT);
			CConversation::registerConversationScript(SCRIPTS_CH4L3_02_DAT);
			break;
		}

		default:
			break;
	}
}
/*===========================================================================
 end */
