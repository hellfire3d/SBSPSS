/*=========================================================================

	map.h

	Author:		PKG
	Created:
	Project:	Spongebob
	Purpose:	

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__MAP_MAP_H__
#define __MAP_MAP_H__

/*----------------------------------------------------------------------
	Includes
	-------- */

#ifndef __SYSTEM_GSTATE_H__
#include "system\gstate.h"
#endif

#ifndef _GLOBAL_HEADER_
#include "system\global.h"		// Doh.. just for DVECTOR :/
#endif


/*	Std Lib
	------- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

class CMapScene: public CScene
{
public:
	void	init();
	void	shutdown();
	void	render();
	void	think(int _frames);
	int		readyToShutdown();
	char	*getSceneName()			{return"Map";}


private:
	void	copyImageToScreen(int _file,int _x,int _y,int _w,int _h);
	int		isLevelOpen(int _chapter,int _level);
	int		getSpatulaCollectedCount(int _chapter,int _level);
	int		getSpatulaAvailableCount(int _chapter,int _level);
	int		hasQuestItemBeenCollected(int _chapter,int _level);


	class FontBank		*m_font;
	int					m_readyToExit;

	char				*m_screenImage;

	class CGUIControlFrame	*m_guiFrame;
	class SpriteBank		*m_spriteBank;

	int					m_currentChapterSelection;
	int					m_currentLevelSelection;
	DVECTOR				m_pointerPos;
	DVECTOR				m_pointerTarget;
	int					m_pointerSin;

};


/*----------------------------------------------------------------------
	Globals
	------- */

extern CMapScene	MapScene;


/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif	/* __MAP_MAP_H__ */

/*===========================================================================
 end */
