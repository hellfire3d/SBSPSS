/*=========================================================================

	demomode.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/


/*----------------------------------------------------------------------
	Includes
	-------- */

#include "frontend\demomode.h"

#ifndef __GFX_SPRBANK_H__
#include "gfx\sprbank.h"
#endif

#ifndef __GFX_FONT_H__
#include "gfx\font.h"
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

#ifndef	__UTILS_HEADER__
#include "utils\utils.h"
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
static int count;

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CFrontEndDemoMode::init()
{
	m_sprites=new ("Demomode Sprites") SpriteBank();
	m_sprites->load(FRONTEND_FRONTEND_SPR);

	m_smallFont=new ("Demomode SmallFont") FontBank();
	m_smallFont->initialise(&standardFont);
	m_smallFont->setJustification(FontBank::JUST_CENTRE);
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CFrontEndDemoMode::shutdown()
{
	m_smallFont->dump();		delete m_smallFont;
	m_sprites->dump();			delete m_sprites;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CFrontEndDemoMode::select()
{
	// LOAD UP DEMO AND DEMO LEVEL
	m_shuttingDown=false;
count=0;
	CFader::setFadingIn();
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CFrontEndDemoMode::unselect()
{
	// DUMP DEMO AND LEVEL
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CFrontEndDemoMode::render()
{
	sFrameHdr	*fh;
	POLY_FT4	*ft4;

	// Game logo/title
	fh=m_sprites->getFrameHeader(FRM__SBLOGO);
	m_sprites->printFT4(fh,256-(fh->W/2),LOGO_CENTRE_Y-(fh->H/2),0,0,220);
	m_smallFont->setColour(GAME_TITLE_TEXT_R,GAME_TITLE_TEXT_G,GAME_TITLE_TEXT_B);
	m_smallFont->print(256,GAME_TITLE_TEXT_CENTRE_Y,STR__FRONTEND__GAME_TITLE);
	m_smallFont->setColour(0,0,0);
	m_smallFont->print(256+1,GAME_TITLE_TEXT_CENTRE_Y+1,STR__FRONTEND__GAME_TITLE);

	if(!CFader::isFading())
	{
		m_smallFont->setColour(PRESS_START_TEXT_R,PRESS_START_TEXT_G,PRESS_START_TEXT_B);
		m_smallFont->print(256,PRESS_START_TEXT_Y,STR__FRONTEND__PRESS_START);
	}


	m_smallFont->setColour(getRndRange(255),getRndRange(255),getRndRange(255));
	m_smallFont->print(getRndRange(512),getRndRange(256),">DEMO MODE<");
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CFrontEndDemoMode::think(int _frames)
{
	if(!m_shuttingDown)
	{
	count+=_frames;	
		if(PadGetDown(0)&PAD_START||count>60*20)		// OR DEMO ENDED
		{
			m_shuttingDown=true;
			CFader::setFadingOut();
		}
	}
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int CFrontEndDemoMode::isReadyToExit()
{
	return !CFader::isFading()&&m_shuttingDown;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
CFrontEndScene::FrontEndMode CFrontEndDemoMode::getNextMode()
{
	return	CFrontEndScene::MODE__MAIN_TITLES;
}

/*===========================================================================
 end */