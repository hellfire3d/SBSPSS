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
	 0,
	 1,
	 2,
	 3,
	 4,
	 5,
	 6,
	 7,
	 8,
	 9,
	10,
	11,
	12,
	13,
	14,
	15,
	16,
	17,
	18,
	19,
	20,
	21,
	22,
	23,
	24,
	25,
	26,
	27,
	28,
	29,
	30,
	31,
	32,
	33,
	34,
	35,
	36,
	37,
	38,
	39,
	40,
	41,
	42,
	43,
	44,
	45,
	46,
	47,
	-1,
};
CGUITextReadout::TextReadoutData CFrontEndStart::s_levelReadoutText[]=
{
	{ 0,STR__DEBUG__LEVEL_SELECT_C3L301},
	{ 1,STR__DEBUG__LEVEL_SELECT_C3L302},
	{ 2,STR__DEBUG__LEVEL_SELECT_C3L303},
	{ 3,STR__DEBUG__LEVEL_SELECT_C3L304},
	{ 4,STR__DEBUG__LEVEL_SELECT_C3L305},
	{ 5,STR__DEBUG__LEVEL_SELECT_C3L306},

	{ 6,STR__DEBUG__LEVEL_SELECT_C3L401},
	{ 7,STR__DEBUG__LEVEL_SELECT_C3L402},
	{ 8,STR__DEBUG__LEVEL_SELECT_C3L403},
	{ 9,STR__DEBUG__LEVEL_SELECT_C3L404},
	{10,STR__DEBUG__LEVEL_SELECT_C3L404X},
	{11,STR__DEBUG__LEVEL_SELECT_C3L405},
	{12,STR__DEBUG__LEVEL_SELECT_C3L406},

	{13,STR__DEBUG__LEVEL_SELECT_C4L101},
	{14,STR__DEBUG__LEVEL_SELECT_C4L102},
	{15,STR__DEBUG__LEVEL_SELECT_C4L103},
	{16,STR__DEBUG__LEVEL_SELECT_C4L104},

	{17,STR__DEBUG__LEVEL_SELECT_C4L201},
	{18,STR__DEBUG__LEVEL_SELECT_C4L202},
	{19,STR__DEBUG__LEVEL_SELECT_C4L203},
	{20,STR__DEBUG__LEVEL_SELECT_C4L204},

	{21,STR__DEBUG__LEVEL_SELECT_C4L301},
	{22,STR__DEBUG__LEVEL_SELECT_C4L302},
	{23,STR__DEBUG__LEVEL_SELECT_C4L303},
	{24,STR__DEBUG__LEVEL_SELECT_C4L304},
	{25,STR__DEBUG__LEVEL_SELECT_C4L305},
	{26,STR__DEBUG__LEVEL_SELECT_C4L306},

	{27,STR__DEBUG__LEVEL_SELECT_C4L401},
	{28,STR__DEBUG__LEVEL_SELECT_C4L402},
	{29,STR__DEBUG__LEVEL_SELECT_C4L403},
	{30,STR__DEBUG__LEVEL_SELECT_C4L404},
	{31,STR__DEBUG__LEVEL_SELECT_C4L405},
	{32,STR__DEBUG__LEVEL_SELECT_C4L406},
	{33,STR__DEBUG__LEVEL_SELECT_C4L407},
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
	m_guiFrame->setObjectXYWH(50,40,412,100);

	CGUIFactory::createCycleButtonFrame(m_guiFrame,
										50,10,312,40,
										STR__DEBUG__LEVEL_SELECT,
										&m_level,s_levelValues,s_levelReadoutText);

	CGUIFactory::createValueButtonFrame(m_guiFrame,
										50,50,312,40,
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