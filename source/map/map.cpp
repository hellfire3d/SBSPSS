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

#ifndef	__FRONTEND_FRONTEND_H__
#include "frontend\frontend.h"
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

#ifndef	__GUI_GFRAME_H__
#include "gui\gframe.h"
#endif

#ifndef	__GUI_GFACTORY_H__
#include "gui\gfactory.h"
#endif

#ifndef __MATHTABLE_HEADER__
#include "utils\mathtab.h"
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


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
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


static int	s_levelMaps[5][6]=
{
	{	MAP_C1_L1_GFX,	MAP_C1_L2_GFX,	MAP_C1_L3_GFX,	MAP_C1_L4_GFX,	MAP_C1_BOSS_GFX,	MAP_C1_FAIR_GFX	},
	{	MAP_C1_L1_GFX,	MAP_C1_L2_GFX,	MAP_C1_L3_GFX,	MAP_C1_L4_GFX,	MAP_C1_BOSS_GFX,	MAP_C1_FAIR_GFX	},
	{	MAP_C1_L1_GFX,	MAP_C1_L2_GFX,	MAP_C1_L3_GFX,	MAP_C1_L4_GFX,	MAP_C1_BOSS_GFX,	MAP_C1_FAIR_GFX	},
	{	MAP_C1_L1_GFX,	MAP_C1_L2_GFX,	MAP_C1_L3_GFX,	MAP_C1_L4_GFX,	MAP_C1_BOSS_GFX,	MAP_C1_FAIR_GFX	},
	{	MAP_C1_L1_GFX,	MAP_C1_L2_GFX,	MAP_C1_L3_GFX,	MAP_C1_L4_GFX,	MAP_C1_BOSS_GFX,	MAP_C1_FAIR_GFX	},
};
static int	s_questItems[5][4]=
{
	{	FRM__C1_L1_JELLY,			FRM__C1_L2_SEAWEED,			FRM__C1_L3_SEANUTBUTTER,	FRM__C1_L4_BREADSLICE	},
	{	FRM__C2_L1_FALSETEETH,		FRM__C2_L2_KELPKREAM,		FRM__C2_L3_MUDPACK,			FRM__C2_L4_SPONGE		},
	{	FRM__C3_L1_SLIPPERS,		FRM__C3_L2_CLAMBRA,			FRM__C3_L3_STARFISHMASK,	FRM__C3_L4_SUPERPANTZ	},
	{	FRM__C4_COIN,				FRM__C4_COIN,				FRM__C4_COIN,				FRM__C4_KELPBAR			},	
	{	FRM__C5_L1_HAMMER,			FRM__C5_L2_ARIEL,			FRM__C5_L3_OILCAN,			FRM__C5_L4_WRENCH		},
};
static int	s_levelNumbers[5][4]=
{
	{	0,		1,		2,		3,		4,		25	},
	{	5,		6,		7,		8,		9,		26	},
	{	10,		11,		12,		13,		14,		27	},
	{	15,		16,		17,		18,		19,		28	},
	{	20,		21,		22,		23,		24,		29	},
}


void CMapScene::init()
{
	m_font=new ("map screen font") FontBank();
	m_font->initialise(&standardFont);
	m_font->setJustification(FontBank::JUST_CENTRE);
	m_font->setOt(10);
	m_font->setColour(0,0,0);
	m_font->setJustification(FontBank::JUST_LEFT);

	m_spriteBank=new ("map screen sprite") SpriteBank();
	m_spriteBank->load(SPRITES_SPRITES_SPR);


	// Generate the map background image
	int	i,x,y,xpos,ypos;
	m_screenImage=MemAlloc(512*256*2,"MapScreen");
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
				copyImageToScreen(s_levelMaps[m_currentChapterSelection][i],xpos,ypos,MAP_LEVEL_WIDTH,MAP_LEVEL_HEIGHT);
			}
			i++;
			xpos+=MAP_LEVEL_WIDTH+MAP_LEVEL_X_SPACING;
		}
		ypos+=MAP_LEVEL_HEIGHT+MAP_LEVEL_Y_SPACING;
	}

	SetScreenImage((u8*)m_screenImage);

	// Generate the ui box at the bottom of the screen
	m_guiFrame=new ("Conversation GUI") CGUIControlFrame();
	m_guiFrame->init(0);
	m_guiFrame->setObjectXYWH((512-MAP_INSTRUCTIONS_WIDTH)/2,256-MAP_INSTRUCTIONS_BOTTOM_OFFSET-MAP_INSTRUCTIONS_HEIGHT,MAP_INSTRUCTIONS_WIDTH,MAP_INSTRUCTIONS_HEIGHT);
//	m_guiFrame->setOt(OT_POS);
	m_guiFrame->setFlags(CGUIObject::FLAG_DRAWBORDER);


	m_readyToExit=false;
//	CFader::setFadingIn(CFader::BLACK_FADE);


	m_currentChapterSelection=0;
	m_currentLevelSelection=0;
	m_pointerPos.vx=256;
	m_pointerPos.vy=-50;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CMapScene::shutdown()
{
	m_guiFrame->shutdown();
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
			fh=m_spriteBank->getFrameHeader(s_questItems[m_currentChapterSelection][i]);
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
	
	m_guiFrame->render();
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
		m_pointerTarget.vx=256-((MAP_LEVEL_WIDTH*3)/2)-MAP_LEVEL_X_SPACING+
						   ((m_currentLevelSelection%3)*(MAP_LEVEL_WIDTH+MAP_LEVEL_X_SPACING))+
						   (MAP_LEVEL_WIDTH/2);
		m_pointerTarget.vy=MAP_PARCHMENT_START_Y+MAP_LEVEL_TOP_BORDER+
						   ((m_currentLevelSelection/3)*(MAP_LEVEL_HEIGHT+MAP_LEVEL_Y_SPACING))+
						   (MAP_LEVEL_HEIGHT)+
						   (msin(m_pointerSin)*4>>12);

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
			m_readyToExit=true;
//			CFader::setFadingOut(CFader::BLACK_FADE);
			GameState::setNextScene(&FrontEndScene);
		}
		m_guiFrame->think(_frames);
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
int		CMapScene::isLevelOpen(int _chapter,int _level)
{
	return true;//_level<4;
}

int		CMapScene::getSpatulaCollectedCount(int _chapter,int _level)
{
	return 10+_level;
}

int		CMapScene::getSpatulaAvailableCount(int _chapter,int _level)
{
		return 30;
}

int		CMapScene::hasQuestItemBeenCollected(int _chapter,int _level)
{
	return _level<2;
}


/*===========================================================================
 end */