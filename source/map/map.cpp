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


/*	Std Lib
	------- */

/*	Data
	---- */

#ifndef __STRING_ENUMS__
#include <trans.h>
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
};
int MAP_LEVEL_TOP_BORDER=20;
int MAP_LEVEL_Y_SPACING=10;
int MAP_LEVEL_X_SPACING=4;


int s_levelMaps[6]=
{
	MAP_C1_L1_GFX,
	MAP_C1_L2_GFX,
	MAP_C1_L3_GFX,
	MAP_C1_L4_GFX,
	MAP_C1_BOSS_GFX,
	MAP_C1_FAIR_GFX,
};

void CMapScene::init()
{
	m_font=new ("map screen font") FontBank();
	m_font->initialise(&standardFont);
	m_font->setJustification(FontBank::JUST_CENTRE);
	m_font->setOt(5);

	m_readyToExit=false;
//	CFader::setFadingIn(CFader::BLACK_FADE);

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
			if(isLevelOpen
			copyImageToScreen(s_levelMaps[i],xpos,ypos,MAP_LEVEL_WIDTH,MAP_LEVEL_HEIGHT);
			i++;
			xpos+=MAP_LEVEL_WIDTH+MAP_LEVEL_X_SPACING;
		}
		ypos+=MAP_LEVEL_HEIGHT+MAP_LEVEL_Y_SPACING;
	}

	SetScreenImage((u8*)m_screenImage);
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
	m_font->setColour(255,255,255);
	m_font->print(256,100,"MAP SCREEN!!!");
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
		if(PadGetDown(0)&(PAD_CROSS|PAD_START))
		{
			m_readyToExit=true;
//			CFader::setFadingOut(CFader::BLACK_FADE);
			GameState::setNextScene(&FrontEndScene);
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

/*===========================================================================
 end */