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


/*	Std Lib
	------- */

/*	Data
	---- */

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
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CFrontEndScene::shutdown()
{
	for(int i=0;i<MODE__NONE;i++)
	{
		s_modeCodes[i]->shutdown();
	}
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
void CFrontEndScene::setMode(FrontEndMode _newMode)
{
PAUL_DBGMSG("CFrontEndScene::setMode(%d)",_newMode);

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