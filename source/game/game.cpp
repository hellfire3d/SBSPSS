/**********************/
/*** Main Game File ***/
/**********************/

#include 	"system\global.h"
#include 	"fileio\fileio.h"
#include 	"pad\pads.h"
#include 	"system\vid.h"
#include 	"gfx\prim.h"
#include	"utils\utils.h"
#include	"level\level.h"
#include	"game\game.h"
#include	"system\gstate.h"
#include	"gfx\font.h"
#include	"gfx\fdata.h"

#ifndef	__GAME_CONVO_H__
#include "game\convo.h"
#endif

#ifndef __PLAYER_PLAYER_H__
#include "player\player.h"
#endif
#include "player\demoplay.h"

#ifndef	__ENEMY_NPC_H__
#include "enemy\npc.h"
#endif

#ifndef __ENEMY_NPLATFRM_H__
#include "enemy\nplatfrm.h"
#endif

#ifndef __PROJECTL_PROJECTL_H__
#include "projectl\projectl.h"
#endif

#ifndef __GFX_FADER_H__
#include "gfx\fader.h"
#endif

#ifndef __GFX_BUBICLES__
#include "gfx\bubicles.h"
#endif

#ifndef __SPR_INGAMEFX_H__
#include <ingamefx.h>
#endif

#ifndef _ANIMTEX_HEADER_
#include "gfx\animtex.h"
#endif

#ifndef	__GAME_PAUSE_H__
#include "game\pause.h"
#endif

#ifndef	__FRONTEND_FRONTEND_H__
#include "frontend\frontend.h"
#endif

#ifndef __PICKUPS_PICKUP_H__
#include "pickups\pickup.h"
#endif


#ifndef	__SOUND_SOUND_H__
#include "sound\sound.h"
#endif

#include "gfx\actor.h"


//int		GX=512/2;
//int		GY=256/;
int		GH=256;
int		ZPos=5100/16;

/*****************************************************************************/

FontBank	*CGameScene::s_genericFont;
MATRIX		CGameScene::CamMtx;

/*****************************************************************************/

#ifndef __USER_paul__
int s_globalLevelSelectThing=0;
#else
int s_globalLevelSelectThing=120;
#endif
int CGameScene::s_readyToExit;
int CGameScene::s_levelFinished;

/*****************************************************************************/

CGameScene	GameScene;

/*****************************************************************************/
void 	CGameScene::init()
{
		SetIdentNoTrans(&CamMtx);
		CamMtx.t[2]=ZPos;

		s_genericFont=new ("CGameScene::Init") FontBank();
		s_genericFont->initialise( &standardFont );
		s_genericFont->setColour( 255, 255 , 0 );
		VidSetClearScreen(0);

//		SetGeomOffset( GX, GY );
		SetGeomScreen(GH);

		m_pauseMenu=new ("Pause Menu") CPauseMenu();
		m_pauseMenu->init();

		s_readyToExit=false;

		CFader::setFadingIn();

		initLevel();
		CFileIO::EnableASync(true);

}

/*****************************************************************************/
// This is a seperate funtion ( and virtual ) so that we can overload it for
// the demo mode (pkg)
// NB: This function should *only* include the new function - Don't add anything else here!
void	CGameScene::createPlayer()
{
		m_player=new ("player") CPlayer();
}

// This is also to be overloaded for demomode.. to stop the pause menu appearing
int		CGameScene::canPause()
{
		return true;
}


/*****************************************************************************/

void	CGameScene::shutdown()
{
		CFileIO::EnableASync(false);
		shutdownLevel(true);
		CSoundMediator::dumpSong();	

		m_pauseMenu->shutdown();	delete m_pauseMenu;
		s_genericFont->dump();		delete s_genericFont;
}

/*****************************************************************************/
void 	CGameScene::render()
{
		CamMtx.t[2]=ZPos;	// Temp

		m_pauseMenu->render();
		CConversation::render();
		CThingManager::renderAllThings();
		Level.render();
}

/*****************************************************************************/
void	CGameScene::think(int _frames)
{
//#ifdef __USER_paul__		
//	if(!CConversation::isActive()&&PadGetDown(0)&PAD_START)
//	{
//		CConversation::trigger(SCRIPTS_SPEECHTEST_DAT);
//	}
//#endif

	CFileIO::LoadASyncFiles();

	if(s_readyToExit)
	{
		// Temporarily.. exiting game scene always goes back to the front end (pkg)
		GameState::setNextScene(&FrontEndScene);
	}
	else if(s_levelFinished)
	{
		s_globalLevelSelectThing=Level.GetNextLevel(s_globalLevelSelectThing);
		shutdownLevel(s_globalLevelSelectThing%12==0);
		initLevel();
		s_levelFinished=false;
	}

	
	if(!m_pauseMenu->isActive()&&PadGetDown(0)&PAD_START&&canPause())
	{
		m_pauseMenu->select();
	}

	CConversation::think(_frames);
	m_pauseMenu->think(_frames);
	if(!CConversation::isActive()&&
	   !m_pauseMenu->isActive())
	{
		DVECTOR	camPos;
		CThingManager::thinkAllThings(_frames);
		camPos=m_player->getCameraPos();
//PKG
//		if(camPos.vx<0){camPos.vx=0;PAUL_DBGMSG("cx<0");}
//		if(camPos.vy<0){camPos.vy=0;PAUL_DBGMSG("cy<0");}
//PKG		
		CBubicleFactory::setMapOffset(&camPos);
		Level.setCameraCentre(camPos);
		Level.think(_frames);

		if(PadGetDown(0)&PAD_R2)
		{
			levelFinished();
		}
	}
}

/*****************************************************************************/
int		CGameScene::readyToShutdown()
{
	return s_readyToExit;
}

/*****************************************************************************/
CPlayer	* CGameScene::getPlayer()
{
	return( m_player );
}

/*****************************************************************************/
void CGameScene::sendEvent( GAME_EVENT evt, CThing *sourceThing )
{
	CThingManager::processEventAllThings(evt, sourceThing);
}



/*****************************************************************************/
void	CGameScene::initLevel()
{
	CThingManager::init();

	CConversation::init();
	CConversation::registerConversationScript(SCRIPTS_SPEECHTEST_DAT);	// Register one script for testing..

#ifdef __USER_paul__
	DVECTOR pos={16*10,16*10};
				createPickup(PICKUP__BIG_HEALTH,&pos);
	pos.vx+=64;	createPickup(PICKUP__MEDIUM_HEALTH,&pos);
	pos.vx+=64;	createPickup(PICKUP__SMALL_HEALTH,&pos);
	pos.vx+=64;	createPickup(PICKUP__LIFE,&pos);
	pos.vx+=64;	createPickup(PICKUP__SPATULA,&pos);
	pos.vx+=64;	createPickup(PICKUP__JELLY_LAUNCHER_AMMO,&pos);
	pos.vx+=64;	createPickup(PICKUP__BUBBLE_MIXTURE,&pos);		
	pos.vx+=64;	createPickup(PICKUP__BUBBLE_WAND,&pos);		
	pos.vx+=64;	createPickup(PICKUP__NET,&pos);		
	pos.vx+=64;	createPickup(PICKUP__GLASSES,&pos);		
	pos.vx+=64;	createPickup(PICKUP__SQUEAKY_SHOES,&pos);		
	pos.vx+=64;	createPickup(PICKUP__BALLOON,&pos);		
	pos.vx+=64;	createPickup(PICKUP__HELMET,&pos);		
	pos.vx+=64;	createPickup(PICKUP__CORAL_BLOWER,&pos);		
	pos.vx+=64;	createPickup(PICKUP__QUEST_ITEM__TEST,&pos);		

//	CNpcPlatform	*platform;
//	platform=new ("test platform") CNpcPlatform;
//	platform->init();
//	platform->setLayerCollision( Level.getCollisionLayer() );
#endif

	s_levelFinished=false;
	Level.init();

	createPlayer();
	m_player->setRespawnPos(Level.getPlayerSpawnPos());
	m_player->init();
	m_player->setLayerCollision(Level.getCollisionLayer());
	m_player->setMapSize(Level.getMapSize());

// Init actors (needs moving and tidying
	int pointNum;
	int actorNum;
	int platformNum;

	sThingActor **actorList = Level.getActorList();
	if (actorList)
	{
		for ( actorNum = 0 ; actorNum < Level.getActorCount() ; actorNum++ )
		{
			CActorPool::ACTOR_TYPE actorType = CActorPool::getActorType( actorList[actorNum]->Type );

			switch ( actorType )
			{
				case CActorPool::ACTOR_ENEMY_NPC:
				{
					CNpcEnemy *enemy;
					enemy = new ("npc enemy") CNpcEnemy;
					ASSERT(enemy);
					enemy->setTypeFromMapEdit( actorList[actorNum]->Type );
					enemy->init();
					enemy->setLayerCollision( Level.getCollisionLayer() );

					u16	*PntList=(u16*)MakePtr(actorList[actorNum],sizeof(sThingActor));

					u16 newXPos, newYPos;

					newXPos = (u16) *PntList;
					PntList++;
					newYPos = (u16) *PntList;
					PntList++;

					enemy->setStartPos( newXPos, newYPos );
					enemy->addWaypoint( newXPos, newYPos );

					if ( actorList[actorNum]->PointCount > 1 )
					{
						for ( pointNum = 1 ; pointNum < actorList[actorNum]->PointCount ; pointNum++ )
						{
							newXPos = (u16) *PntList;
							PntList++;
							newYPos = (u16) *PntList;
							PntList++;

							enemy->addWaypoint( newXPos, newYPos );
						}
					}

					enemy->postInit();

					break;
				}

				default:
					break;
			}
		}
	}

	sThingPlatform **platformList = Level.getPlatformList();
	if (platformList)
	{
		for ( platformNum = 0 ; platformNum < Level.getPlatformCount() ; platformNum++ )
		{
			CNpcPlatform *platform;
			platform = new ("platform") CNpcPlatform;
			ASSERT(platform);
			platform->setTypeFromMapEdit( platformList[platformNum]->Type );

			u16	*PntList=(u16*)MakePtr(platformList[platformNum],sizeof(sThingPlatform));

			u16 newXPos, newYPos;

			newXPos = (u16) *PntList;
			PntList++;
			newYPos = (u16) *PntList;
			PntList++;

			DVECTOR startPos;
			startPos.vx = newXPos << 4;
			startPos.vy = newYPos << 4;

			platform->init( startPos );
			platform->setLayerCollision( Level.getCollisionLayer() );
			platform->setTiltable( false );

			platform->addWaypoint( newXPos, newYPos );

			if ( platformList[platformNum]->PointCount > 1 )
			{
				for ( pointNum = 1 ; pointNum < platformList[platformNum]->PointCount ; pointNum++ )
				{
					newXPos = (u16) *PntList;
					PntList++;
					newYPos = (u16) *PntList;
					PntList++;

					platform->addWaypoint( newXPos, newYPos );
				}
			}
		}
	}

	// Song is loaded/dumped by the level, and played from here. This just gives some
	// better timing over when it starts (pkg)
	CSoundMediator::playSong();
}


/*****************************************************************************/
void	CGameScene::shutdownLevel(bool CleanUp)
{
	CConversation::shutdown();
	CThingManager::shutdown();
	Level.shutdown(CleanUp);
}

/*****************************************************************************/
