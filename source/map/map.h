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
	enum
	{
		MAP_PARCHMENT_WIDTH=496,
		MAP_PARCHMENT_HEIGHT=190,
		MAP_PARCHMENT_START_X=(512-MAP_PARCHMENT_WIDTH)/2,
		MAP_PARCHMENT_START_Y=20,

		MAP_LEVEL_WIDTH=140,
		MAP_LEVEL_HEIGHT=60,
		MAP_LEVEL_TOP_BORDER=20,
		MAP_LEVEL_Y_SPACING=8,
		MAP_LEVEL_X_SPACING=4,


		MAP_INSTRUCTIONS_WIDTH=416,
		MAP_INSTRUCTIONS_HEIGHT=32,
		MAP_INSTRUCTIONS_BOTTOM_OFFSET=40,
	};

	typedef struct
	{
		int		m_mapFiles[6];
		int		m_globalLevelNumbers[6];
		int		m_questItemFrames[4];
	} sMapLevelData;

	
	void	generateMapScreenImage();
	void	copyImageToScreen(int _file,int _x,int _y,int _w,int _h);
	DVECTOR	getPointerTargetPosition();
	int		isLevelOpen(int _chapter,int _level);
	int		getSpatulaCollectedCount(int _chapter,int _level);
	int		getSpatulaAvailableCount(int _chapter,int _level);
	int		hasQuestItemBeenCollected(int _chapter,int _level);


	class FontBank		*m_font;
	int					m_readyToExit;

	char				*m_screenImage;

	class SpriteBank	*m_spriteBank;

	int					m_currentChapterSelection;
	int					m_currentLevelSelection;
	DVECTOR				m_pointerPos;
	DVECTOR				m_pointerTarget;
	int					m_pointerSin;

	static sMapLevelData	s_mapLevelData[5];

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
