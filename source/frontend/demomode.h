/*=========================================================================

	demomode.h

	Author:		PKG
	Created:
	Project:	Spongebob
	Purpose:

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef	__FRONTEND_DEMOMODE_H__
#define __FRONTEND_DEMOMODE_H__

/*----------------------------------------------------------------------
	Includes
	-------- */

#ifndef	__FRONTEND_FRONTEND_H__
#include "frontend\frontend.h"
#endif

#ifndef __GAME_GAME_H__
#include "game\game.h"
#endif


/*	Std Lib
	------- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

class CDemoGameScene : public CGameScene
{
public:
	virtual void	createPlayer();
	virtual int		canPause()			{return false;}
};


class CFrontEndDemoMode : public CFrontEndMode
{
public:
	void	init();
	void	shutdown();
	void	select();
	void	unselect();
	void	render();
	void	think(int _frames);

	int		isReadyToExit();
	CFrontEndScene::FrontEndMode	getNextMode();


private:
	enum
	{
		PRESS_START_TEXT_Y=180,
		PRESS_START_TEXT_R=150,
		PRESS_START_TEXT_G=100,
		PRESS_START_TEXT_B=100,

		// Fade-out to titles this many frames before end of demo
		FADE_OUT_FRAMES=95,
	};

	class FontBank		*m_smallFont;

	int					m_shuttingDown;

	CDemoGameScene		*m_gameScene;
};


/*----------------------------------------------------------------------
	Globals
	------- */

/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif	/* __FRONTEND_DEMOMODE_H__ */

/*===========================================================================
 end */
