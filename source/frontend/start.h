/*=========================================================================

	start.h

	Author:		PKG
	Created:
	Project:	Spongebob
	Purpose:

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef	__FRONTEND_START_H__
#define __FRONTEND_START_H__

/*----------------------------------------------------------------------
	Includes
	-------- */

#ifndef	__FRONTEND_FRONTEND_H__
#include "frontend\frontend.h"
#endif

#ifndef	__GUI_GREADOUT_H__
#include "gui\greadout.h"
#endif


/*	Std Lib
	------- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

class CFrontEndStart : public CFrontEndMode
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
		NO_SLOT_SELECTED=-1,
	};

	int					m_selectedSlot;
	int					m_escapeToTitles;
	int					m_shuttingDown;

	class CGUIControlFrame					*m_guiFrame;
	int										m_level;
	static int								s_levelValues[];
	static CGUITextReadout::TextReadoutData	s_levelReadoutText[];
	int										m_startGame;

};


/*----------------------------------------------------------------------
	Globals
	------- */

/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif	/* __FRONTEND_START_H__ */

/*===========================================================================
 end */
