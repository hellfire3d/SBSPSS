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

#ifndef __SYSTEM_CLICKCOUNT_H__
#include "system\clickcount.h"
#endif

#ifndef	__MEMORY_HEADER__
#include "mem\memory.h"
#endif


/*****************************************************************************/
static CScene	*s_currentScene;
static CScene	*s_pendingScene;

int		GameState::s_timeSinceLast;
static	CClickCount	s_clickCounter;

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
				SYSTEM_DBGMSG("GameState: Closing down scene..");
#ifdef __VERSION_DEBUG__
				int gain;
				gain=MainRam.RamUsed-s_baseSceneMemory;
				if(gain)
				{
					SYSTEM_DBGMSG("Scene allocated an extra %d bytes after init()",gain);
				}
#endif
				ASSERT(s_pendingScene);		// There really should be a scene pending before you shutdown..!
				s_currentScene->shutdown();
				s_currentScene=NULL;
#ifdef __VERSION_DEBUG__
				int loss;
				loss=MainRam.RamUsed-s_baseMemory;
				if(loss)
				{
					SYSTEM_DBGMSG("MEMORY HAS CHANGED BY %d BYTES DURING SCENE!",loss);
					ASSERT(0);
					s_baseMemory=0;
				}
#endif
			}
		}
		if(!s_currentScene)
		{
#ifdef __VERSION_DEBUG__
			if(s_baseMemory==0)
			{
				s_baseMemory=MainRam.RamUsed;
			}
			SYSTEM_DBGMSG("GameState: Opening new scene ( with %d bytes used )",s_baseMemory);
#endif
			s_currentScene=s_pendingScene;
			s_currentScene->init();
#ifdef __VERSION_DEBUG__
			s_baseSceneMemory=MainRam.RamUsed;
			SYSTEM_DBGMSG("GameState: Scene has used %d bytes during init()",s_baseSceneMemory-s_baseMemory);
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
void GameState::setNextScene( CScene *_nextScene )
{
	ASSERT(!s_pendingScene);

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
static int s_timeSpeed = ONE;
void GameState::updateTimer()
{
	s_timeSinceLast = (s_clickCounter.timeSinceLast() * s_timeSpeed) >> 12;
	
	if (s_timeSinceLast > 4 * 4096)
	{
		s_timeSinceLast = 4 * 4096;
		SYSTEM_DBGMSG("updateTimer loosing frames!");		
	}
}


/*===========================================================================
 end */