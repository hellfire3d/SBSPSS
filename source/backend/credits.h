/*=========================================================================

	credits.h

	Author:		PKG
	Created:
	Project:	Spongebob
	Purpose:

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef	__FRONTEND_CREDITS_H__
#define __FRONTEND_CREDITS_H__

/*----------------------------------------------------------------------
	Includes
	-------- */

#ifndef __SYSTEM_GSTATE_H__
#include "system\gstate.h"
#endif

#ifndef _GLOBAL_HEADER_
#include "system\global.h"
#endif


/*	Std Lib
	------- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

class CCreditsScene : public CScene
{
public:
	void	init();
	void	shutdown();
	void	render();
	void	think(int _frames);
	int		readyToShutdown();
	char	*getSceneName()			{return"Credits";}

private:
	void	renderCreditsListTillEndOfPage(u16 *_list);


	class ScalableFontBank		*m_font;
	class CScrollyBackground	*m_background1;
	class CScrollyBackground	*m_background2;
	u8							*m_image;
	u16							*m_currentPage;
	u16							*m_lastPage;
	int							m_frame;
								
								
	int							m_shuttingDown;

};


/*----------------------------------------------------------------------
	Globals
	------- */

extern CCreditsScene	CreditsScene;


/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif	/* __FRONTEND_CREDITS_H__ */

/*===========================================================================
 end */
