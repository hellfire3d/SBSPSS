/*=========================================================================

	frontend.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/


/*----------------------------------------------------------------------
	Includes
	-------- */

#include "frontend\frontend.h"

#ifndef	__MEMORY_HEADER__
#include "mem\memory.h"
#endif

#ifndef	__FRONTEND_NICKLOGO_H__
#include "frontend\nicklogo.h"
#endif

#ifndef	__FRONTEND_MAINTITL_H__
#include "frontend\maintitl.h"
#endif

#ifndef	__FRONTEND_FMVAD_H__
#include "frontend\fmvad.h"
#endif

#ifndef	__FRONTEND_FMVTHQ_H__
#include "frontend\fmvthq.h"
#endif

#ifndef	__FRONTEND_FMVINTRO_H__
#include "frontend\fmvintro.h"
#endif

#ifndef	__FRONTEND_OPTIONS_H__
#include "frontend\options.h"
#endif

#ifndef	__FRONTEND_START_H__
#include "frontend\start.h"
#endif

#ifndef	__FRONTEND_DEMOMODE_H__
#include "frontend\demomode.h"
#endif

#ifndef __GFX_SPRBANK_H__
#include "gfx\sprbank.h"
#endif

#ifndef __GFX_FONT_H__
#include "gfx\font.h"
#endif

#ifndef __LOCALE_TEXTDBASE_H__
#include "locale\textdbase.h"
#endif

#ifndef __DATA_STRUCTS_HEADER__
#include "Dstructs.h"
#endif

#ifndef __MAP_MAP_H__
#include "map\map.h"
#endif

#ifndef	__GAME_GAMESLOT_H__
#include "game\gameslot.h"
#endif

#ifndef __FMA_FMA_H__
#include "fma\fma.h"
#endif

#ifndef __VID_HEADER_
#include "system\vid.h"
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

static CFrontEndNickLogo		s_nickLogo;
static CFrontEndFMVAdvert		s_frontEndFMVAdvert;
static CFrontEndFMVTHQ			s_frontEndFMVTHQ;
static CFrontEndFMVIntro		s_frontEndFMVIntro;
static CFrontEndMainTitles		s_frontEndModeMainTitles;
static CFrontEndOptions			s_frontEndModeOptions;
static CFrontEndStart			s_frontEndStart;
static CFrontEndDemoMode		s_frontEndDemoMode;

CFrontEndMode	*CFrontEndScene::s_modeCodes[]=
{
	&s_nickLogo,					// MODE__NICK_LOGO
	&s_frontEndFMVAdvert,			// MODE__FMV_ADVERT
	&s_frontEndFMVTHQ,				// MODE__FMV_THQ
	&s_frontEndFMVIntro,			// MODE__FMV_INTRO
	&s_frontEndModeMainTitles,		// MODE__MAIN_TITLES
	&s_frontEndModeOptions,			// MODE__GAME_OPTIONS
	&s_frontEndStart,				// MODE__CHOOSE_SLOT
	&s_frontEndDemoMode,			// MODE__DEMO

	// NULL,							// MODE__NONE
	// NULL,							// MODE__EXIT_TO_GAME
};


CFrontEndScene	FrontEndScene;


//int rsx=4096;
//int rsy=4096;
//int rsr=0;
//int rspeed=0;
SpriteBank	*m_sprites;
FontBank	*m_font;

CFrontEndScene::FrontEndMode			CFrontEndScene::s_startMode=MODE__NICK_LOGO;
int		CFrontEndScene::s_bootUp=1;

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CFrontEndScene::init()
{
	if (s_bootUp)
	{ // Dont display ingame loading screen on bootup
		s_bootUp=0;
	}
	else
	{
		CLevel::DisplayLoadingScreen();
	}

	for(int i=0;i<MODE__NONE;i++)
	{
		s_modeCodes[i]->init();
	}

	m_exitToGame=false;
	m_mode=MODE__NONE;

	setMode(s_startMode);

	m_sprites=new ("MainTitle Sprites") SpriteBank();
	m_sprites->load(SPRITES_SPRITES_SPR);
	
	m_font=new ("frontendfont") FontBank();
	m_font->initialise(&standardFont);
	m_font->setJustification(FontBank::JUST_CENTRE);
	StopLoad();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CFrontEndScene::shutdown()
{
	m_font->dump();				delete m_font;
	m_sprites->dump();			delete m_sprites;

	for(int i=0;i<MODE__NONE;i++)
	{
		s_modeCodes[i]->shutdown();
	}

	// Nex time.. go into this mode first
	setStartMode(MODE__FMV_INTRO);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CFrontEndScene::render()
{
	s_modeCodes[m_mode]->render();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CFrontEndScene::think(int _frames)
{
	CFrontEndMode	*mode;

	thinkLogo(_frames);
	
	mode=s_modeCodes[m_mode];
	if(mode->isReadyToExit())
	{
		setMode(mode->getNextMode());
		mode=s_modeCodes[m_mode];
	}

	mode->think(_frames);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int CFrontEndScene::readyToShutdown()
{
	return m_exitToGame;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CFrontEndScene::renderLogo()
{
	sFrameHdr	*fh;

	fh=m_sprites->getFrameHeader(FRM__SBLOGO);
//	m_sprites->printRotatedScaledSprite(fh,256,LOGO_CENTRE_Y,rsx,rsy,rsr,20);
	m_sprites->printFT4(fh,256-(fh->W/2),LOGO_CENTRE_Y-(fh->H/2),0,0,FE_OT__LOGO);
	m_font->setColour(GAME_TITLE_TEXT_R,GAME_TITLE_TEXT_G,GAME_TITLE_TEXT_B);
	m_font->print(256,GAME_TITLE_TEXT_CENTRE_Y,STR__FRONTEND__GAME_TITLE);
	m_font->setColour(0,0,0);
	m_font->print(256+1,GAME_TITLE_TEXT_CENTRE_Y+1,STR__FRONTEND__GAME_TITLE);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CFrontEndScene::thinkLogo(int _frames)
{
//	rsr=(rsr+(_frames*rspeed))&4095;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CFrontEndScene::setMode(FrontEndMode _newMode)
{
	// Close old mode
	if(m_mode!=MODE__NONE)
	{
		s_modeCodes[m_mode]->unselect();
	}
	
	// Open new mode
	if(_newMode==MODE__EXIT_TO_GAME)
	{
		CGameSlotManager::GameSlot	*gameSlot;
		gameSlot=CGameSlotManager::getSlotData();

		if(gameSlot->m_hasSeenOpeningFMA)
		{
			GameState::setNextScene(&MapScene);
		}
		else
		{
			CFmaScene::selectFma(CFmaScene::FMA_SCRIPT__INTRO);
			GameState::setNextScene(&FmaScene);
			gameSlot->m_hasSeenOpeningFMA=true;
		}

		m_exitToGame=true;
	}
	else
	{
		m_mode=_newMode;
		s_modeCodes[m_mode]->select();
	}
}


/*===========================================================================
 end */