/*=========================================================================

	map.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose:	

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/


/*----------------------------------------------------------------------
	Includes
	-------- */

#include "map\map.h"

#ifndef __GFX_FONT_H__
#include "gfx\font.h"
#endif

#ifndef __PAD_PADS_H__
#include "pad\pads.h"
#endif

#ifndef __PRIM_HEADER__
#include "gfx\prim.h"
#endif

#ifndef __GAME_GAME_H__
#include "game\game.h"
#endif

#ifndef __SAVE_SAVE_H__
#include "save\save.h"
#endif

#ifndef	__GFX_FADER_H__
#include "gfx\fader.h"
#endif

#ifndef __VID_HEADER_
#include "system\vid.h"
#endif

#ifndef _FILEIO_HEADER_
#include "fileio\fileio.h"
#endif

#ifndef __MATHTABLE_HEADER__
#include "utils\mathtab.h"
#endif

#ifndef	__GAME_GAMESLOT_H__
#include "game\gameslot.h"
#endif

#ifndef	__SOUND_SOUND_H__
#include "sound\sound.h"
#endif

#ifndef	__GUI_POINTER_H__
#include "gui\pointer.h"
#endif

#ifndef	__FMA_FMA_H__
#include "fma\fma.h"
#endif

#include	"utils\lznp.h"
	
/*	Std Lib
	------- */

/*	Data
	---- */

#ifndef __STRING_ENUMS__
#include <trans.h>
#endif

#ifndef __SPR_SPRITES_H__
#include <sprites.h>
#endif

// Level data include files:
#include <CHAPTER01_LEVEL01_INF.h>
#include <CHAPTER01_LEVEL02_INF.h>
#include <CHAPTER01_LEVEL03_INF.h>
#include <CHAPTER01_LEVEL04_INF.h>
#include <CHAPTER02_LEVEL01_INF.h>
#include <CHAPTER02_LEVEL02_INF.h>
#include <CHAPTER02_LEVEL03_INF.h>
#include <CHAPTER02_LEVEL04_INF.h>
#include <CHAPTER03_LEVEL01_INF.h>
#include <CHAPTER03_LEVEL02_INF.h>
#include <CHAPTER03_LEVEL03_INF.h>
#include <CHAPTER03_LEVEL04_INF.h>
#include <CHAPTER04_LEVEL01_INF.h>
#include <CHAPTER04_LEVEL02_INF.h>
#include <CHAPTER04_LEVEL03_INF.h>
#include <CHAPTER04_LEVEL04_INF.h>
#include <CHAPTER05_LEVEL01_INF.h>
#include <CHAPTER05_LEVEL02_INF.h>
#include <CHAPTER05_LEVEL03_INF.h>
#include <CHAPTER05_LEVEL04_INF.h>
#include <CHAPTER06_LEVEL01_INF.h>
#include <CHAPTER06_LEVEL02_INF.h>
#include <CHAPTER06_LEVEL03_INF.h>
#include <CHAPTER06_LEVEL04_INF.h>
#include <CHAPTER06_LEVEL05_INF.h>


/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

/*----------------------------------------------------------------------
	Function Prototypes
	------------------- */

/*----------------------------------------------------------------------
	Vars
	---- */

CMapScene	MapScene;

CMapScene::sMapLevelGfxTable	CMapScene::s_GfxTable[MAP_GFX_MAX]=
{
	{MAP_C1_L1_GFX,0},	
	{MAP_C1_L2_GFX,0},	
	{MAP_C1_L3_GFX,0},	
	{MAP_C1_L4_GFX,0},	
	{MAP_C1_FAIR_GFX,0},

	{MAP_C2_L1_GFX,0},	
	{MAP_C2_L2_GFX,0},	
	{MAP_C2_L3_GFX,0},	
	{MAP_C2_L4_GFX,0},	
	{MAP_C2_FAIR_GFX,0},

	{MAP_C3_L1_GFX,0},	
	{MAP_C3_L2_GFX,0},	
	{MAP_C3_L3_GFX,0},	
	{MAP_C3_L4_GFX,0},	
	{MAP_C3_FAIR_GFX,0},

	{MAP_C4_L1_GFX,0},	
	{MAP_C4_L2_GFX,0},	
	{MAP_C4_L3_GFX,0},	
	{MAP_C4_L4_GFX,0},	
	{MAP_C4_FAIR_GFX,0},

	{MAP_C5_L1_GFX,0},	
	{MAP_C5_L2_GFX,0},	
	{MAP_C5_L3_GFX,0},	
	{MAP_C5_L4_GFX,0},	
	{MAP_C5_FAIR_GFX,0},
};


CMapScene::sMapLevelData	CMapScene::s_mapLevelData[MAP_NUM_CHAPTERS][MAP_NUM_LEVELS_PER_CHAPTER]=
{
	//		mapFile				Gfx,	globalLevelNumber	spatulaOrTokenCounts								questItemFrame			kelpWorldLevel
	{
		{	MAP_GFX_C1_L1,		0,					CHAPTER01_LEVEL01_INF_TOTAL_ITEM_GOLDEN_SPATULA,	FRM__C1_L1_QUEST_ITEM,	false	},
		{	MAP_GFX_C1_L2,		1,					CHAPTER01_LEVEL02_INF_TOTAL_ITEM_GOLDEN_SPATULA,	FRM__C1_L2_QUEST_ITEM,	false	},
		{	MAP_GFX_C1_L3,		2,					CHAPTER01_LEVEL03_INF_TOTAL_ITEM_GOLDEN_SPATULA,	FRM__C1_L3_QUEST_ITEM,	false	},
		{	MAP_GFX_C1_L4,		3,					CHAPTER01_LEVEL04_INF_TOTAL_ITEM_GOLDEN_SPATULA,	FRM__C1_L4_QUEST_ITEM,	false	},
		{	MAP_GFX_C1_FAIR,	4,					CHAPTER06_LEVEL01_INF_TOTAL_ITEM_KELP_TOKEN,		-1,						true	},
	},
	{
		{	MAP_GFX_C2_L1,		5,					CHAPTER02_LEVEL01_INF_TOTAL_ITEM_GOLDEN_SPATULA,	FRM__C2_L1_QUEST_ITEM,	false	},
		{	MAP_GFX_C2_L2,		6,					CHAPTER02_LEVEL02_INF_TOTAL_ITEM_GOLDEN_SPATULA,	FRM__C2_L2_QUEST_ITEM,	false	},
		{	MAP_GFX_C2_L3,		7,					CHAPTER02_LEVEL03_INF_TOTAL_ITEM_GOLDEN_SPATULA,	FRM__C2_L3_QUEST_ITEM,	false	},
		{	MAP_GFX_C2_L4,		8,					CHAPTER02_LEVEL04_INF_TOTAL_ITEM_GOLDEN_SPATULA,	FRM__C2_L4_QUEST_ITEM,	false	},
		{	MAP_GFX_C2_FAIR,	9,					CHAPTER06_LEVEL02_INF_TOTAL_ITEM_KELP_TOKEN,		-1,						true	},
	},
	{
		{	MAP_GFX_C3_L1,		10,					CHAPTER03_LEVEL01_INF_TOTAL_ITEM_GOLDEN_SPATULA,	FRM__C3_L1_QUEST_ITEM,	false	},
		{	MAP_GFX_C3_L2,		11,					CHAPTER03_LEVEL02_INF_TOTAL_ITEM_GOLDEN_SPATULA,	FRM__C3_L2_QUEST_ITEM,	false	},
		{	MAP_GFX_C3_L3,		12,					CHAPTER03_LEVEL03_INF_TOTAL_ITEM_GOLDEN_SPATULA,	FRM__C3_L3_QUEST_ITEM,	false	},
		{	MAP_GFX_C3_L4,		13,					CHAPTER03_LEVEL04_INF_TOTAL_ITEM_GOLDEN_SPATULA,	FRM__C3_L4_QUEST_ITEM,	false	},
		{	MAP_GFX_C3_FAIR,	14,					CHAPTER06_LEVEL03_INF_TOTAL_ITEM_KELP_TOKEN,		-1,						true	},
	},
	{
		{	MAP_GFX_C4_L1,		15,					CHAPTER04_LEVEL01_INF_TOTAL_ITEM_GOLDEN_SPATULA,	FRM__C4_QUEST_ITEM_1,	false	},
		{	MAP_GFX_C4_L2,		16,					CHAPTER04_LEVEL02_INF_TOTAL_ITEM_GOLDEN_SPATULA,	FRM__C4_QUEST_ITEM_1,	false	},
		{	MAP_GFX_C4_L3,		17,					CHAPTER04_LEVEL03_INF_TOTAL_ITEM_GOLDEN_SPATULA,	FRM__C4_QUEST_ITEM_1,	false	},
		{	MAP_GFX_C4_L4,		18,					CHAPTER04_LEVEL04_INF_TOTAL_ITEM_GOLDEN_SPATULA,	FRM__C4_QUEST_ITEM_1,	false	},
		{	MAP_GFX_C4_FAIR,	19,					CHAPTER06_LEVEL04_INF_TOTAL_ITEM_KELP_TOKEN,		-1,						true	},
	},
	{
		{	MAP_GFX_C5_L1,		20,					CHAPTER05_LEVEL01_INF_TOTAL_ITEM_GOLDEN_SPATULA,	FRM__C5_L1_QUEST_ITEM,	false	},
		{	MAP_GFX_C5_L2,		21,					CHAPTER05_LEVEL02_INF_TOTAL_ITEM_GOLDEN_SPATULA,	FRM__C5_L2_QUEST_ITEM,	false	},
		{	MAP_GFX_C5_L3,		22,					CHAPTER05_LEVEL03_INF_TOTAL_ITEM_GOLDEN_SPATULA,	FRM__C5_L3_QUEST_ITEM,	false	},
		{	MAP_GFX_C5_L4,		23,					CHAPTER05_LEVEL04_INF_TOTAL_ITEM_GOLDEN_SPATULA,	FRM__C5_L4_QUEST_ITEM,	false	},
		{	MAP_GFX_C5_FAIR,	24,					CHAPTER06_LEVEL05_INF_TOTAL_ITEM_KELP_TOKEN,		-1,						true	},
	},
};


// Positions of map gfx on the screen
DVECTOR						CMapScene::s_mapLevelPositions[CMapScene::MAP_NUM_LEVELS_PER_CHAPTER]=
{
	{	42,49						},		// MAP_ICON_LEVEL_1
	{	186,49						},		// MAP_ICON_LEVEL_2
	{	330,49						},		// MAP_ICON_LEVEL_3
	{	42,113						},		// MAP_ICON_LEVEL_4
	{	330,113						},		// MAP_ICON_LEVEL_BONUS
};

// Cursor positions for all icons
DVECTOR						CMapScene::s_mapPointerPositions[CMapScene::MAP_NUM_LEVELS_PER_CHAPTER+2]=
{
	{	MAP_CHAPTER_ARROW_XOFF,MAP_CHAPTER_ARROW_Y+25		},		// MAP_ICON_PREVIOUS_CHAPTER
	{	42+(MAP_LEVEL_WIDTH/2),49+MAP_LEVEL_HEIGHT			},		// MAP_ICON_LEVEL_1
	{	186+(MAP_LEVEL_WIDTH/2),49+MAP_LEVEL_HEIGHT			},		// MAP_ICON_LEVEL_2
	{	330+(MAP_LEVEL_WIDTH/2),49+MAP_LEVEL_HEIGHT			},		// MAP_ICON_LEVEL_3
	{	42+(MAP_LEVEL_WIDTH/2),113+MAP_LEVEL_HEIGHT			},		// MAP_ICON_LEVEL_4
	{	330+(MAP_LEVEL_WIDTH/2),113+MAP_LEVEL_HEIGHT		},		// MAP_ICON_LEVEL_BONUS
	{	512-MAP_CHAPTER_ARROW_XOFF,MAP_CHAPTER_ARROW_Y+25	},		// MAP_ICON_NEXT_CHAPTER
};


int							CMapScene::s_chapterToStartOn=0;
int							CMapScene::s_levelToStartOn=0;


extern int s_globalLevelSelectThing;


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CMapScene::init()
{
	CLevel::DisplayLoadingScreen();
	m_font=new ("map screen font") FontBank();
	m_font->initialise(&standardFont);
	m_font->setOt(10);
	m_font->setJustification(FontBank::JUST_LEFT);

	m_currentChapterSelection=s_chapterToStartOn;

	m_screenImage=MemAlloc(512*256*2,"MapScreen");
	ASSERT(m_screenImage);
	m_packedBackgroundImage=(char*)CFileIO::loadFile(MAP_MAP_BACKGROUND_GFX);
	ASSERT(m_packedBackgroundImage);

// Load level Gfx
	for (int i=0; i<MAP_GFX_MAX; i++)
	{
		s_GfxTable[i].Gfx=(u8*)CFileIO::loadFile((FileEquate)s_GfxTable[i].Filename,"MapLevelGfx");
	}

	generateMapScreenImage();
	m_currentIconSelection=s_levelToStartOn+1;

	m_pointerIcon=new ("MapPointer") CPointerIcon();
	m_pointerIcon->snapToTarget(getPointerTargetPosition());
	
	SetScreenImage((u8*)m_screenImage);

	m_readyToExit=false;
	CFader::setFadingIn();

	CSoundMediator::setSong(CSoundMediator::SONG_MAPSCREEN);
	m_musicStarted=false;
	StopLoad();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CMapScene::shutdown()
{
	CSoundMediator::dumpSong();

	ClearScreenImage();
// Free level Gfx
	for (int i=0; i<MAP_GFX_MAX; i++)
	{
		MemFree(s_GfxTable[i].Gfx);
	}

	delete m_pointerIcon;
	MemFree(m_packedBackgroundImage);
	MemFree(m_screenImage);
	m_font->dump();				delete m_font;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CMapScene::render()
{
	SpriteBank		*sb;
	sMapLevelData	*level;
	int				i;
	char			buf[100];
	
	m_pointerIcon->render();

	sb=CGameScene::getSpriteBank();
	level=&s_mapLevelData[m_currentChapterSelection][0];

	// Render spatula/kelp counts and quest items
	for(i=0;i<MAP_NUM_LEVELS_PER_CHAPTER;i++)
	{
		if(isLevelOpen(m_currentChapterSelection,i))
		{
			DVECTOR			pos;

			pos=s_mapLevelPositions[i];

			if(!level->m_kelpWorldLevel)
			{
				sFrameHdr		*fh;
				POLY_FT4		*ft4;

				// Normal level
				m_font->setColour(253,251,67);
				sprintf(buf,"%d/%d",CGameSlotManager::getSlotData()->getSpatulaCollectedCount(m_currentChapterSelection,i),level->m_spatulaOrTokenCounts);
				fh=sb->getFrameHeader(level->m_questItemFrame);
				ft4=sb->printFT4(fh,pos.vx+MAP_LEVEL_WIDTH-fh->W,pos.vy+MAP_LEVEL_HEIGHT-fh->H,0,0,10);
				if(!hasQuestItemBeenCollected(m_currentChapterSelection,i))
				{
					setRGB0(ft4,50,50,50);
				}
			}
			else
			{
				// Bonuse level
				m_font->setColour(67,251,67);
				sprintf(buf,"%d/%d",CGameSlotManager::getSlotData()->getKelpTokenCollectedCount(m_currentChapterSelection,i),level->m_spatulaOrTokenCounts);
			}

			m_font->print(pos.vx,pos.vy,buf);
		}
		level++;
	}

	renderInstructions();
	renderChapterArrows();


sprintf(buf,"Chapter %d",m_currentChapterSelection+1);
m_font->setColour(0,255,0);
m_font->print(24,24,buf);
m_font->setColour(0,0,0);
m_font->print(25,25,buf);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CMapScene::renderInstructions()
{
	// Instructions
	SpriteBank		*sb;
	sFrameHdr		*fh1,*fh2;
	int				width;
	int				x,y;
	int				xText=0;


	sb=CGameScene::getSpriteBank();
	m_font->setColour(MAP_INSTRUCTIONS_TEXT_R,MAP_INSTRUCTIONS_TEXT_G,MAP_INSTRUCTIONS_TEXT_B);

	y=MAP_INSTRUCTIONS_YSTART;

	switch(m_currentIconSelection)
	{
		case MAP_ICON_PREVIOUS_CHAPTER:
			xText=STR__MAP_SCREEN__X_FOR_PREVIOUS_CHAPTER;
			break;
		case MAP_ICON_LEVEL_1:
		case MAP_ICON_LEVEL_2:
		case MAP_ICON_LEVEL_3:
		case MAP_ICON_LEVEL_4:
		case MAP_ICON_LEVEL_BONUS:
			xText=STR__MAP_SCREEN__X_TO_START;
			break;
		case MAP_ICON_NEXT_CHAPTER:
			xText=STR__MAP_SCREEN__X_FOR_NEXT_CHAPTER;
			break;
	}
	
	y+=MAP_INSTRUCTIONS_Y_SPACE_BETWEEN_LINES;
	fh1=sb->getFrameHeader(FRM__BUTX);
	width=fh1->W+MAP_INSTRUCTIONS_GAP_BETWEEN_BUTTONS_AND_TEXT+m_font->getStringWidth(xText);
	x=256-(width/2);
	sb->printFT4(fh1,x,y+MAP_INSTRUCTIONS_BUTTON_Y_OFFSET,0,0,0);
	x+=fh1->W+MAP_INSTRUCTIONS_GAP_BETWEEN_BUTTONS_AND_TEXT;
	m_font->print(x,y,xText);

	y+=MAP_INSTRUCTIONS_Y_SPACE_BETWEEN_LINES;
	fh1=sb->getFrameHeader(FRM__BUTT);
	width=fh1->W+MAP_INSTRUCTIONS_GAP_BETWEEN_BUTTONS_AND_TEXT+m_font->getStringWidth(STR__MAP_SCREEN__TRIANGLE_TO_SAVE);
	x=256-(width/2);
	sb->printFT4(fh1,x,y+MAP_INSTRUCTIONS_BUTTON_Y_OFFSET,0,0,0);
	x+=fh1->W+MAP_INSTRUCTIONS_GAP_BETWEEN_BUTTONS_AND_TEXT;
	m_font->print(x,y,STR__MAP_SCREEN__TRIANGLE_TO_SAVE);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CMapScene::renderChapterArrows()
{
	SpriteBank	*sb;
	sFrameHdr	*fh;
	int			x,y;
	POLY_FT4	*ft4;

	sb=CGameScene::getSpriteBank();
	fh=sb->getFrameHeader(FRM__MAP_ARROW);

	if(m_currentChapterSelection>0&&isChapterOpen(m_currentChapterSelection-1))
	{
		int	rgb;
		rgb=m_currentIconSelection==MAP_ICON_PREVIOUS_CHAPTER?MAP_CHAPTER_ARROW_RGB_SELECTED:MAP_CHAPTER_ARROW_RGB_UNSELECTED;
		x=MAP_CHAPTER_ARROW_XOFF-(fh->W/2);
		y=MAP_CHAPTER_ARROW_Y;
		ft4=sb->printFT4(fh,x,y,0,0,0);
		setRGB0(ft4,rgb,rgb,rgb);
	}
	if(m_currentChapterSelection<5-1&&isChapterOpen(m_currentChapterSelection+1))
	{
		int	rgb;
		rgb=m_currentIconSelection==MAP_ICON_NEXT_CHAPTER?MAP_CHAPTER_ARROW_RGB_SELECTED:MAP_CHAPTER_ARROW_RGB_UNSELECTED;
		x=512-MAP_CHAPTER_ARROW_XOFF-(fh->W/2)+fh->W;
		y=MAP_CHAPTER_ARROW_Y;
		ft4=sb->printFT4(fh,x,y,1,0,0);
		setRGB0(ft4,rgb,rgb,rgb);
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
const int	CMapScene::s_padToIconMappings[4][NUM_MAP_ICONS][NUM_MAP_ICONS]=
{
	// UP
	{
		// MAP_ICON_PREVIOUS_CHAPTER
		{	MAP_ICON_LEVEL_4,MAP_ICON_LEVEL_BONUS,MAP_ICON_LEVEL_1,MAP_ICON_LEVEL_2,MAP_ICON_LEVEL_3,MAP_ICON_PREVIOUS_CHAPTER	},
		// MAP_ICON_LEVEL_1
		{	MAP_ICON_LEVEL_1	},
		// MAP_ICON_LEVEL_2
		{	MAP_ICON_LEVEL_2	},
		// MAP_ICON_LEVEL_3
		{	MAP_ICON_LEVEL_3	},
		// MAP_ICON_LEVEL_4
		{	MAP_ICON_LEVEL_1,MAP_ICON_LEVEL_4	},
		// MAP_ICON_LEVEL_BONUS
		{	MAP_ICON_LEVEL_3,MAP_ICON_LEVEL_2,MAP_ICON_LEVEL_1,MAP_ICON_LEVEL_BONUS	},
		// MAP_ICON_NEXT_CHAPTER
		{	MAP_ICON_LEVEL_BONUS,MAP_ICON_LEVEL_4,MAP_ICON_LEVEL_3,MAP_ICON_LEVEL_2,MAP_ICON_LEVEL_1,MAP_ICON_NEXT_CHAPTER	},
	},
	// DOWN
	{
		// MAP_ICON_PREVIOUS_CHAPTER
		{	MAP_ICON_PREVIOUS_CHAPTER	},
		// MAP_ICON_LEVEL_1
		{	MAP_ICON_LEVEL_4,MAP_ICON_LEVEL_BONUS,MAP_ICON_PREVIOUS_CHAPTER,MAP_ICON_NEXT_CHAPTER,MAP_ICON_LEVEL_1	},
		// MAP_ICON_LEVEL_2
		{	MAP_ICON_LEVEL_4,MAP_ICON_LEVEL_BONUS,MAP_ICON_PREVIOUS_CHAPTER,MAP_ICON_NEXT_CHAPTER,MAP_ICON_LEVEL_2	},
		// MAP_ICON_LEVEL_3
		{	MAP_ICON_LEVEL_BONUS,MAP_ICON_LEVEL_4,MAP_ICON_NEXT_CHAPTER,MAP_ICON_PREVIOUS_CHAPTER,MAP_ICON_LEVEL_3	},
		// MAP_ICON_LEVEL_4
		{	MAP_ICON_PREVIOUS_CHAPTER,MAP_ICON_NEXT_CHAPTER,MAP_ICON_LEVEL_4	},
		// MAP_ICON_LEVEL_BONUS
		{	MAP_ICON_NEXT_CHAPTER,MAP_ICON_PREVIOUS_CHAPTER,MAP_ICON_LEVEL_BONUS	},
		// MAP_ICON_NEXT_CHAPTER
		{	MAP_ICON_NEXT_CHAPTER	},
	},
	// LEFT
	{
		// MAP_ICON_PREVIOUS_CHAPTER
		{	MAP_ICON_LEVEL_BONUS,MAP_ICON_LEVEL_4,MAP_ICON_LEVEL_3,MAP_ICON_LEVEL_2,MAP_ICON_LEVEL_1,MAP_ICON_NEXT_CHAPTER,MAP_ICON_PREVIOUS_CHAPTER	},
		// MAP_ICON_LEVEL_1
		{	MAP_ICON_NEXT_CHAPTER,MAP_ICON_PREVIOUS_CHAPTER,MAP_ICON_LEVEL_BONUS,MAP_ICON_LEVEL_4,MAP_ICON_LEVEL_3,MAP_ICON_LEVEL_2,MAP_ICON_LEVEL_1	},
		// MAP_ICON_LEVEL_2
		{	MAP_ICON_LEVEL_1,MAP_ICON_NEXT_CHAPTER,MAP_ICON_PREVIOUS_CHAPTER,MAP_ICON_LEVEL_BONUS,MAP_ICON_LEVEL_4,MAP_ICON_LEVEL_3,MAP_ICON_LEVEL_2	},
		// MAP_ICON_LEVEL_3
		{	MAP_ICON_LEVEL_2,MAP_ICON_LEVEL_1,MAP_ICON_NEXT_CHAPTER,MAP_ICON_PREVIOUS_CHAPTER,MAP_ICON_LEVEL_BONUS,MAP_ICON_LEVEL_4,MAP_ICON_LEVEL_3	},
		// MAP_ICON_LEVEL_4
		{	MAP_ICON_LEVEL_3,MAP_ICON_LEVEL_2,MAP_ICON_LEVEL_1,MAP_ICON_NEXT_CHAPTER,MAP_ICON_PREVIOUS_CHAPTER,MAP_ICON_LEVEL_BONUS,MAP_ICON_LEVEL_4	},
		// MAP_ICON_LEVEL_BONUS
		{	MAP_ICON_LEVEL_4,MAP_ICON_LEVEL_3,MAP_ICON_LEVEL_2,MAP_ICON_LEVEL_1,MAP_ICON_NEXT_CHAPTER,MAP_ICON_PREVIOUS_CHAPTER,MAP_ICON_LEVEL_BONUS	},
		// MAP_ICON_NEXT_CHAPTER
		{	MAP_ICON_PREVIOUS_CHAPTER,MAP_ICON_LEVEL_BONUS,MAP_ICON_LEVEL_4,MAP_ICON_LEVEL_3,MAP_ICON_LEVEL_2,MAP_ICON_LEVEL_1,MAP_ICON_NEXT_CHAPTER	},
	},
	// RIGHT
	{
		// MAP_ICON_PREVIOUS_CHAPTER
		{	MAP_ICON_NEXT_CHAPTER,MAP_ICON_LEVEL_1,MAP_ICON_LEVEL_2,MAP_ICON_LEVEL_3,MAP_ICON_LEVEL_4,MAP_ICON_LEVEL_BONUS,MAP_ICON_PREVIOUS_CHAPTER	},
		// MAP_ICON_LEVEL_1
		{	MAP_ICON_LEVEL_2,MAP_ICON_LEVEL_3,MAP_ICON_LEVEL_4,MAP_ICON_LEVEL_BONUS,MAP_ICON_PREVIOUS_CHAPTER,MAP_ICON_NEXT_CHAPTER,MAP_ICON_LEVEL_1	},
		// MAP_ICON_LEVEL_2
		{	MAP_ICON_LEVEL_3,MAP_ICON_LEVEL_4,MAP_ICON_LEVEL_BONUS,MAP_ICON_PREVIOUS_CHAPTER,MAP_ICON_NEXT_CHAPTER,MAP_ICON_LEVEL_1,MAP_ICON_LEVEL_2	},
		// MAP_ICON_LEVEL_3
		{	MAP_ICON_LEVEL_4,MAP_ICON_LEVEL_BONUS,MAP_ICON_PREVIOUS_CHAPTER,MAP_ICON_NEXT_CHAPTER,MAP_ICON_LEVEL_1,MAP_ICON_LEVEL_2,MAP_ICON_LEVEL_3	},
		// MAP_ICON_LEVEL_4
		{	MAP_ICON_LEVEL_BONUS,MAP_ICON_PREVIOUS_CHAPTER,MAP_ICON_NEXT_CHAPTER,MAP_ICON_LEVEL_1,MAP_ICON_LEVEL_2,MAP_ICON_LEVEL_3,MAP_ICON_LEVEL_4	},
		// MAP_ICON_LEVEL_BONUS
		{	MAP_ICON_PREVIOUS_CHAPTER,MAP_ICON_NEXT_CHAPTER,MAP_ICON_LEVEL_1,MAP_ICON_LEVEL_2,MAP_ICON_LEVEL_3,MAP_ICON_LEVEL_4,MAP_ICON_LEVEL_BONUS	},
		// MAP_ICON_NEXT_CHAPTER
		{	MAP_ICON_LEVEL_1,MAP_ICON_LEVEL_2,MAP_ICON_LEVEL_3,MAP_ICON_LEVEL_4,MAP_ICON_LEVEL_BONUS,MAP_ICON_PREVIOUS_CHAPTER,MAP_ICON_NEXT_CHAPTER	},
	},
};

void CMapScene::think(int _frames)
{
	if(!CFader::isFading()&&!m_readyToExit)
	{
		CURSOR_MOVE_STATE	cursor=CURSOR_DIDNT_MOVE;

		if(m_musicStarted==false)
		{
			m_musicStarted=true;
			CSoundMediator::playSong();
		}
		
		int	pad=PadGetDown(0);
#ifdef __VERSION_DEBUG__
		if(pad&PAD_L1)
		{
			CGameSlotManager::getSlotData()->debugCheatOpenAllLevels();
			generateMapScreenImage();
		}
#endif

		// Move cursor in the gayest way possible..
		// Hahahaha! It's like a spiders nest! (pkg and proud of it)
		const int	*mapBase=NULL;

		if(pad&PAD_UP)mapBase=&s_padToIconMappings[0][m_currentIconSelection][0];
		else if(pad&PAD_DOWN)mapBase=&s_padToIconMappings[1][m_currentIconSelection][0];
		else if(pad&PAD_LEFT)mapBase=&s_padToIconMappings[2][m_currentIconSelection][0];
		else if(pad&PAD_RIGHT)mapBase=&s_padToIconMappings[3][m_currentIconSelection][0];

		if(mapBase)
		{
			int	i,level,isOpen;
			for(i=0;i<NUM_MAP_ICONS;i++)
			{
				level=*mapBase++;
				if(level==m_currentIconSelection)
				{
					CSoundMediator::playSfx(CSoundMediator::SFX_FRONT_END__ERROR,false,true);
					break;
				}
			
				isOpen=false;
				switch(level)
				{
					case MAP_ICON_PREVIOUS_CHAPTER:
						if(m_currentChapterSelection>0&&isChapterOpen(m_currentChapterSelection-1))isOpen=true;
						break;
					case MAP_ICON_LEVEL_1:
					case MAP_ICON_LEVEL_2:
					case MAP_ICON_LEVEL_3:
					case MAP_ICON_LEVEL_4:
					case MAP_ICON_LEVEL_BONUS:
						if(isLevelOpen(m_currentChapterSelection,level-1))isOpen=true;
						break;
					case MAP_ICON_NEXT_CHAPTER:
						if(m_currentChapterSelection<5-1&&isChapterOpen(m_currentChapterSelection+1))isOpen=true;
						break;
				}

				if(isOpen)
				{
					m_currentIconSelection=level;
					m_pointerIcon->setTarget(getPointerTargetPosition());
					CSoundMediator::playSfx(CSoundMediator::SFX_FRONT_END__MOVE_CURSOR);
					break;
				}
			}
		}
		m_pointerIcon->think(_frames);


		// SELECT or SAVE pressed?		
		if(m_pointerIcon->canPointerSelect()&&
		   pad&PAD_CROSS)
		{
			CSoundMediator::playSfx(CSoundMediator::SFX_FRONT_END__SELECT);
			switch(m_currentIconSelection)
			{
				case MAP_ICON_PREVIOUS_CHAPTER:
					m_currentChapterSelection--;
					generateMapScreenImage();
					if(m_currentChapterSelection==0)
					{
						m_currentIconSelection=MAP_ICON_LEVEL_1;
						m_pointerIcon->setTarget(getPointerTargetPosition());
					}
					break;

				case MAP_ICON_LEVEL_1:
				case MAP_ICON_LEVEL_2:
				case MAP_ICON_LEVEL_3:
				case MAP_ICON_LEVEL_4:
				case MAP_ICON_LEVEL_BONUS:
					s_globalLevelSelectThing=s_mapLevelData[m_currentChapterSelection][m_currentIconSelection-1].m_globalLevelNumber;
					m_readyToExit=true;
					CFader::setFadingOut();
					if(m_currentChapterSelection==3-1&&m_currentIconSelection-1==1-1)
					{
						// Plankton FMA..
						CFmaScene::selectFma(CFmaScene::FMA_SCRIPT__PLANKTON);
						GameState::setNextScene(&FmaScene);
					}
					else
					{
						GameState::setNextScene(&GameScene);
					}
					break;

				case MAP_ICON_NEXT_CHAPTER:
					m_currentChapterSelection++;
					generateMapScreenImage();
					if(m_currentChapterSelection==5-1||!isChapterOpen(m_currentChapterSelection+1))
					{
						for(int i=MAP_ICON_LEVEL_BONUS;i>=MAP_ICON_LEVEL_1;i--)
						{
							if(isLevelOpen(m_currentChapterSelection,i-1))
							{
								m_currentIconSelection=i;
								m_pointerIcon->setTarget(getPointerTargetPosition());
								break;
							}
						}
					}
					break;
			}
		}
		else if(pad&PAD_TRIANGLE)
		{
			CSoundMediator::playSfx(CSoundMediator::SFX_FRONT_END__OK);
			m_readyToExit=true;
			CFader::setFadingOut();
			CSaveScene::setNextScene(CSaveScene::NEXTSCENE_MAP);
			GameState::setNextScene(&SaveScene);
		}
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int CMapScene::readyToShutdown()
{
	return m_readyToExit&&!CFader::isFading();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CMapScene::generateMapScreenImage()
{
	int	i;

	LZNP_Decode((u8*)m_packedBackgroundImage,(u8*)m_screenImage);

	for(i=0;i<MAP_NUM_LEVELS_PER_CHAPTER;i++)
	{
		if(isLevelOpen(m_currentChapterSelection,i))
		{
			copyImageToScreen(s_mapLevelData[m_currentChapterSelection][i].m_Gfx,s_mapLevelPositions[i].vx,s_mapLevelPositions[i].vy,MAP_LEVEL_WIDTH,MAP_LEVEL_HEIGHT);
		}
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CMapScene::copyImageToScreen(int _file,int _x,int _y,int _w,int _h)
{
	u8	*image;
	u16	*src,*dst;
	int	y;

	image=(u8*)MemAlloc(MAP_LEVEL_WIDTH*MAP_LEVEL_HEIGHT*2,"MapLvlBuffer");
	LZNP_Decode(s_GfxTable[_file].Gfx,image);
	src=(u16*)image;
	dst=(u16*)m_screenImage+(_x+(_y*512));
	for(y=0;y<_h;y++)
	{
		u32	*S=(u32*)src;
		u32	*D=(u32*)dst;
		for (int x=0; x<MAP_LEVEL_WIDTH/2; x++)
		{
			*D++=*S++;
		}
		dst+=512;
		src+=_w;
	}
	MemFree(image);
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
DVECTOR	CMapScene::getPointerTargetPosition()
{
	DVECTOR	pos;
	pos=s_mapPointerPositions[m_currentIconSelection];
	return pos;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int		CMapScene::isLevelOpen(unsigned int _chapter,unsigned int _level)
{
	return CGameSlotManager::getSlotData()->isLevelOpen(_chapter,_level);
}

int		CMapScene::isChapterOpen(unsigned int _chapter)
{
	return CGameSlotManager::getSlotData()->isLevelOpen(_chapter,0);
}

int		CMapScene::getSpatulaCollectedCount(unsigned int _chapter,unsigned int _level)
{
	return CGameSlotManager::getSlotData()->getSpatulaCollectedCount(_chapter,_level);
}

int		CMapScene::hasQuestItemBeenCollected(unsigned int _chapter,unsigned int _level)
{
	return CGameSlotManager::getSlotData()->hasQustItemBeenCollected(_chapter,_level);
}


/*===========================================================================
 end */