/*=========================================================================

	gstate.cpp

	Author:		PKG
	Created: 
	Project:	PRLSR
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/


#include "system\global.h"
#include "system\gstate.h"

#ifndef	__MEMORY_HEADER__
#include "mem\memory.h"
#endif

#ifndef __VID_HEADER_
#include "system\vid.h"
#endif

#ifndef __PAD_PADS_H__
#include "pad\pads.h"
#endif

#ifndef	__PAD_VIBE_H__
#include "pad\vibe.h"
#endif


/*****************************************************************************/


// Use click counter or vbl counter
//#define USE_CLICK_COUNTER

#ifdef USE_CLICK_COUNTER
#ifndef __SYSTEM_CLICKCOUNT_H__
#include "system\clickcount.h"
#endif
#endif


/*****************************************************************************/
static CScene	*s_currentScene;
static CScene	*s_pendingScene;

#ifdef USE_CLICK_COUNTER
static	CClickCount	s_clickCounter;
#endif
int		GameState::s_framesSinceLast=1;

#ifdef __VERSION_DEBUG__
static int		s_baseMemory=0;
static int		s_baseSceneMemory=0;
#endif



/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void GameState::initialise()
{
	s_currentScene=NULL;
	s_pendingScene=NULL;
}


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void GameState::think()
{
	updateTimer();


	if(s_pendingScene)
	{
		if(s_currentScene)
		{
			if(s_currentScene->readyToShutdown())
			{
				SYSTEM_DBGMSG("GameState: Closing down scene '%s'..",s_currentScene->getSceneName());
#ifdef __VERSION_DEBUG__
				int gain;
				gain=MainRam.RamUsed-s_baseSceneMemory;
				if(gain)
				{
					SYSTEM_DBGMSG("Scene '%s' allocated an extra %d bytes after init()",s_currentScene->getSceneName(),gain);
				}
#endif
				ASSERT(s_pendingScene);		// There really should be a scene pending before you shutdown..!
				s_currentScene->shutdown();
				CPadVibrationManager::stopAllVibration();
				PadUpdate();
#ifdef __VERSION_DEBUG__
				int loss;
				loss=MainRam.RamUsed-s_baseMemory;
				if(loss)
				{
					SYSTEM_DBGMSG("MEMORY HAS CHANGED BY %d BYTES DURING SCENE '%s'!",loss,s_currentScene->getSceneName());
					ASSERT(!"MEMORY CHANGED!");
					s_baseMemory=0;
				}
#endif
				s_currentScene=NULL;
			}
		}
		if(!s_currentScene)
		{
			s_currentScene=s_pendingScene;
#ifdef __VERSION_DEBUG__
			if(s_baseMemory==0)
			{
				s_baseMemory=MainRam.RamUsed;
			}
			SYSTEM_DBGMSG("GameState: Opening new scene '%s' ( with %d bytes used )",s_currentScene->getSceneName(),s_baseMemory);
#endif
			s_currentScene->init();
#ifdef __VERSION_DEBUG__
			s_baseSceneMemory=MainRam.RamUsed;
			SYSTEM_DBGMSG("GameState: Scene '%s' has used %d bytes during init()",s_currentScene->getSceneName(),s_baseSceneMemory-s_baseMemory);
#endif
			s_pendingScene=NULL;
		}
	}

	ASSERT(s_currentScene);
	s_currentScene->think(getFramesSinceLast());
}


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void GameState::render()
{
	ASSERT(s_currentScene);
	s_currentScene->render();
}


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
// Define this to make the game go to the scene selecter after every scene
//#define _ALWAYS_GO_BACK_TO_THE_SCENE_SELECTER_

#ifdef _ALWAYS_GO_BACK_TO_THE_SCENE_SELECTER_
#include "paul\scenesel.h"
#endif

void GameState::setNextScene( CScene *_nextScene )
{
	ASSERT(!s_pendingScene);

#ifdef _ALWAYS_GO_BACK_TO_THE_SCENE_SELECTER_
	if(s_currentScene!=&SceneSelector)
	{
		_nextScene=&SceneSelector;
	}
#endif

	s_pendingScene=_nextScene;
}



/*****************************************************************************/
CScene * GameState::getCurrentScene()
{
	return s_currentScene;
}

/*****************************************************************************/
CScene * GameState::getPendingScene()
{
	return s_pendingScene;
}

/*****************************************************************************/
void GameState::updateTimer()
{
#ifdef USE_CLICK_COUNTER
	s_framesSinceLast=(s_clickCounter.timeSinceLast()>>12)/4+1;
#else
	s_framesSinceLast=VidGetVblsThisFrame();
#endif
}


/*===========================================================================
 end */