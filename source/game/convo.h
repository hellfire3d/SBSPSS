/*=========================================================================

	convo.h

	Author:		PKG
	Created:
	Project:	Spongebob
	Purpose:

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef	__GAME_CONVO_H__
#define	__GAME_CONVO_H__

/*----------------------------------------------------------------------
	Includes
	-------- */

#ifndef _GLOBAL_HEADER_
#include "system\global.h"
#endif

#ifndef __FILE_EQUATES_H__
#include <biglump.h>
#endif


/*	Std Lib
	------- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

class CConversation
{
public:
	static void			init();
	static void			shutdown();
						
	static void			think(int _frames);
	static void			render();
						
	static void			registerConversationLevelScripts(int level);
	static void			registerConversationScript(FileEquate _feScript);
	static void			trigger(FileEquate _feScript);
	static int			isActive();
						
	static void			setCharacterAndText(int _characterId,int _textId);
	static void			setResponseOptions(int _responseOptions);
	static int			getResponse();
	static RECT			getTextRegion();

	static void			ignoreNewlyPressedButtonsOnPadThisThink();


private:
	enum
	{

		TEXTBOX_X=125,
		TEXTBOX_Y=30,
		TEXTBOX_WIDTH=357,
		TEXTBOX_HEIGHT=25,
		TEXTBOX_X_FOR_NARRATOR=512-TEXTBOX_X-TEXTBOX_WIDTH,
		TEXTBOX_WIDTH_FOR_NARRATOR=512-(TEXTBOX_X_FOR_NARRATOR*2),
		TEXTBOX_QUESTIONHEIGHT=16,
		OT_POS=0,
		TEXTBOX_FONT_LINE_SPACING=14,
		TEXTBOX_FONT_NUM_LINES_IN_BOX=2,
		TEXTBOX_BUTTONS_XOFF=0,
		TEXTBOX_BUTTONS_YOFF=TEXTBOX_HEIGHT+6,
		TEXTBOX_BUTTONS_GAP=20,
		TEXTBOX_XBUTTON_XOFFSET=50,
		// Main text
		TEXT_R=120,
		TEXT_G=115,
		TEXT_B=90,
		// Question text
		SELECT_TEXT_R=250,
		SELECT_TEXT_G=250,
		SELECT_TEXT_B=250,
		UNSELECT_TEXT_R=100,
		UNSELECT_TEXT_G=100,
		UNSELECT_TEXT_B=100,
	};

	// This order is the same as in scripts/defs/charanim.scr
	enum
	{
		CHAR_SPONGEBOB,
		CHAR_SANDY,
		CHAR_PATRICK,
		CHAR_SQUIDWARD,
		CHAR_KRABS,
		CHAR_PLANKTON,
		CHAR_MERMAIDMAN,
		CHAR_BARNACLEBOY,
		CHAR_JACKCUSTARD,
		CHAR_GARY,
		MAX_CHARS,
	};
	typedef struct
	{
		int		m_frame;
	} CHAR_ICON_FRAMES;

	// These two are as in scripts/defs/response.scr
	enum
	{
		QUESTION_NONE,
		QUESTION_OK,
		QUESTION_YES_NO,
		MAX_QUESTIONS,
	};
	enum
	{
		ANSWER_NONE,
		ANSWER_OK,
		ANSWER_YES,
		ANSWER_NO,
		MAX_ANSWERS
	};

	// State of the conversation dialog	
	enum
	{
		STATE_INACTIVE,
		STATE_JUST_ACTIVATED,
		STATE_ACTIVE,
	};

	// Maximum number of scripts per level
	enum
	{
		MAX_LOADED_SCRIPTS=10,
	};

	static void		thinkText();
	static void		thinkQuestion();
	static void		renderText();
	static void		renderQuestion();

	static void		dumpConversationScripts();


	static class FontBank		*s_textFontBank;
	static class FontBank		*s_questionFontBank;

	static CHAR_ICON_FRAMES		s_characterIconFrames[MAX_CHARS];

	static class CScript		*s_registeredScripts[MAX_LOADED_SCRIPTS];
	static int					s_registeredScriptIds[MAX_LOADED_SCRIPTS];
	static int					s_numRegisteredScripts;

	static class CScript		*s_currentScript;
	static int					s_currentState;

	static int					s_currentQuestion;
	static int					s_currentAnswer;
	static int					s_currentSelectedAnswer;

	static int					s_faceFrame;
	static int					s_speechId;
	static int					s_textPageOffset;
	static int					s_maxTextPageOffset;
	static class SpriteBank		*s_sprites;

	static int					s_ignoreNewlyPressedButtonsOnPadThisThink;

};


/*----------------------------------------------------------------------
	Globals
	------- */

/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif	/* __GAME_CONVO_H__ */

/*===========================================================================
 end */
