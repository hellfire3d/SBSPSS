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

#ifndef __GUI_GFACTORY_H__
#include "gui\gfactory.h"
#endif

#ifndef __GUI_GFRAME_H__
#include "gui\gframe.h"
#endif

#ifndef _FILEIO_HEADER_
#include "fileio\fileio.h"
#endif

#ifndef __VID_HEADER_
#include "system\vid.h"
#endif

#ifndef	__SOUND_SOUND_H__
#include "sound\sound.h"
#endif

#ifndef	__FRONTEND_FRONTEND_H__
#include "frontend\frontend.h"
#endif

#ifndef	__UTILS_HEADER__
#include "utils\utils.h"
#endif

#ifndef	__GAME_GAMESLOT_H__
#include "game\gameslot.h"
#endif


/*	Std Lib
	------- */

/*	Data
	---- */

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

///////////////
/*
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
*/
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
	m_sprites->load(SPRITES_SPRITES_SPR);

	m_smallFont=new ("MainTitle SmallFont") FontBank();
	m_smallFont->initialise(&standardFont);
	m_smallFont->setJustification(FontBank::JUST_CENTRE);
	m_smallFont->setOt(5);

	// Create the main menu ( START GAME/OPTIONS )
	m_mainMenu=new ("Main Menu GUI") CGUIControlFrame();
	m_mainMenu->init(NULL);
	m_mainMenu->setObjectXYWH(106,145,300,60);
	m_mainMenu->clearFlags(CGUIObject::FLAG_DRAWBORDER);

	CGUIFactory::createValueButtonFrame(m_mainMenu,
										50,0,200,20,
										STR__FRONTEND__START_GAME,
										&m_startGameFlag,true);
	CGUIFactory::createValueButtonFrame(m_mainMenu,
										50,20,200,20,
										STR__FRONTEND__OPTIONS,
										&m_gotoOptionsFlag,true);
#if defined(__TERRITORY_EUR__)
	CGUIFactory::createValueButtonFrame(m_mainMenu,
										50,40,200,20,
										STR__FRONTEND__PLAY_ADVERT,
										&m_gotoAdvertFlag,true);
#endif
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
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
static u8 *s_image;
static FileEquate	s_imageFiles[4]=
{
	BACKDROP_START1_GFX,
	BACKDROP_START2_GFX,
	BACKDROP_START3_GFX,
	BACKDROP_START4_GFX,
};
void CFrontEndMainTitles::select()
{
	m_mode=MODE__PRESS_START;

	m_startGameFlag=false;
	m_gotoOptionsFlag=false;
#if defined(__TERRITORY_EUR__)
	m_gotoAdvertFlag=false;
#endif

	m_demoTimeout=0;

	s_image=LoadPakScreen(s_imageFiles[CGameSlotManager::getNumberOfFrontendScreenToUse()]);
	ASSERT(s_image);
	SetScreenImage(s_image);

	CSoundMediator::setSong(CSoundMediator::SONG_TITLE);
	m_musicStarted=false;

	CFader::setFadingIn();
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CFrontEndMainTitles::unselect()
{
	CSoundMediator::dumpSong();

	ClearScreenImage();
	MemFree(s_image);	s_image=NULL;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
/*
int isx=39;
int isy=3;
int isw=100;
int ish=56;
*/

void CFrontEndMainTitles::render()
{
	CFrontEndScene::renderLogo();
/*
	sFrameHdr	*fh;
	POLY_FT4	*ft4;

	// Game logo/title
	CFrontEndScene::renderLogo();

	// The island
	fh=m_sprites->getFrameHeader(FRM__ISLAND);
	m_sprites->printFT4(fh,ISLAND_LEFT_X,ISLAND_BOTTOM_Y-(fh->H),0,0,FE_OT__SEAPOS);

	// Sea
	renderSeaSection(m_sprites->getFrameHeader(FRM__ISLAND),isx,HORIZON_LEVEL+isy,isw,ish);
	renderSeaSection(m_sprites->getFrameHeader(FRM_SKY),0,HORIZON_LEVEL,512,256-HORIZON_LEVEL+SEA_OVERLAP);
*/

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
/*
	sval=(sval+(seaspeed*_frames))&4095;
*/

	// Wait until fade in has finished before starting music
	if(!m_musicStarted&&!CFader::isFading())
	{
		CSoundMediator::playSong();
		m_musicStarted=true;
	}

	switch(m_mode)
	{
		case MODE__PRESS_START:
			if(!CFader::isFading()&&PadGetDown(0)&PAD_START)
			{
				CSoundMediator::playSfx(CSoundMediator::SFX_FRONT_END__OK);
				m_mode=MODE__SELECT_OPTION;
				m_mainMenu->select();
			}
			break;

		case MODE__SELECT_OPTION:
			m_mainMenu->think(_frames);
			if(m_startGameFlag)
			{
				CFader::setFadingOut();
				m_mode=MODE__GOTO_CHOOSE_SLOT;
				m_mainMenu->unselect();
			}
			else if(m_gotoOptionsFlag)
			{
				CFader::setFadingOut();
				m_mode=MODE__GOTO_OPTIONS;
				m_mainMenu->unselect();
			}
#if defined(__TERRITORY_EUR__)
			else if(m_gotoAdvertFlag)
			{
				CFader::setFadingOut();
				m_mode=MODE__GOTO_ADVERT;
				m_mainMenu->unselect();
			}
#endif
			break;

		default:
			break;
	}

	if(m_mode==MODE__PRESS_START||m_mode==MODE__SELECT_OPTION)
	{
		if(!CFader::isFading())
		{
			m_demoTimeout+=_frames;
		}

		/*
		if(!PadGetHeld(0))
		{
			if(m_demoTimeout>DEMO_TIMEOUT_IN_SECONDS*GameState::getOneSecondInFrames())
			{
				CFader::setFadingOut();
				m_mode=MODE__GOTO_DEMO;
				if(m_mainMenu->isSelected())
					m_mainMenu->unselect();
			}
		}
		else
		{
			m_demoTimeout=0;
		}
		*/
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
	return !CFader::isFading()&&(m_mode==MODE__GOTO_CHOOSE_SLOT||m_mode==MODE__GOTO_OPTIONS||m_mode==MODE__GOTO_ADVERT||m_mode==MODE__GOTO_DEMO);
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

		case MODE__GOTO_CHOOSE_SLOT:
			ret=CFrontEndScene::MODE__CHOOSE_SLOT;
			break;

		case MODE__GOTO_OPTIONS:
			ret=CFrontEndScene::MODE__GAME_OPTIONS;
			break;

		case MODE__GOTO_ADVERT:
			ret=CFrontEndScene::MODE__FMV_ADVERT;
			break;

		case MODE__GOTO_DEMO:
			ret=CFrontEndScene::MODE__DEMO;
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
/*
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
*/

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
/*
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
//			grid[xloop][yloop].colour=colourbase+((msin(waveval)/colourscale)*colourpostscale);
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
			AddPrimToList(ft4,FE_OT__SEAPOS);
			x+=xstep;
		}
		y+=ystep;
	}

//	//////////////////////// bg
//	POLY_F4	*f4;
//	f4=GetPrimF4();
//	setXYWH(f4,0,HORIZON_LEVEL,512,256-HORIZON_LEVEL);
//	setRGB0(f4,80,70,60);
//	AddPrimToList(f4,FE_OT__SEAPOS);
//	//////////////////////// bg
}
*/

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CFrontEndMainTitles::renderPressStart()
{
	if(!CFader::isFading()&&!(m_demoTimeout&32))
	{
		m_smallFont->setColour(PRESS_START_TEXT_R,PRESS_START_TEXT_G,PRESS_START_TEXT_B);
		m_smallFont->print(256,PRESS_START_TEXT_Y,STR__FRONTEND__PRESS_START);

		m_smallFont->setColour(0,0,0);
		m_smallFont->print(256-1,PRESS_START_TEXT_Y-1,STR__FRONTEND__PRESS_START);
		m_smallFont->print(256+1,PRESS_START_TEXT_Y-1,STR__FRONTEND__PRESS_START);
		m_smallFont->print(256-1,PRESS_START_TEXT_Y+1,STR__FRONTEND__PRESS_START);
		m_smallFont->print(256+1,PRESS_START_TEXT_Y+1,STR__FRONTEND__PRESS_START);
	}
}


/*===========================================================================
 end */