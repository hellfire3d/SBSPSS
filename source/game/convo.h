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
	void		init();
	void		shutdown();

	void		think(int _frames);
	void		render();

	void		registerConversationScript(FileEquate _feScript);

	void		trigger(FileEquate _feScript);
	int			isActive();

	static void	setCharacterAndText(int _characterId,int _textId);
	static void	setResponseOptions(int _responseOptions);
	static int	getResponse();


private:
	enum
	{
		FRAME_WIDTH=400,
		FRAME_HEIGHT=90,
		FRAME_BOTTOM_OFFSET=40,
		TEXT_BORDER=20,
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


	void		thinkQuestion();
	void		renderQuestion();


	// PKG - These don't need to be static..
	// Make the sprites/text readouts maybe?
	static class CGUIGroupFrame	*s_guiFrame;
	static class CGUISprite		*s_guiIcon;
	static class CGUITextBox	*s_guiText;

	class FontBank				*m_fontBank;

	static CHAR_ICON_FRAMES	s_characterIconFrames[MAX_CHARS];

	class CScript			*m_currentScript;
	int						m_currentState;

	static int				s_currentCharacterId;
	static int				s_currentTextId;
	static int				s_currentQuestion;
	static int				s_currentAnswer;
	static int				s_currentSelectedAnswer;
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
