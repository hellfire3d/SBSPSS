/*=========================================================================

	scenesel.h

	Author:		PKG
	Created:
	Project:	Spongebob
	Purpose:	Scene selection thingy..

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef	__PAUL_SCENESEL_H__
#define __PAUL_SCENESEL_H__

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

class CSceneSelector : public CScene
{
public:
	void	init();
	void	shutdown();
	void	render();
	void	think(int _frames);
	int		readyToShutdown();
	char	*getSceneName()			{return"SceneSelector";}


private:
	enum
	{
		STATE_INIT,
		STATE_SELECTING,
		STATE_SELECTED,
		STATE_READY_TO_EXIT,
	};

	static class CScene	*s_sceneList[];
	static int			s_sceneCount;
	int					m_currentSelection;
	int					m_state;
	int					m_countdown;
	
	class FontBank		*m_font;


	void				selectScene(int _scene);

};


/*----------------------------------------------------------------------
	Globals
	------- */

extern CSceneSelector	SceneSelector;


/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif	/* __PAUL_SCENESEL_H__ */

/*===========================================================================
 end */
