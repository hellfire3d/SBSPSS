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
	} GAMEOVER_STATE;


	// Continue
	enum
	{
		CONTINUE_STROBE_SPEED=75,
		CONTINUE_FONT_SCALE=100,
		CONTINUE_FONT_BASE_SIZE=400,
		COUNTDOWN_TIME_MODIFIER=2,		// Seconds last this many times longer
	};
	void	initContinue();
	void	thinkContinue(int _frames);
	void	renderContinue();
	int		m_continueFontOffset;
	int		m_continueFontSin;
	int		m_continueTimer;

	// Game over
	void	initGameOver();
	void	thinkGameOver(int _frames);
	void	renderGameOver();


	GAMEOVER_STATE			m_state;
	class ScalableFontBank	*m_font;
	int						m_readyToExit;
	unsigned char			*m_image;

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
