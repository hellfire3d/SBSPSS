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
	char	*getSceneName()			{return"GameOver";}


private:
	class FontBank		*m_font;
	int					m_readyToExit;

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
