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

CFrontEndMode	*CFrontEndScene::s_modeCodes[]=
{
	&s_frontEndModeMainTitles,		// MODE__MAIN_TITLES
	&s_frontEndModeMainTitles,//NULL,							// MODE__GAME_OPTIONS
	&s_frontEndModeMainTitles,//NULL,							// MODE__CHOOSE_SLOT
	NULL,							// MODE__DEMO

	NULL,							// MODE__NONE
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
	return false;
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
	if(s_modeCodes[m_mode])
	{
		s_modeCodes[m_mode]->shutdown();
	}
	
	// Open new mode
	m_mode=_newMode;
	s_modeCodes[m_mode]->init();
//	s_modeCodes[m_mode]->shutdown();
//	ASSERT(0);
}


/*===========================================================================
 end */