/*=========================================================================

	maintitl.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/


/*----------------------------------------------------------------------
	Includes
	-------- */

#include "frontend\maintitl.h"

#ifndef	__MEMORY_HEADER__
#include "mem\memory.h"
#endif

#ifndef __GFX_SPRBANK_H__
#include "gfx\sprbank.h"
#endif

#ifndef __GFX_FONT_H__
#include "gfx\font.h"
#endif

#ifndef __MATHTABLE_HEADER__
#include "utils\mathtab.h"
#endif

#ifndef __LOCALE_TEXTDBASE_H__
#include "locale\textdbase.h"
#endif

#ifndef __PAD_PADS_H__
#include "pad\pads.h"
#endif

#ifndef	__GFX_FADER_H__
#include "gfx\fader.h"
#endif


/*	Std Lib
	------- */

/*	Data
	---- */

#ifndef __SPR_FRONTEND_H__
#include <frontend.h>
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

///////////////
int seaw=10;
int seah=70;
int seaspeed=20;
int seagap=120;
int seascale=4000;
int	seascalegrow=-50;

int sval=0;
///////////////


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CFrontEndMainTitles::init()
{
	m_sprites=new ("MainTitle Sprites") SpriteBank();
	m_sprites->load(FRONTEND_FRONTEND_SPR);

	m_smallFont=new ("MainTitle SmallFont") FontBank();
	m_smallFont->initialise(&standardFont);
	m_smallFont->setJustification(FontBank::JUST_CENTRE);

	m_bigFont=new ("MainTitle BigFont") FontBank();
	m_bigFont->initialise(&largeFont);
	m_bigFont->setJustification(FontBank::JUST_CENTRE);
	m_bigFont->setColour(PRESS_START_TEXT_R,PRESS_START_TEXT_G,PRESS_START_TEXT_B);

	m_mode=MODE__PRESS_START;

	CFader::setFadingIn();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CFrontEndMainTitles::shutdown()
{
	m_bigFont->dump();			delete m_bigFont;
	m_smallFont->dump();		delete m_smallFont;
	m_sprites->dump();			delete m_sprites;
}


typedef struct
{
	int	x,y;
}xy;
xy pos[10]={{0,0},{3,0},{-3,0},{0,2},{0,-2}};
int posnum=5;

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CFrontEndMainTitles::render()
{
	sFrameHdr	*fh;
	POLY_FT4	*ft4;

	// Game logo/title
	fh=m_sprites->getFrameHeader(FRM__SBLOGO);
	m_sprites->printFT4(fh,256-(fh->W/2),LOGO_CENTRE_Y-(fh->H/2),0,0,20);
	m_smallFont->setColour(GAME_TITLE_TEXT_R,GAME_TITLE_TEXT_G,GAME_TITLE_TEXT_B);
	m_smallFont->print(256,GAME_TITLE_TEXT_CENTRE_Y,STR__FRONTEND__GAME_TITLE);
	m_smallFont->setColour(0,0,0);
	m_smallFont->print(256+1,GAME_TITLE_TEXT_CENTRE_Y+1,STR__FRONTEND__GAME_TITLE);

	// The island
	fh=m_sprites->getFrameHeader(FRM__ISLAND);
	m_sprites->printFT4(fh,ISLAND_LEFT_X,ISLAND_BOTTOM_Y-(fh->H),0,0,11);

	// Sky
	fh=m_sprites->getFrameHeader(FRM_SKY);
	for(int i=0;i<posnum;i++)
	{
		ft4=m_sprites->printFT4(fh,0,0,0,0,1020-i);
		setXYWH(ft4,pos[i].x,pos[i].y,512,HORIZON_LEVEL);
		if(i)
			setSemiTrans(ft4,true);
	}

	renderSea();

	switch(m_mode)
	{
		case MODE__PRESS_START:
			renderPressStart();
			break;
		case MODE__SELECT_OPTION:
			break;
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CFrontEndMainTitles::think(int _frames)
{
	sval=(sval+(seaspeed*_frames))&4095;

	switch(m_mode)
	{
		case MODE__PRESS_START:
			if(!CFader::isFading()&&PadGetDown(0)&PAD_START)
			{
				m_mode=MODE__SELECT_OPTION;
CFader::setFadingOut();
			}
			break;
		case MODE__SELECT_OPTION:
			break;
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
POLY_FT4	*CFrontEndMainTitles::prepareSeaPortionFT4(sFrameHdr *_fh,int _x,int _y,int _w,int _h)
{
	int			u,v,w,h;
	POLY_FT4	*ft4;


	w=(_fh->W*_w)>>8;
	h=(_fh->H*_h)>>8;
	u=_fh->U+((_fh->W*_x)>>8);
	v=_fh->V+_fh->H-((_fh->H*_y)>>8)-h;

	ft4=GetPrimFT4();
	setShadeTexPolyFT4(ft4);
	setShadeTex(ft4,0);
	setRGB0(ft4,64,64,64);

	ft4->u0=u;
	ft4->u1=u+w;
	ft4->u2=u;
	ft4->u3=u+w;
	ft4->v0=v+h-1;
	ft4->v1=v+h-1;
	ft4->v2=v-1;
	ft4->v3=v-1;
	ft4->tpage=_fh->TPage;
	ft4->clut=_fh->Clut;

	return ft4;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CFrontEndMainTitles::renderSea()
{
	sFrameHdr	*fh;
	int			xloop,yloop;
	int			xstep,ystep;
	int			x,y;
	int			scale;
	POLY_FT4	*ft4;
	int			grid[seaw+1][seah+1][2];
	int			waveval;


	// Grow the grid
	xstep=(512<<8)/seaw;
	ystep=((256-HORIZON_LEVEL+(2048/(seascale+(seascalegrow*seah))))<<8)/seah;
	y=HORIZON_LEVEL<<8;
	waveval=sval;
	scale=seascale;
	for(yloop=0;yloop<seah+1;yloop++)
	{
		x=0;
		for(xloop=0;xloop<seaw+1;xloop++)
		{
			grid[xloop][yloop][0]=(x>>8)+0;
			grid[xloop][yloop][1]=(y>>8)+(msin(waveval)/scale);
			x+=xstep;
		}
		y+=ystep;
		scale+=seascalegrow;
		waveval=(waveval+seagap)&4095;
	}

	// Hmm.. let's stick the top line to the horizon shall we?
	for(xloop=0;xloop<seaw+1;xloop++)
	{
		grid[xloop][yloop][1]=HORIZON_LEVEL;
	}

	// Draw it
	fh=m_sprites->getFrameHeader(FRM_SKY);
	//fh=m_sprites->getFrameHeader(FRM__SBLOGO);
	xstep=(256<<8)/seaw;
	ystep=(256<<8)/seah;
	y=0;
	for(yloop=0;yloop<seah;yloop++)
	{
		x=0;
		for(xloop=0;xloop<seaw;xloop++)
		{
			ft4=prepareSeaPortionFT4(fh,x>>8,y>>8,xstep>>8,ystep>>8);
			ft4->x0=grid[xloop  ][yloop  ][0];
			ft4->y0=grid[xloop  ][yloop  ][1];
			ft4->x1=grid[xloop+1][yloop  ][0];
			ft4->y1=grid[xloop+1][yloop  ][1];
			ft4->x3=grid[xloop+1][yloop+1][0];
			ft4->y3=grid[xloop+1][yloop+1][1];
			ft4->x2=grid[xloop  ][yloop+1][0];
			ft4->y2=grid[xloop  ][yloop+1][1];
			AddPrimToList(ft4,1000);
			x+=xstep;
		}
		y+=ystep;
	}

	//////////////////////// bg
	POLY_F4	*f4;
	f4=GetPrimF4();
	setXYWH(f4,0,HORIZON_LEVEL,512,256-HORIZON_LEVEL);
	setRGB0(f4,80,70,60);
	AddPrimToList(f4,1020);
	//////////////////////// bg
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CFrontEndMainTitles::renderPressStart()
{
	if(!CFader::isFading())
	{
		m_bigFont->print(256,PRESS_START_TEXT_Y,STR__FRONTEND__PRESS_START);
	}
}


/*===========================================================================
 end */