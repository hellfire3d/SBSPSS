/*=========================================================================

	sndtest.h

	Author:		PKG
	Created:
	Project:	Spongebob
	Purpose:	Scene selection thingy..

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef	__PAUL_SNDTEST_H__
#define __PAUL_SNDTEST_H__

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

class CSceneSoundTest : public CScene
{
public:
	void	init();
	void	shutdown();
	void	render();
	void	think(int _frames);
	int		readyToShutdown();
	char	*getSceneName()			{return"SoundTest";}


private:

	class ScalableFontBank		*m_font;

};


/*----------------------------------------------------------------------
	Globals
	------- */

extern CSceneSoundTest	SoundTestScene;


/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif	/* __PAUL_SNDTEST_H__ */

/*===========================================================================
 end */
