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


int		GX=248;
int		GY=129;
int		GH=256;
int		ZPos=5100;

/*****************************************************************************/

FontBank	*CGameScene::s_genericFont;
MATRIX		CGameScene::CamMtx;

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
		VidSetClearScreen(1);

		CConversation::init();
		CConversation::registerConversationScript(SCRIPTS_SPEECHTEST_DAT);	// Register one script for testing..

		Level.init();

#ifdef __USER_charles__		
		C2dEnemy	*enemy;
		enemy=new ("test enemy") C2dEnemy;
		enemy->init();
#endif

		createPlayer();
		m_player->init();
		m_player->setLayerCollision(Level.getCollisionLayer());

		CFader::setFadingIn();

		SetGeomOffset( GX, GY );
		SetGeomScreen(GH);
}

/*****************************************************************************/
// This is a seperate funtion ( and virtual ) so that we can overload it for
// the demo mode (pkg)
// NB: This function should *only* include the new function - Don't add anything else here!
void	CGameScene::createPlayer()
{
		m_player=new ("player") CPlayer();
}


/*****************************************************************************/

void	CGameScene::shutdown()
{
		m_player->shutdown();		delete m_player;
		CThing::shutdownAndDeleteAllThings();

		Level.shutdown();
		CConversation::shutdown();
		s_genericFont->dump();		delete s_genericFont;
}

/*****************************************************************************/
void 	CGameScene::render()
{
		CamMtx.t[2]=ZPos;	// Temp

		CConversation::render();
		CThing::renderAllThings();
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

	CConversation::think(_frames);
	if(!CConversation::isActive())
	{
		DVECTOR	camPos;
		CThing::thinkAllThings(_frames);
		camPos=m_player->getCameraPos();
//PKG
if(camPos.vx<0)camPos.vx=0;
if(camPos.vy<0)camPos.vy=0;
//PKG		
		CBubicleFactory::setMapOffset(&camPos);
		Level.setCameraCentre(camPos);
		Level.think(_frames);
	}
}

/*****************************************************************************/
int		CGameScene::readyToShutdown()
{
	return false;
}

/*****************************************************************************/
CPlayer	* CGameScene::getPlayer()
{
	return( m_player );
}

/*****************************************************************************/
void CGameScene::sendEvent( GAME_EVENT evt, CThing *sourceThing )
{
	CThing::processEventAllThings(evt, sourceThing);
}

/*****************************************************************************/
