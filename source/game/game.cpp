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

#include	"level\level.h"
#include	"player\player.h"
#include	"gfx\anim.h"

#ifndef __GFX_BUBICLES__
#include "gfx\bubicles.h"
#endif

#ifndef __SPR_INGAMEFX_H__
#include <ingamefx.h>
#endif



/*****************************************************************************/

FontBank			*CGameScene::s_genericFont;

/*****************************************************************************/

CGameScene	GameScene;

/*****************************************************************************/
void 	CGameScene::init()
{
		s_genericFont=new ("CGameScene::Init") FontBank();
		s_genericFont->initialise( &standardFont );
		s_genericFont->setColour( 255, 255 , 0 );
		VidSetClearScreen(1);
		m_conversation.init();
		Level.init();
		Player.init();
		CAnimDB::LoadAnims();

}

/*****************************************************************************/

void	CGameScene::shutdown()
{
		Level.shutdown();
		m_conversation.shutdown();
		s_genericFont->dump();		delete s_genericFont;
}

/*****************************************************************************/
void 	CGameScene::render()
{
		m_conversation.render();
		Player.render();
		Level.render();
}

/*****************************************************************************/
void	CGameScene::think(int _frames)
{
		m_conversation.think(_frames);
		Level.think(_frames);
		Player.think();
		Player.Animate();

		if(!m_conversation.isActive()&&PadGetDown(0)&PAD_START)
		{
			m_conversation.trigger(SCRIPTS_SPEECHTEST_DAT);
		}

}

/*****************************************************************************/
int		CGameScene::readyToShutdown()
{
	return false;
}

/*****************************************************************************/
