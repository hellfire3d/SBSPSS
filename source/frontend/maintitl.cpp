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

#ifndef __GUI_GUI_H__
#include "gui\gui.h"
#endif

#ifndef __GUI_GTEXTBOX_H__
#include "gui\gtextbox.h"
#endif

#ifndef __GUI_GFRAME_H__
#include "gui\gframe.h"
#endif

#ifndef __GUI_GBUTTON_H__
#include "gui\gbutton.h"
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
int seaw=1;
int seah=50;
int seaspeed=20;
int seagap=120;
int seascale=4200;
int	seascalegrow=-50;
int colourbase=64;
int colourscale=4000;
int colourpostscale=10;

int sval=0;
///////////////


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int mem;
void CFrontEndMainTitles::init()
{
mem=MainRam.TotalRam-MainRam.RamUsed;
PAUL_DBGMSG("initial mem free=%d",mem);
	CGUIGroupFrame		*fr;
	CGUITextBox			*tb;
	CGUIToggleButton	*tg;

	m_sprites=new ("MainTitle Sprites") SpriteBank();
	m_sprites->load(FRONTEND_FRONTEND_SPR);

	m_smallFont=new ("MainTitle SmallFont") FontBank();
	m_smallFont->initialise(&standardFont);
	m_smallFont->setJustification(FontBank::JUST_CENTRE);

	m_mode=MODE__PRESS_START;

	// Create the main menu ( START GAME/OPTIONS )
	m_mainMenu=new ("Main Menu GUI") CGUIControlFrame();
	m_mainMenu->init(NULL);
	m_mainMenu->setObjectXYWH(106,140,300,40);
	m_mainMenu->clearFlags(CGUIObject::FLAG_DRAWBORDER);

	fr=new ("frame") CGUIGroupFrame();
	fr->init(m_mainMenu);
	fr->setObjectXYWH(50,0,200,20);
		tb=new ("textbox") CGUITextBox();
		tb->init(fr);
		tb->setObjectXYWH(0,0,200,20);
		tb->setText(STR__FRONTEND__START_GAME);
		tg=new ("togglebutton") CGUIToggleButton();
		tg->init(fr);
		tg->setButtonTarget(&m_startGameFlag);

	fr=new ("frame") CGUIGroupFrame();
	fr->init(m_mainMenu);
	fr->setObjectXYWH(50,20,200,20);
		tb=new ("textbox") CGUITextBox();
		tb->init(fr);
		tb->setObjectXYWH(0,0,200,20);
		tb->setText(STR__FRONTEND__OPTIONS);
		tg=new ("togglebutton") CGUIToggleButton();
		tg->init(fr);
		tg->setButtonTarget(&m_gotoOptionsFlag);

	m_mainMenu->select();
	
	m_startGameFlag=false;
	m_gotoOptionsFlag=false;

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
	m_mainMenu->shutdown();		// GUI items delete themselves when shutdown..
	m_smallFont->dump();		delete m_smallFont;
	m_sprites->dump();			delete m_sprites;
PAUL_DBGMSG("change=%d",mem-(MainRam.TotalRam-MainRam.RamUsed));
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
int isx=39;
int isy=3;
int isw=100;
int ish=56;

int rsx=4096;
int rsy=4096;
int rsr=0;
int	rspeed=0;

void CFrontEndMainTitles::render()
{
	sFrameHdr	*fh;
	POLY_FT4	*ft4;

	// Game logo/title
	fh=m_sprites->getFrameHeader(FRM__SBLOGO);
//	m_sprites->printFT4(fh,256-(fh->W/2),LOGO_CENTRE_Y-(fh->H/2),0,0,220);
	m_sprites->printRotatedScaledSprite(fh,256,LOGO_CENTRE_Y,rsx,rsy,rsr,220);
	m_smallFont->setColour(GAME_TITLE_TEXT_R,GAME_TITLE_TEXT_G,GAME_TITLE_TEXT_B);
	m_smallFont->print(256,GAME_TITLE_TEXT_CENTRE_Y,STR__FRONTEND__GAME_TITLE);
	m_smallFont->setColour(0,0,0);
	m_smallFont->print(256+1,GAME_TITLE_TEXT_CENTRE_Y+1,STR__FRONTEND__GAME_TITLE);


	// The island
	fh=m_sprites->getFrameHeader(FRM__ISLAND);
	m_sprites->printFT4(fh,ISLAND_LEFT_X,ISLAND_BOTTOM_Y-(fh->H),0,0,221);

	// Sky
	fh=m_sprites->getFrameHeader(FRM_SKY);
	for(int i=0;i<posnum;i++)
	{
		ft4=m_sprites->printFT4(fh,0,0,0,0,1020-i);
		setXYWH(ft4,pos[i].x,pos[i].y,512,HORIZON_LEVEL);
		if(i)
			setSemiTrans(ft4,true);
	}

	renderSeaSection(m_sprites->getFrameHeader(FRM__ISLAND),isx,HORIZON_LEVEL+isy,isw,ish);
	renderSeaSection(m_sprites->getFrameHeader(FRM_SKY),0,HORIZON_LEVEL,512,256-HORIZON_LEVEL+SEA_OVERLAP);

	switch(m_mode)
	{
		case MODE__PRESS_START:
			renderPressStart();
			break;
			
		case MODE__SELECT_OPTION:
			m_mainMenu->render();
			break;

		default:
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
rsr=(rsr+(_frames*rspeed))&4095;
	sval=(sval+(seaspeed*_frames))&4095;

	switch(m_mode)
	{
		case MODE__PRESS_START:
			if(!CFader::isFading()&&PadGetDown(0)&PAD_START)
			{
				m_mode=MODE__SELECT_OPTION;
			}
			break;

		case MODE__SELECT_OPTION:
			m_mainMenu->think(_frames);
			if(m_startGameFlag)
			{
				CFader::setFadingOut();
				m_mode=MODE__START_GAME;
			}
			else if(m_gotoOptionsFlag)
			{
				CFader::setFadingOut();
				m_mode=MODE__GOTO_OPTIONS;
			}
			break;

		default:
			break;
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int CFrontEndMainTitles::isReadyToExit()
{
	return !CFader::isFading()&&(m_mode==MODE__START_GAME||m_mode==MODE__GOTO_OPTIONS);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
CFrontEndScene::FrontEndMode CFrontEndMainTitles::getNextMode()
{
	CFrontEndScene::FrontEndMode	ret;

	ret=CFrontEndScene::MODE__NONE;
	switch(m_mode)
	{
		case MODE__PRESS_START:
		case MODE__SELECT_OPTION:
			ASSERT(0);
			break;

		case MODE__START_GAME:
			ret=CFrontEndScene::MODE__MAIN_TITLES;//MODE__CHOOSE_SLOT;
			break;

		case MODE__GOTO_OPTIONS:
			ret=CFrontEndScene::MODE__MAIN_TITLES;//MODE__GAME_OPTIONS;
			break;
	}

	return ret;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
POLY_FT4 *CFrontEndMainTitles::prepareSeaPortionFT4(sFrameHdr *_fh,int _x,int _y,int _w,int _h,int _rgb)
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
	setRGB0(ft4,_rgb,_rgb,_rgb);

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
typedef struct
{
	int	x,y;
	int	colour;
} SeaGrid;
void CFrontEndMainTitles::renderSeaSection(sFrameHdr *_fh,int _x,int _y,int _w,int _h)
{
	int			xloop,yloop;
	int			xstep,ystep;
	int			x,y;
	int			scale;
	POLY_FT4	*ft4;
	SeaGrid		grid[seaw+1][seah+1];
	int			waveval;


	// Grow the grid
	xstep=(_w<<8)/seaw;
	ystep=(_h<<8)/seah;
	y=_y<<8;
	waveval=sval;
	scale=seascale;
	for(yloop=0;yloop<seah+1;yloop++)
	{
		x=_x<<8;
		for(xloop=0;xloop<seaw+1;xloop++)
		{
			grid[xloop][yloop].x=(x>>8)+0;
			grid[xloop][yloop].y=(y>>8)+(msin(waveval)/scale);
			grid[xloop][yloop].colour=colourbase+((msin(waveval)/colourscale)*colourpostscale);
			x+=xstep;
		}
		y+=ystep;
		scale+=seascalegrow;
		waveval=(waveval+seagap)&4095;
	}

	// Draw it
	xstep=(256<<8)/seaw;
	ystep=(256<<8)/seah;
	y=0;
	for(yloop=0;yloop<seah;yloop++)
	{
		x=0;
		for(xloop=0;xloop<seaw;xloop++)
		{
			ft4=prepareSeaPortionFT4(_fh,x>>8,y>>8,xstep>>8,ystep>>8,64);//grid[xloop][yloop].colour);
			ft4->x0=grid[xloop  ][yloop  ].x;
			ft4->y0=grid[xloop  ][yloop  ].y;
			ft4->x1=grid[xloop+1][yloop  ].x;
			ft4->y1=grid[xloop+1][yloop  ].y;
			ft4->x3=grid[xloop+1][yloop+1].x;
			ft4->y3=grid[xloop+1][yloop+1].y;
			ft4->x2=grid[xloop  ][yloop+1].x;
			ft4->y2=grid[xloop  ][yloop+1].y;
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
		m_smallFont->setColour(PRESS_START_TEXT_R,PRESS_START_TEXT_G,PRESS_START_TEXT_B);
		m_smallFont->print(256,PRESS_START_TEXT_Y,STR__FRONTEND__PRESS_START);
	}
}


/*===========================================================================
 end */