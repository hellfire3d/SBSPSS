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


CMapScene::sMapLevelData	CMapScene::s_mapLevelData[MAP_NUM_CHAPTERS][MAP_NUM_LEVELS_PER_CHAPTER]=
{
	//		mapFile				globalLevelNumber	spatulaOrTokenCounts								questItemFrame			kelpWorldLevel
	{
		{	MAP_C1_L1_GFX,		0,					CHAPTER01_LEVEL01_INF_TOTAL_ITEM_GOLDEN_SPATULA,	FRM__C1_L1_QUEST_ITEM,	false	},
		{	MAP_C1_L2_GFX,		1,					CHAPTER01_LEVEL02_INF_TOTAL_ITEM_GOLDEN_SPATULA,	FRM__C1_L2_QUEST_ITEM,	false	},
		{	MAP_C1_L3_GFX,		2,					CHAPTER01_LEVEL03_INF_TOTAL_ITEM_GOLDEN_SPATULA,	FRM__C1_L3_QUEST_ITEM,	false	},
		{	MAP_C1_L4_GFX,		3,					CHAPTER01_LEVEL04_INF_TOTAL_ITEM_GOLDEN_SPATULA,	FRM__C1_L4_QUEST_ITEM,	false	},
		{	MAP_C1_FAIR_GFX,	4,					CHAPTER06_LEVEL01_INF_TOTAL_ITEM_KELP_TOKEN,		-1,						true	},
	},
	{
		{	MAP_C2_L1_GFX,		5,					CHAPTER02_LEVEL01_INF_TOTAL_ITEM_GOLDEN_SPATULA,	FRM__C2_L1_QUEST_ITEM,	false	},
		{	MAP_C2_L2_GFX,		6,					CHAPTER02_LEVEL02_INF_TOTAL_ITEM_GOLDEN_SPATULA,	FRM__C2_L2_QUEST_ITEM,	false	},
		{	MAP_C2_L3_GFX,		7,					CHAPTER02_LEVEL03_INF_TOTAL_ITEM_GOLDEN_SPATULA,	FRM__C2_L3_QUEST_ITEM,	false	},
		{	MAP_C2_L4_GFX,		8,					CHAPTER02_LEVEL04_INF_TOTAL_ITEM_GOLDEN_SPATULA,	FRM__C2_L4_QUEST_ITEM,	false	},
		{	MAP_C2_FAIR_GFX,	9,					CHAPTER06_LEVEL02_INF_TOTAL_ITEM_KELP_TOKEN,		-1,						true	},
	},
	{
		{	MAP_C3_L1_GFX,		10,					CHAPTER03_LEVEL01_INF_TOTAL_ITEM_GOLDEN_SPATULA,	FRM__C3_L1_QUEST_ITEM,	false	},
		{	MAP_C3_L2_GFX,		11,					CHAPTER03_LEVEL02_INF_TOTAL_ITEM_GOLDEN_SPATULA,	FRM__C3_L2_QUEST_ITEM,	false	},
		{	MAP_C3_L3_GFX,		12,					CHAPTER03_LEVEL03_INF_TOTAL_ITEM_GOLDEN_SPATULA,	FRM__C3_L3_QUEST_ITEM,	false	},
		{	MAP_C3_L4_GFX,		13,					CHAPTER03_LEVEL04_INF_TOTAL_ITEM_GOLDEN_SPATULA,	FRM__C3_L4_QUEST_ITEM,	false	},
		{	MAP_C3_FAIR_GFX,	14,					CHAPTER06_LEVEL03_INF_TOTAL_ITEM_KELP_TOKEN,		-1,						true	},
	},
	{
		{	MAP_C4_L1_GFX,		15,					CHAPTER04_LEVEL01_INF_TOTAL_ITEM_GOLDEN_SPATULA,	FRM__C4_QUEST_ITEM_1,	false	},
		{	MAP_C4_L2_GFX,		16,					CHAPTER04_LEVEL02_INF_TOTAL_ITEM_GOLDEN_SPATULA,	FRM__C4_QUEST_ITEM_1,	false	},
		{	MAP_C4_L3_GFX,		17,					CHAPTER04_LEVEL03_INF_TOTAL_ITEM_GOLDEN_SPATULA,	FRM__C4_QUEST_ITEM_1,	false	},
		{	MAP_C4_L4_GFX,		18,					CHAPTER04_LEVEL04_INF_TOTAL_ITEM_GOLDEN_SPATULA,	FRM__C4_QUEST_ITEM_2,	false	},
		{	MAP_C4_FAIR_GFX,	19,					CHAPTER06_LEVEL04_INF_TOTAL_ITEM_KELP_TOKEN,		-1,						true	},
	},
	{
		{	MAP_C5_L1_GFX,		20,					CHAPTER05_LEVEL01_INF_TOTAL_ITEM_GOLDEN_SPATULA,	FRM__C5_L1_QUEST_ITEM,	false	},
		{	MAP_C5_L2_GFX,		21,					CHAPTER05_LEVEL02_INF_TOTAL_ITEM_GOLDEN_SPATULA,	FRM__C5_L2_QUEST_ITEM,	false	},
		{	MAP_C5_L3_GFX,		22,					CHAPTER05_LEVEL03_INF_TOTAL_ITEM_GOLDEN_SPATULA,	FRM__C5_L3_QUEST_ITEM,	false	},
		{	MAP_C5_L4_GFX,		23,					CHAPTER05_LEVEL04_INF_TOTAL_ITEM_GOLDEN_SPATULA,	FRM__C5_L4_QUEST_ITEM,	false	},
		{	MAP_C5_FAIR_GFX,	24,					CHAPTER06_LEVEL05_INF_TOTAL_ITEM_KELP_TOKEN,		-1,						true	},
	},
};


DVECTOR						CMapScene::s_mapLevelPositions[MAP_NUM_LEVELS_PER_CHAPTER]=
{
	{	42,49	},
	{	186,49	},
	{	330,49	},
	{	42,113	},
	{	330,113	},
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
	m_font=new ("map screen font") FontBank();
	m_font->initialise(&standardFont);
	m_font->setOt(10);
	m_font->setJustification(FontBank::JUST_LEFT);

	m_screenImage=MemAlloc(512*256*2,"MapScreen");

	m_currentChapterSelection=s_chapterToStartOn;
	m_mapBackgroundImage=(char*)CFileIO::loadFile(MAP_MAP_BACKGROUND_GFX);ASSERT(m_mapBackgroundImage);
	generateMapScreenImage();
	m_currentLevelSelection=s_levelToStartOn;

	m_pointerIcon=new ("MapPointer") CPointerIcon();
	m_pointerIcon->snapToTarget(getPointerTargetPosition());
	
	SetScreenImage((u8*)m_screenImage);

	m_readyToExit=false;
	CFader::setFadingIn();

	CSoundMediator::setSong(CSoundMediator::SONG_MAPSCREEN);
	m_musicStarted=false;
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
	delete m_pointerIcon;
	MemFree(m_mapBackgroundImage);
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


sprintf(buf,"Chapter %d, Level %d",m_currentChapterSelection+1,m_currentLevelSelection+1);
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
	int				renderChapterControls;
	SpriteBank		*sb;
	sFrameHdr		*fh1,*fh2;
	int				width;
	int				x,y;


	// If only the first chapter is open then you can't select other ones...
	renderChapterControls=isChapterOpen(1)?true:false;

	sb=CGameScene::getSpriteBank();
	m_font->setColour(MAP_INSTRUCTIONS_TEXT_R,MAP_INSTRUCTIONS_TEXT_G,MAP_INSTRUCTIONS_TEXT_B);

	y=MAP_INSTRUCTIONS_YSTART;
	fh1=sb->getFrameHeader(FRM__BUTL);
	fh2=sb->getFrameHeader(FRM__BUTR);
	width=fh1->W+MAP_INSTRUCTIONS_GAP_BETWEEN_BUTTONS+fh2->W+MAP_INSTRUCTIONS_GAP_BETWEEN_BUTTONS_AND_TEXT+m_font->getStringWidth(STR__MAP_SCREEN__LEFT_RIGHT_TO_SELECT_LEVEL);
	x=256-(width/2);
	sb->printFT4(fh1,x,y+MAP_INSTRUCTIONS_BUTTON_Y_OFFSET,0,0,0);
	x+=fh1->W+MAP_INSTRUCTIONS_GAP_BETWEEN_BUTTONS;
	sb->printFT4(fh2,x,y+MAP_INSTRUCTIONS_BUTTON_Y_OFFSET,0,0,0);
	x+=fh2->W+MAP_INSTRUCTIONS_GAP_BETWEEN_BUTTONS_AND_TEXT;
	m_font->print(x,y,STR__MAP_SCREEN__LEFT_RIGHT_TO_SELECT_LEVEL);

	y+=MAP_INSTRUCTIONS_Y_SPACE_BETWEEN_LINES;
	fh1=sb->getFrameHeader(FRM__BUTU);
	fh2=sb->getFrameHeader(FRM__BUTD);
	width=fh1->W+MAP_INSTRUCTIONS_GAP_BETWEEN_BUTTONS+fh2->W+MAP_INSTRUCTIONS_GAP_BETWEEN_BUTTONS_AND_TEXT+m_font->getStringWidth(STR__MAP_SCREEN__UP_DOWN_TO_SELECT_CHAPTER);
	x=256-(width/2);
	if(renderChapterControls)
	{
		sb->printFT4(fh1,x,y+MAP_INSTRUCTIONS_BUTTON_Y_OFFSET,0,0,0);
	}
	x+=fh1->W+MAP_INSTRUCTIONS_GAP_BETWEEN_BUTTONS;
	if(renderChapterControls)
	{
		sb->printFT4(fh2,x,y+MAP_INSTRUCTIONS_BUTTON_Y_OFFSET,0,0,0);
	}
	x+=fh2->W+MAP_INSTRUCTIONS_GAP_BETWEEN_BUTTONS_AND_TEXT;
	if(renderChapterControls)
	{
		m_font->print(x,y,STR__MAP_SCREEN__UP_DOWN_TO_SELECT_CHAPTER);
	}

	y+=MAP_INSTRUCTIONS_Y_SPACE_BETWEEN_LINES;
	fh1=sb->getFrameHeader(FRM__BUTX);
	width=fh1->W+MAP_INSTRUCTIONS_GAP_BETWEEN_BUTTONS_AND_TEXT+m_font->getStringWidth(STR__MAP_SCREEN__X_TO_START);
	x=256-(width/2);
	sb->printFT4(fh1,x,y+MAP_INSTRUCTIONS_BUTTON_Y_OFFSET,0,0,0);
	x+=fh1->W+MAP_INSTRUCTIONS_GAP_BETWEEN_BUTTONS_AND_TEXT;
	m_font->print(x,y,STR__MAP_SCREEN__X_TO_START);

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
void CMapScene::think(int _frames)
{
	if(!CFader::isFading()&&!m_readyToExit)
	{
		if(m_musicStarted==false)
		{
			m_musicStarted=true;
			CSoundMediator::playSong();
		}
		
		int	pad=PadGetDown(0);

		// Change chapter
		if(pad&PAD_UP)
		{
			int nextChapter=m_currentChapterSelection;
			do
			{
				nextChapter++;
				if(nextChapter==MAP_NUM_CHAPTERS)nextChapter=0;
			}
			while(!isChapterOpen(nextChapter));
			if(nextChapter!=m_currentChapterSelection)
			{
				m_currentChapterSelection=nextChapter;
				generateMapScreenImage();
			}
		}
		else if(pad&PAD_DOWN)
		{
			int nextChapter=m_currentChapterSelection;
			do
			{
				nextChapter--;
				if(nextChapter<0)nextChapter=MAP_NUM_CHAPTERS-1;
			}
			while(!isChapterOpen(nextChapter));
			if(nextChapter!=m_currentChapterSelection)
			{
				m_currentChapterSelection=nextChapter;
				generateMapScreenImage();
			}
		}
#ifdef __VERSION_DEBUG__
		else if(pad&PAD_L1)
		{
			CGameSlotManager::getSlotData()->debugCheatOpenAllLevels();
			generateMapScreenImage();
		}
#endif

		// Move cursor
		int	lastLevel=m_currentLevelSelection;
		if(pad&PAD_LEFT)
		{
			do
			{
				if(--m_currentLevelSelection<0)m_currentLevelSelection=MAP_NUM_LEVELS_PER_CHAPTER-1;
			}
			while(!isLevelOpen(m_currentChapterSelection,m_currentLevelSelection));
			m_pointerIcon->setTarget(getPointerTargetPosition());
		}
		else if(pad&PAD_RIGHT)
		{
			do
			{
				if(++m_currentLevelSelection>=MAP_NUM_LEVELS_PER_CHAPTER)m_currentLevelSelection=0;
			}
			while(!isLevelOpen(m_currentChapterSelection,m_currentLevelSelection));
			m_pointerIcon->setTarget(getPointerTargetPosition());
		}
		if(lastLevel!=m_currentLevelSelection)
		{
			CSoundMediator::playSfx(CSoundMediator::SFX_FRONT_END__MOVE_CURSOR);
		}
		m_pointerIcon->think(_frames);

		if(m_pointerIcon->canPointerSelect()&&
		   pad&PAD_CROSS)
		{
			CSoundMediator::playSfx(CSoundMediator::SFX_FRONT_END__OK);
			s_globalLevelSelectThing=s_mapLevelData[m_currentChapterSelection][m_currentLevelSelection].m_globalLevelNumber;
			m_readyToExit=true;
			CFader::setFadingOut();
			GameState::setNextScene(&GameScene);
		}
		else if(pad&PAD_TRIANGLE)
		{
			CSoundMediator::playSfx(CSoundMediator::SFX_FRONT_END__OK);
			m_readyToExit=true;
			CFader::setFadingOut();
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

	m_currentLevelSelection=0;
	memcpy(m_screenImage,m_mapBackgroundImage,512*256*2);
	for(i=0;i<MAP_NUM_LEVELS_PER_CHAPTER;i++)
	{
		if(isLevelOpen(m_currentChapterSelection,i))
		{
			copyImageToScreen(s_mapLevelData[m_currentChapterSelection][i].m_mapFile,s_mapLevelPositions[i].vx,s_mapLevelPositions[i].vy,MAP_LEVEL_WIDTH,MAP_LEVEL_HEIGHT);
		}
	}

	m_pointerIcon->snapToTarget(getPointerTargetPosition());
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
	int	x;

	image=CFileIO::loadFile((FileEquate)_file);ASSERT(image);
	src=(u16*)image;
	dst=(u16*)m_screenImage+(_x+(_y*512));
	for(y=0;y<_h;y++)
	{
		memcpy(dst,src,_w*2);
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
	
	pos=s_mapLevelPositions[m_currentLevelSelection];
	pos.vx+=(MAP_LEVEL_WIDTH/2);
	pos.vy+=MAP_LEVEL_HEIGHT;

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