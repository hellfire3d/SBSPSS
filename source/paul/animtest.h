/*=========================================================================

	animtest.h

	Author:		PKG
	Created:
	Project:	Spongebob
	Purpose:	

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__PAUL_ANIMTEST_H__
#define __PAUL_ANIMTEST_H__

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

class CAnimTestScene : public CScene
{
public:
	void	init();
	void	shutdown()				{;}
	void	render();
	void	think(int _frames);
	int		readyToShutdown()		{return false;}
	char	*getSceneName()			{return"AnimTest";}


private:
	class FontBank	*m_font;

	int				m_animNumber;
	int				m_animFrame;
	int				m_addonNumber;

};


/*----------------------------------------------------------------------
	Globals
	------- */

extern CAnimTestScene	AnimTestScene;


/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif	/* __PAUL_ANIMTEST_H__ */

/*===========================================================================
 end */
