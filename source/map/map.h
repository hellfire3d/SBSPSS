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

	static void	setLevelToStartOn(int _chapter,int _level)	{s_chapterToStartOn=_chapter;s_levelToStartOn=_level;}

private:
	enum
	{
		MAP_LEVEL_WIDTH=140,
		MAP_LEVEL_HEIGHT=60,

		MAP_INSTRUCTIONS_YSTART=180,
		MAP_INSTRUCTIONS_BUTTON_Y_OFFSET=3,
		MAP_INSTRUCTIONS_TEXT_R=128,
		MAP_INSTRUCTIONS_TEXT_G=64,
		MAP_INSTRUCTIONS_TEXT_B=64,
		MAP_INSTRUCTIONS_GAP_BETWEEN_BUTTONS=5,
		MAP_INSTRUCTIONS_GAP_BETWEEN_BUTTONS_AND_TEXT=10,
		MAP_INSTRUCTIONS_Y_SPACE_BETWEEN_LINES=13,

		MAP_CHAPTER_ARROW_XOFF=90,
		MAP_CHAPTER_ARROW_Y=180,
		MAP_CHAPTER_ARROW_RGB_UNSELECTED=50,
		MAP_CHAPTER_ARROW_RGB_SELECTED=150,

		MAP_NUM_CHAPTERS=5,
		MAP_NUM_LEVELS_PER_CHAPTER=5,
	};

	enum MAP_GFX
	{
		MAP_GFX_C1_L1=0,
		MAP_GFX_C1_L2,
		MAP_GFX_C1_L3,
		MAP_GFX_C1_L4,
		MAP_GFX_C1_FAIR,

		MAP_GFX_C2_L1,
		MAP_GFX_C2_L2,
		MAP_GFX_C2_L3,
		MAP_GFX_C2_L4,
		MAP_GFX_C2_FAIR,

		MAP_GFX_C3_L1,
		MAP_GFX_C3_L2,
		MAP_GFX_C3_L3,
		MAP_GFX_C3_L4,
		MAP_GFX_C3_FAIR,

		MAP_GFX_C4_L1,
		MAP_GFX_C4_L2,
		MAP_GFX_C4_L3,
		MAP_GFX_C4_L4,
		MAP_GFX_C4_FAIR,

		MAP_GFX_C5_L1,
		MAP_GFX_C5_L2,
		MAP_GFX_C5_L3,
		MAP_GFX_C5_L4,
		MAP_GFX_C5_FAIR,

		MAP_GFX_MAX
	};
	typedef struct
	{
		MAP_GFX	m_Gfx;
		int		m_globalLevelNumber;
		int		m_spatulaOrTokenCounts;
		int		m_questItemFrame;
		int		m_kelpWorldLevel;
	} sMapLevelData;

	struct sMapLevelGfxTable
	{
		int	Filename;
		u8	*Gfx;
	};

	enum
	{
		MAP_ICON_PREVIOUS_CHAPTER,
		MAP_ICON_LEVEL_1,
		MAP_ICON_LEVEL_2,
		MAP_ICON_LEVEL_3,
		MAP_ICON_LEVEL_4,
		MAP_ICON_LEVEL_BONUS,
		MAP_ICON_NEXT_CHAPTER,

		NUM_MAP_ICONS,
	};
	typedef struct
	{
		u8		m_left,m_right,m_up,m_down;
	} sMapIconMoves;

	typedef enum
	{
		CURSOR_DIDNT_MOVE,
		CURSOR_MOVED,
		CURSOR_COULDNT_MOVE,
	} CURSOR_MOVE_STATE;



	void	renderPointer();
	void	renderInstructions();
	void	renderChapterArrows();

	
	void	generateMapScreenImage();
	void	copyImageToScreen(int _file,int _x,int _y,int _w,int _h);
	DVECTOR	getPointerTargetPosition();
	int		isLevelOpen(unsigned int _chapter,unsigned int _level);
	int		isChapterOpen(unsigned int _chapter);
	int		getSpatulaCollectedCount(unsigned int _chapter,unsigned int _level);
	int		hasQuestItemBeenCollected(unsigned int _chapter,unsigned int _level);


	class FontBank		*m_font;
	int					m_readyToExit;
	int					m_musicStarted;

	char				*m_screenImage;
	char				*m_packedBackgroundImage;

	int					m_currentChapterSelection;
	int					m_currentIconSelection;
	class CPointerIcon	*m_pointerIcon;

	static sMapLevelGfxTable	s_GfxTable[MAP_GFX_MAX];
	static sMapLevelData		s_mapLevelData[MAP_NUM_CHAPTERS][MAP_NUM_LEVELS_PER_CHAPTER];
	static DVECTOR				s_mapLevelPositions[MAP_NUM_LEVELS_PER_CHAPTER];
	static DVECTOR				s_mapPointerPositions[MAP_NUM_LEVELS_PER_CHAPTER+2];
	static const int			s_padToIconMappings[4][NUM_MAP_ICONS][NUM_MAP_ICONS];

	static int				s_chapterToStartOn;
	static int				s_levelToStartOn;

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
