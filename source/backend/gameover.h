/*=========================================================================

	gameover.h

	Author:		PKG
	Created:
	Project:	Spongebob
	Purpose:	

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__BACKEND_GAMEOVER_H__
#define __BACKEND_GAMEOVER_H__

/*----------------------------------------------------------------------
	Includes
	-------- */

#ifndef __SYSTEM_GSTATE_H__
#include "system\gstate.h"
#endif


/*	Std Lib
	------- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

class CGameOverScene: public CScene
{
public:
	void	init();
	void	shutdown();
	void	render();
	void	think(int _frames);
	int		readyToShutdown();
	char	*getSceneName()			{return"GameOver/Continue";}


private:
	typedef enum
	{
		STATE__CONTINUE,
		STATE__CONTINUE_TIMED_OUT,
		STATE__GAME_OVER,
		STATE__SPEECH_BEFORE_EXITING_TO_GAME,
		STATE__EXITING_TO_GAME,
		STATE__EXITING_TO_FRONT_END,
	} GAMEOVER_STATE;


	// Continue
	enum
	{
		CONTINUE_STROBE_SPEED=75,
		CONTINUE_FONT_SCALE=100,
		CONTINUE_FONT_BASE_SIZE=400,
		COUNTDOWN_TIME_SECOND_LENGTH=100,		// Each 'second' on the countdown lasts this long
	};
	void	initContinue();
	void	thinkContinue(int _frames);
	void	renderContinue();
	int		m_continueFontOffset;
	int		m_continueFontSin;
	int		m_continueTimer;

	// Game over
	enum
	{
		GAMEOVER_FONT_SCALE=511,
		GAMEOVER_FONT_GROWSPEED=150,
		GAMEOVER_FONT_GROWSPACING=900,
		GAMEOVER_FONT_MAXSIN=1500,
		GAMEOVER_DISPLAY_TIME=60*10,
	};
	void	initGameOver();
	void	thinkGameOver(int _frames);
	void	renderGameOver();
	int		m_gameOverTimer;
	int		m_finishedGrowingText;


	GAMEOVER_STATE			m_state;
	class ScalableFontBank	*m_font;
	int						m_readyToExit;
	unsigned char			*m_image;

	int						m_hasPlayedFirstSpeech;
	int						m_hasPlayedSecondSpeech;

	class CActorGfx			*m_anotherGfx,*m_patrickGfx;
	int						m_animFrame;

};


/*----------------------------------------------------------------------
	Globals
	------- */

extern CGameOverScene	GameOverScene;


/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif	/* __BACKEND_GAMEOVER_H__ */

/*===========================================================================
 end */
