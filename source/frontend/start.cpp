/*=========================================================================

	start.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/


/*----------------------------------------------------------------------
	Includes
	-------- */

#include "frontend\start.h"

#ifndef __GFX_SPRBANK_H__
#include "gfx\sprbank.h"
#endif

#ifndef __LOCALE_TEXTDBASE_H__
#include "locale\textdbase.h"
#endif

#ifndef	__GFX_FADER_H__
#include "gfx\fader.h"
#endif

#ifndef __PRIM_HEADER__
#include "gfx\prim.h"
#endif

#ifndef __GUI_GFACTORY_H__
#include "gui\gfactory.h"
#endif

#ifndef __GUI_GFRAME_H__
#include "gui\gframe.h"
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

int CFrontEndStart::s_levelValues[]=
{
	0,48,96,144,192,240,
	-1,
};
CGUITextReadout::TextReadoutData CFrontEndStart::s_levelReadoutText[]=
{
	{ 0,	STR__DEBUG__LEVEL_SELECT_C1 },
	{ 48,	STR__DEBUG__LEVEL_SELECT_C2 },
	{ 96,	STR__DEBUG__LEVEL_SELECT_C3 },
	{ 144,	STR__DEBUG__LEVEL_SELECT_C4 },
	{ 192,	STR__DEBUG__LEVEL_SELECT_C5 },
	{ 240,	STR__DEBUG__LEVEL_SELECT_C6 },
};

extern int s_globalLevelSelectThing;


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CFrontEndStart::init()
{
	m_guiFrame=new ("optionsframe") CGUIControlFrame();
	m_guiFrame->init(NULL);
	m_guiFrame->setObjectXYWH((512-352)/2,50,352,96);

	CGUIFactory::createCycleButtonFrame(m_guiFrame,
										10,10,332,40,
										STR__DEBUG__LEVEL_SELECT,
										&m_level,s_levelValues,s_levelReadoutText);

	CGUIFactory::createValueButtonFrame(m_guiFrame,
										10,50,332,40,
										STR__DEBUG__LEVEL_SELECT_START,
										&m_startGame,true);
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CFrontEndStart::shutdown()
{
	m_guiFrame->shutdown();
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CFrontEndStart::select()
{
	m_guiFrame->select();
	m_shuttingDown=false;
	m_selectedSlot=NO_SLOT_SELECTED;
	m_escapeToTitles=false;
	CFader::setFadingIn();
	m_startGame=false;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CFrontEndStart::unselect()
{
	m_guiFrame->unselect();
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CFrontEndStart::render()
{
	POLY_G4	*g4;
	
	g4=GetPrimG4();
	setXYWH(g4,0,0,512,256);
	setRGB0(g4,50, 0, 0);
	setRGB1(g4, 0, 0,50);
	setRGB2(g4, 0,50, 0);
	setRGB3(g4,50, 0,50);
	AddPrimToList(g4,MAX_OT-1);

	m_guiFrame->render();
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CFrontEndStart::think(int _frames)
{
	if(!CFader::isFading())
	{
		m_guiFrame->think(_frames);
	}

	if(!m_shuttingDown)
	{
		if(/*m_selectedSlot!=NO_SLOT_SELECTED||
		   m_escapeToTitles||*/
		   m_startGame)
		{			
			m_selectedSlot=0;
			m_shuttingDown=true;
			s_globalLevelSelectThing=m_level;
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
int CFrontEndStart::isReadyToExit()
{
	return !CFader::isFading()&&m_shuttingDown;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
CFrontEndScene::FrontEndMode CFrontEndStart::getNextMode()
{
	if(m_selectedSlot==NO_SLOT_SELECTED)
	{
		return CFrontEndScene::MODE__MAIN_TITLES;
	}
	else
	{
		return CFrontEndScene::MODE__EXIT_TO_GAME;
	}
}

/*===========================================================================
 end */