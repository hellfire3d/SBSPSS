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

#ifndef	__GUI_GFRAME_H__
#include "gui\gframe.h"
#endif

#ifndef	__GUI_GSPRITE_H__
#include "gui\gsprite.h"
#endif

#ifndef	__GUI_GTEXTBOX_H__
#include "gui\gtextbox.h"
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

#ifndef __SPR_UIGFX_H__
#include <uigfx.h>
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
	{	FRM_JACK_CUSTARD	},			// CHAR_JACKCUSTARD

};

class CGUIGroupFrame	*CConversation::s_guiFrame=NULL;
class CGUISprite		*CConversation::s_guiIcon=NULL;
class CGUITextBox		*CConversation::s_guiText=NULL;

int CConversation::s_currentCharacterId=-1;
int	CConversation::s_currentTextId=-1;
int	CConversation::s_currentQuestion=QUESTION_NONE;
int CConversation::s_currentAnswer=ANSWER_NONE;
int CConversation::s_currentSelectedAnswer=0;


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CConversation::init()
{
	s_guiFrame=new ("Conversation GUI") CGUIGroupFrame();
	s_guiFrame->init(0);
	s_guiFrame->setObjectXYWH((512-FRAME_WIDTH)/2,256-FRAME_BOTTOM_OFFSET-FRAME_HEIGHT,FRAME_WIDTH,FRAME_HEIGHT);
	s_guiFrame->setOt(OT_POS);
	s_guiFrame->setFlags(CGUIObject::FLAG_DRAWBORDER);

		s_guiIcon=new("Conversation Icon") CGUISprite();
		s_guiIcon->init(s_guiFrame);
		s_guiIcon->setObjectXYWH(0,0,FRAME_HEIGHT,FRAME_HEIGHT);
		s_guiIcon->setOt(OT_POS);
		s_guiIcon->setSpriteBank(UI_UIGFX_SPR);
		s_guiIcon->setFrame(0);

		s_guiText=new("Conversation Text") CGUITextBox();
		s_guiText->init(s_guiFrame);
		s_guiText->setObjectXYWH(FRAME_HEIGHT,TEXT_BORDER,FRAME_WIDTH-FRAME_HEIGHT-TEXT_BORDER,FRAME_HEIGHT-(TEXT_BORDER*2));
		s_guiText->setOt(OT_POS);

	m_fontBank=new ("Conversation Font") FontBank();
	m_fontBank->initialise(&standardFont);
	m_fontBank->setOt(0);

	m_currentState=STATE_INACTIVE;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CConversation::shutdown()
{
	s_guiFrame->shutdown();			delete s_guiFrame;
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
		if(m_currentState==STATE_JUST_ACTIVATED)
		{
			m_currentState=STATE_ACTIVE;
		}

		s_guiFrame->think(_frames);

		thinkQuestion();

		m_currentScript->run();
		if(m_currentScript->isFinished())
		{
			m_currentState=STATE_INACTIVE;
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
	if(m_currentState==STATE_ACTIVE)
	{
		renderQuestion();
		s_guiFrame->render();
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
	// Blah blah..
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CConversation::trigger(FileEquate _feScript)
{
	ASSERT(!isActive());

	m_currentScript=new ("script") CScript();
	m_currentScript->initialise(_feScript);

	m_currentState=STATE_JUST_ACTIVATED;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int CConversation::isActive()
{
	return m_currentState!=STATE_INACTIVE;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CConversation::setCharacterAndText(int _characterId,int _textId)
{
	s_guiIcon->setFrame(s_characterIconFrames[_characterId].m_frame);
	s_guiText->setText(_textId);
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
void CConversation::renderQuestion()
{
	int	xbase,y;

	xbase=(512-FRAME_WIDTH)/2;
	y=256-FRAME_BOTTOM_OFFSET-(TEXT_BORDER/2)-(m_fontBank->getCharHeight()/2);

	switch(s_currentQuestion)
	{
		case QUESTION_OK:
			{
				m_fontBank->setColour(SELECT_TEXT_R,SELECT_TEXT_G,SELECT_TEXT_B);
				m_fontBank->setJustification(FontBank::JUST_RIGHT);
				m_fontBank->print(xbase+FRAME_WIDTH-TEXT_BORDER,y,STR__OK);
			}
			break;

		case QUESTION_YES_NO:
			{
				if(s_currentSelectedAnswer==0)
				{
					m_fontBank->setColour(SELECT_TEXT_R,SELECT_TEXT_G,SELECT_TEXT_B);
				}
				else
				{
					m_fontBank->setColour(UNSELECT_TEXT_R,UNSELECT_TEXT_G,UNSELECT_TEXT_B);
				}
				m_fontBank->setJustification(FontBank::JUST_LEFT);
				m_fontBank->print(xbase+FRAME_HEIGHT,y,STR__YES);

				if(s_currentSelectedAnswer==1)
				{
					m_fontBank->setColour(SELECT_TEXT_R,SELECT_TEXT_G,SELECT_TEXT_B);
				}
				else
				{
					m_fontBank->setColour(UNSELECT_TEXT_R,UNSELECT_TEXT_G,UNSELECT_TEXT_B);
				}
				m_fontBank->setJustification(FontBank::JUST_RIGHT);
				m_fontBank->print(xbase+FRAME_WIDTH-TEXT_BORDER,y,STR__NO);
			}
			break;

		case QUESTION_NONE:
		default:
			break;
	}
}


/*===========================================================================
 end */
