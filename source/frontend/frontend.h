/*=========================================================================

	frontend.h

	Author:		PKG
	Created:
	Project:	Spongebob
	Purpose:

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef	__FRONTEND_FRONTENTD_H__

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

class CFrontEndScene : public CScene
{
public:
	void	init();
	void	shutdown();
	void	render();
	void	think(int _frames);
	int		readyToShutdown();
	char	*getSceneName()			{return"FrontEndScene";}


private:
	class SpriteBank		*m_sprites;
};


/*----------------------------------------------------------------------
	Globals
	------- */

/*----------------------------------------------------------------------
	Functions
	--------- */

extern CFrontEndScene	FrontEndScene;


/*---------------------------------------------------------------------- */

#endif	/* __FRONTEND_FRONTENTD_H__ */

/*===========================================================================
 end */
