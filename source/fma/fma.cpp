/*=========================================================================

	gameover.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose:	
				
	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/


/*----------------------------------------------------------------------
	Includes
	-------- */

#include "fma\fma.h"

#ifndef __LEVEL_LEVEL_H__
#include "level\level.h"
#endif

#ifndef __THING_THING_H__
#include "thing\thing.h"
#endif

#ifndef	__GAME_CONVO_H__
#include "game\convo.h"
#endif

#ifndef __MAP_MAP_H__
#include "map\map.h"
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

CFmaScene	FmaScene;







// tmp
#include 	"pad\pads.h"
// tmp


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CFmaScene::init()
{
	CThingManager::init();
	CConversation::init();

	CConversation::registerConversationScript(SCRIPTS_CH1L1_01_DAT);

	m_level=new ("FMALevel") CLevel();
	m_level->init(25);

	m_cameraPos.vx=m_cameraPos.vy=0;
	m_readyToShutdown=false;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CFmaScene::shutdown()
{
	m_level->shutdown();	delete m_level;

	CConversation::shutdown();
	CThingManager::shutdown();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CFmaScene::render()
{
	CThingManager::renderAllThings();
	CConversation::render();
	m_level->render();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CFmaScene::think(int _frames)
{
// tmp
	if(PadGetHeld(0)&PAD_UP)
	{
		m_cameraPos.vy-=10*_frames;
	}
	else if(PadGetHeld(0)&PAD_DOWN)
	{
		m_cameraPos.vy+=10*_frames;
	}
	else if(PadGetHeld(0)&PAD_LEFT)
	{
		m_cameraPos.vx-=10*_frames;
	}
	else if(PadGetHeld(0)&PAD_RIGHT)
	{
		m_cameraPos.vx+=10*_frames;
	}
	else if(PadGetDown(0)&PAD_START)
	{
		startShutdown();
	}
// tmp

	CThingManager::thinkAllThings(_frames);
	CConversation::think(_frames);
	m_level->setCameraCentre(m_cameraPos);
	m_level->think(_frames);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int		CFmaScene::readyToShutdown()
{
	return m_readyToShutdown;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CFmaScene::startShutdown()
{
	GameState::setNextScene(&MapScene);
	m_readyToShutdown=true;
}


/*===========================================================================
 end */