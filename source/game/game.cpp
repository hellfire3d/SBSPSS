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

#ifndef __ENEMY_2DENEMY_H__
#include "enemy\2denemy.h"
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


int		GX=248;
int		GY=129;
int		GH=256;
int		ZPos=5100;

/*****************************************************************************/

FontBank	*CGameScene::s_genericFont;
MATRIX		CGameScene::CamMtx;

/*****************************************************************************/

int s_globalLevelSelectThing=0;
int CGameScene::s_readyToExit;
int exitToNextLevel;


/*****************************************************************************/

CGameScene	GameScene;

/*****************************************************************************/
void 	CGameScene::init()
{
		CThingManager::init();

		SetIdentNoTrans(&CamMtx);
		CamMtx.t[2]=ZPos;

		s_genericFont=new ("CGameScene::Init") FontBank();
		s_genericFont->initialise( &standardFont );
		s_genericFont->setColour( 255, 255 , 0 );
		VidSetClearScreen(1);

		CConversation::init();
		CConversation::registerConversationScript(SCRIPTS_SPEECHTEST_DAT);	// Register one script for testing..

		Level.init();

#ifdef __USER_charles__		
		CNpcEnemy	*enemy;
		enemy=new ("test enemy") CNpcEnemy;
		enemy->init();
		enemy->setLayerCollision( Level.getCollisionLayer() );
#endif

#ifdef __USER_paul__
		DVECTOR pos={16*10,16*10};
					createPickup(PICKUP__BIG_HEALTH,&pos);
		pos.vx+=32;	createPickup(PICKUP__MEDIUM_HEALTH,&pos);
		pos.vx+=32;	createPickup(PICKUP__SMALL_HEALTH,&pos);
		pos.vx+=32;	createPickup(PICKUP__LIFE,&pos);
		pos.vx+=32;	createPickup(PICKUP__SPATULA,&pos);
		pos.vx+=32;	createPickup(PICKUP__JELLY_LAUNCHER_AMMO,&pos);
		pos.vx+=32;	createPickup(PICKUP__BUBBLE_MIXTURE,&pos);		
		pos.vx+=32;	createPickup(PICKUP__GLASSES,&pos);		
		pos.vx+=32;	createPickup(PICKUP__SQUEAKY_SHOES,&pos);		
		pos.vx+=32;	createPickup(PICKUP__BALLOON,&pos);		
		pos.vx+=32;	createPickup(PICKUP__HELMET,&pos);		
		pos.vx+=32;	createPickup(PICKUP__QUEST_ITEM__TEST,&pos);		
#endif

		createPlayer();
		m_player->init();
		m_player->setLayerCollision(Level.getCollisionLayer());
		m_player->setMapSize(Level.getMapSize());

		m_pauseMenu=new ("Pause Menu") CPauseMenu();
		m_pauseMenu->init();

		CFader::setFadingIn();

		SetGeomOffset( GX, GY );
		SetGeomScreen(GH);

		s_readyToExit=false;
		exitToNextLevel=false;
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
		m_pauseMenu->shutdown();	delete m_pauseMenu;

		CThingManager::shutdown();

		Level.shutdown();
		CConversation::shutdown();
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
			exitToNextLevel=true;
		}
	}


	if(s_readyToExit)
	{
		// Temporarily.. exiting game scene always goes back to the front end (pkg)
		GameState::setNextScene(&FrontEndScene);
	}
	else if(exitToNextLevel)
	{
		s_globalLevelSelectThing++;
		GameState::setNextScene(&GameScene);
		s_readyToExit=true;
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
