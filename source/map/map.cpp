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


CMapScene::sMapLevelData	CMapScene::s_mapLevelData[5]=
{
	// Chapter 1
	{
		{	MAP_C1_L1_GFX,		MAP_C1_L2_GFX,		MAP_C1_L3_GFX,		MAP_C1_L4_GFX,		MAP_C1_BOSS_GFX,	MAP_C1_FAIR_GFX	},
		{	0,					1,					2,					3,					-1,					20				},
		{	FRM__C1_L1_JELLY,			FRM__C1_L2_SEAWEED,			FRM__C1_L3_SEANUTBUTTER,	FRM__C1_L4_BREADSLICE	},
	},
	// Chapter 2
	{
		{	MAP_C2_L1_GFX,		MAP_C2_L2_GFX,		MAP_C2_L3_GFX,		MAP_C2_L4_GFX,		MAP_C2_BOSS_GFX,	MAP_C2_FAIR_GFX	},
		{	4,					5,					6,					7,					-1,					21				},
		{	FRM__C2_L1_FALSETEETH,		FRM__C2_L2_KELPKREAM,		FRM__C2_L3_MUDPACK,			FRM__C2_L4_SPONGE		},
	},
	// Chapter 3
	{
		{	MAP_C3_L1_GFX,		MAP_C3_L2_GFX,		MAP_C3_L3_GFX,		MAP_C3_L4_GFX,		MAP_C3_BOSS_GFX,	MAP_C3_FAIR_GFX	},
		{	8,					9,					10,					11,					-1,					22				},
		{	FRM__C3_L1_SLIPPERS,		FRM__C3_L2_CLAMBRA,			FRM__C3_L3_STARFISHMASK,	FRM__C3_L4_SUPERPANTZ	},
	},
	// Chapter 4
	{
		{	MAP_C4_L1_GFX,		MAP_C4_L2_GFX,		MAP_C4_L3_GFX,		MAP_C4_L4_GFX,		MAP_C4_BOSS_GFX,	MAP_C4_FAIR_GFX	},
		{	12,					13,					14,					15,					-1,					23				},
		{	FRM__C4_COIN,				FRM__C4_COIN,				FRM__C4_COIN,				FRM__C4_KELPBAR			},	
	},
	// Chapter 5
	{
		{	MAP_C5_L1_GFX,		MAP_C5_L2_GFX,		MAP_C5_L3_GFX,		MAP_C5_L4_GFX,		MAP_C5_BOSS_GFX,	MAP_C5_FAIR_GFX	},
		{	16,					17,					18,					19,					-1,					24				},
		{	FRM__C5_L1_HAMMER,			FRM__C5_L2_ARIEL,			FRM__C5_L3_OILCAN,			FRM__C5_L4_WRENCH		},
	},
};

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
	m_font->setJustification(FontBank::JUST_CENTRE);
	m_font->setOt(10);
	m_font->setJustification(FontBank::JUST_LEFT);

	m_spriteBank=new ("map screen sprite") SpriteBank();
	m_spriteBank->load(SPRITES_SPRITES_SPR);


	m_currentChapterSelection=0;
	m_currentLevelSelection=0;
	m_screenImage=MemAlloc(512*256*2,"MapScreen");
	generateMapScreenImage();
	SetScreenImage((u8*)m_screenImage);

	m_readyToExit=false;
	CFader::setFadingIn(CFader::BLACK_FADE);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CMapScene::shutdown()
{
	ClearScreenImage();
	MemFree(m_screenImage);
	m_spriteBank->dump();		delete m_spriteBank;
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
	int			i,x,y,xpos,ypos;
	int			compilerGetsComfused;
	char		spatCount[10];
	sFrameHdr	*fh;
	POLY_FT4	*ft4;
	
	// Render spatula counts and quest items
	m_font->setColour(253,251,67);
	i=0;
	ypos=MAP_PARCHMENT_START_Y+MAP_LEVEL_TOP_BORDER;
	for(y=0;y<2;y++)
	{
		xpos=256-((MAP_LEVEL_WIDTH*3)/2)-MAP_LEVEL_X_SPACING;
		compilerGetsComfused=y==0?3:1;		// 3 on top row, 1 on bottom - put this in the for loop and the complier breaks :o
		for(x=0;x<compilerGetsComfused;x++)
		{
			sprintf(spatCount,"%d/%d",getSpatulaCollectedCount(m_currentChapterSelection,i),getSpatulaAvailableCount(m_currentChapterSelection,i));
			m_font->print(xpos,ypos,spatCount);
			fh=m_spriteBank->getFrameHeader(s_mapLevelData[m_currentChapterSelection].m_questItemFrames[i]);
			ft4=m_spriteBank->printFT4Scaled(fh,xpos+MAP_LEVEL_WIDTH-fh->W,ypos+MAP_LEVEL_HEIGHT-fh->H,0,0,10,128);
			if(!hasQuestItemBeenCollected(m_currentChapterSelection,i))
			{
				setRGB0(ft4,10,10,10);
			}
			i++;
			xpos+=MAP_LEVEL_WIDTH+MAP_LEVEL_X_SPACING;
		}
		ypos+=MAP_LEVEL_HEIGHT+MAP_LEVEL_Y_SPACING;
	}

	// Selection cursor
	fh=m_spriteBank->getFrameHeader(FRM__MAPPOINTER);
	m_spriteBank->printFT4(fh,m_pointerPos.vx-(fh->W/2),m_pointerPos.vy-(fh->H/2),0,0,9);

char buf[100];
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
void CMapScene::think(int _frames)
{
	if(!CFader::isFading()&&!m_readyToExit)
	{
		// Change chapter
#ifndef __USER_CDBUILD__
		if(PadGetDown(0)&PAD_UP)
		{
			if(++m_currentChapterSelection>4)m_currentChapterSelection=0;
			generateMapScreenImage();
		}
		else if(PadGetDown(0)&PAD_DOWN)
		{
			if(--m_currentChapterSelection<0)m_currentChapterSelection=4;
			generateMapScreenImage();
		}
#endif

		// Move cursor
		if(PadGetDown(0)&PAD_LEFT)
		{
			do
			{
				if(--m_currentLevelSelection<0)m_currentLevelSelection=5;
			}
			while(!isLevelOpen(m_currentChapterSelection,m_currentLevelSelection));
		}
		else if(PadGetDown(0)&PAD_RIGHT)
		{
			do
			{
				if(++m_currentLevelSelection>5)m_currentLevelSelection=0;
			}
			while(!isLevelOpen(m_currentChapterSelection,m_currentLevelSelection));
		}
		
		// Calc where the pointer should be
		m_pointerSin=(m_pointerSin+(_frames*70))&4095;
		m_pointerTarget=getPointerTargetPosition();

		// Move the pointer
		for(int i=0;i<_frames;i++)
		{
			int delta;

			delta=m_pointerTarget.vx-m_pointerPos.vx;
			if(delta<0)
			{
				delta/=2;
				if(!delta)delta=-1;
			}
			else if(delta>0)
			{
				delta/=2;
				if(!delta)delta=+1;
			}
			m_pointerPos.vx+=delta;

			delta=m_pointerTarget.vy-m_pointerPos.vy;
			if(delta<0)
			{
				delta/=2;
				if(!delta)delta=-1;
			}
			else if(delta>0)
			{
				delta/=2;
				if(!delta)delta=+1;
			}
			m_pointerPos.vy+=delta;
		}

		if(PadGetDown(0)&(PAD_CROSS|PAD_START))
		{
			s_globalLevelSelectThing=s_mapLevelData[m_currentChapterSelection].m_globalLevelNumbers[m_currentLevelSelection];
			m_readyToExit=true;
//			CFader::setFadingOut(CFader::BLACK_FADE);
			GameState::setNextScene(&GameScene);
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
	int	i,x,y,xpos,ypos;


	m_currentLevelSelection=0;
	memset(m_screenImage,0,512*256*2);
	copyImageToScreen(MAP_MAP_BACKGROUND_GFX,MAP_PARCHMENT_START_X,MAP_PARCHMENT_START_Y,MAP_PARCHMENT_WIDTH,MAP_PARCHMENT_HEIGHT);
	i=0;
	ypos=MAP_PARCHMENT_START_Y+MAP_LEVEL_TOP_BORDER;
	for(y=0;y<2;y++)
	{
		xpos=256-((MAP_LEVEL_WIDTH*3)/2)-MAP_LEVEL_X_SPACING;
		for(x=0;x<3;x++)
		{
			if(isLevelOpen(m_currentChapterSelection,i))
			{
				copyImageToScreen(s_mapLevelData[m_currentChapterSelection].m_mapFiles[i],xpos,ypos,MAP_LEVEL_WIDTH,MAP_LEVEL_HEIGHT);
			}
			i++;
			xpos+=MAP_LEVEL_WIDTH+MAP_LEVEL_X_SPACING;
		}
		ypos+=MAP_LEVEL_HEIGHT+MAP_LEVEL_Y_SPACING;
	}

	m_pointerPos=getPointerTargetPosition();
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
	int	x,y;

	image=CFileIO::loadFile((FileEquate)_file);ASSERT(image);
	src=(u16*)image;
	dst=(u16*)m_screenImage+(_x+(_y*512));
	for(y=0;y<_h;y++)
	{
		for(x=0;x<_w;x++)
		{
			*dst++=*src++;
		}
		dst+=512-_w;
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
	pos.vx=256-((MAP_LEVEL_WIDTH*3)/2)-MAP_LEVEL_X_SPACING+
		   ((m_currentLevelSelection%3)*(MAP_LEVEL_WIDTH+MAP_LEVEL_X_SPACING))+
		   (MAP_LEVEL_WIDTH/2);
	pos.vy=MAP_PARCHMENT_START_Y+MAP_LEVEL_TOP_BORDER+
		   ((m_currentLevelSelection/3)*(MAP_LEVEL_HEIGHT+MAP_LEVEL_Y_SPACING))+
		   (MAP_LEVEL_HEIGHT)+
		   (msin(m_pointerSin)*4>>12);
	return pos;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int		CMapScene::isLevelOpen(int _chapter,int _level)
{
#ifdef __USER_CDBUILD__
	return _level<4;		// no boss or kelp world for thq
#else
	return _level!=4;		// no boss levels atm..
#endif
}

int		CMapScene::getSpatulaCollectedCount(int _chapter,int _level)
{
	return CGameSlotManager::getSlotData()->getSpatulaCollectedCount(_chapter,_level);
}

int		CMapScene::getSpatulaAvailableCount(int _chapter,int _level)
{
	return 99;
}

int		CMapScene::hasQuestItemBeenCollected(int _chapter,int _level)
{
	return true;
}


/*===========================================================================
 end */