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

#ifndef __PRIM_HEADER__
#include "gfx\prim.h"
#endif

#ifndef	__PLAYER_DEMOPLAY_H__
#include "player\demoplay.h"
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

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CDemoGameScene::createPlayer()
{
	m_player=new ("player") CDemoPlayer();
	((CDemoPlayer*)m_player)->loadControlData(DEMO_DEMO_____DMO);	// Ooo
}








/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CFrontEndDemoMode::init()
{
	m_smallFont=new ("Demomode SmallFont") FontBank();
	m_smallFont->initialise(&standardFont);
	m_smallFont->setJustification(FontBank::JUST_CENTRE);
	m_smallFont->setOt(500);
	m_smallFont->setWobble(true);
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
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CFrontEndDemoMode::select()
{
	m_gameScene=new ("Demo Game Scene") CDemoGameScene();
	m_gameScene->init();
	m_shuttingDown=false;
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
	m_gameScene->shutdown();	delete m_gameScene;
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

	m_gameScene->render();

	// Game logo/title
	CFrontEndScene::renderLogo();

	if(!CFader::isFading())
	{
		m_smallFont->setColour(PRESS_START_TEXT_R,PRESS_START_TEXT_G,PRESS_START_TEXT_B);
		m_smallFont->print(256,PRESS_START_TEXT_Y,STR__FRONTEND__PRESS_START);
	}
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CFrontEndDemoMode::think(int _frames)
{
	m_gameScene->think(_frames);

	if(!m_shuttingDown&&!CFader::isFading())
	{
		if(PadGetDown(0)&PAD_START||
		   ((CDemoPlayer*)m_gameScene->getPlayer())->getFramesLeft()<FADE_OUT_FRAMES)
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