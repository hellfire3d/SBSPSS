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

#ifndef	__FRONTEND_MAINTITL_H__
#include "frontend\maintitl.h"
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

#ifndef	__FRONTEND_CREDITS_H__
#include "frontend\credits.h"
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

#ifndef	__SOUND_SOUND_H__
#include "sound\sound.h"
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

static CFrontEndMainTitles		s_frontEndModeMainTitles;
static CFrontEndOptions			s_frontEndModeOptions;
static CFrontEndStart			s_frontEndStart;
static CFrontEndDemoMode		s_frontEndDemoMode;
static CFrontEndCredits			s_frontEndCredits;

CFrontEndMode	*CFrontEndScene::s_modeCodes[]=
{
	&s_frontEndModeMainTitles,		// MODE__MAIN_TITLES
	&s_frontEndModeOptions,			// MODE__GAME_OPTIONS
	&s_frontEndStart,				// MODE__CHOOSE_SLOT
	&s_frontEndDemoMode,			// MODE__DEMO
	&s_frontEndCredits,				// MODE__CREDITS

	// NULL,							// MODE__NONE
	// NULL,							// MODE__EXIT_TO_GAME
};


CFrontEndScene	FrontEndScene;


int rsx=4096;
int rsy=4096;
int rsr=0;
int	rspeed=0;
SpriteBank	*m_sprites;
FontBank	*m_font;


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CFrontEndScene::init()
{
	for(int i=0;i<MODE__NONE;i++)
	{
		s_modeCodes[i]->init();
	}

	m_exitToGame=false;
	m_mode=MODE__NONE;
	setMode(MODE__MAIN_TITLES);

	m_sprites=new ("MainTitle Sprites") SpriteBank();
	m_sprites->load(FRONTEND_FRONTEND_SPR);
	
	m_font=new ("frontendfont") FontBank();
	m_font->initialise(&standardFont);
	m_font->setJustification(FontBank::JUST_CENTRE);

	CSoundMediator::setSong(CSoundMediator::SONG_TITLE);
	CSoundMediator::playSong();
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

	CSoundMediator::dumpSong();
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
	mode->think(_frames);
	if(mode->isReadyToExit())
	{
		setMode(mode->getNextMode());
	}
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
	m_sprites->printRotatedScaledSprite(fh,256,LOGO_CENTRE_Y,rsx,rsy,rsr,220);
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
	rsr=(rsr+(_frames*rspeed))&4095;
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
		m_exitToGame=true;
		GameState::setNextScene(&FrontEndScene);
	}
	else
	{
		m_mode=_newMode;
		s_modeCodes[m_mode]->select();
	}
}


/*===========================================================================
 end */