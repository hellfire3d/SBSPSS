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


/*	Std Lib
	------- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

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
		LOGO_CENTRE_Y=74,
		GAME_TITLE_TEXT_CENTRE_Y=100,
		GAME_TITLE_TEXT_R=128,
		GAME_TITLE_TEXT_G=128,
		GAME_TITLE_TEXT_B=128,

		PRESS_START_TEXT_Y=180,
		PRESS_START_TEXT_R=150,
		PRESS_START_TEXT_G=100,
		PRESS_START_TEXT_B=100,
	};

	class FontBank		*m_smallFont;

	int					m_shuttingDown;
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
