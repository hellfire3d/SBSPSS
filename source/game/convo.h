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
	static void	init();
	static void	shutdown();

	static void	think(int _frames);
	static void	render();

	static void registerConversationLevelScripts(int level);
	static void	registerConversationScript(FileEquate _feScript);
	static void	trigger(FileEquate _feScript);
	static int	isActive();

	static void	setCharacterAndText(int _characterId,int _textId);
	static void	setResponseOptions(int _responseOptions);
	static int	getResponse();


private:
	enum
	{

		TEXTBOX_X=125,
		TEXTBOX_Y=140,
		TEXTBOX_WIDTH=357,
		TEXTBOX_HEIGHT=50,
		TEXTBOX_QUESTIONHEIGHT=16,
		TEXT_STEP_SIZE=5,
		OT_POS=0,
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
	static int					s_textOffset;
	static int					s_maxTextOffset;

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
