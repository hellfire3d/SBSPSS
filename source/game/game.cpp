/**********************/
/*** Main Game File ***/
/**********************/

#include 	"system\global.h"
#include 	"fileio\fileio.h"
#include 	"pad\pads.h"
#include 	"system\vid.h"
#include 	"gfx\prim.h"
#include	"utils\utils.h"
#include	"game\game.h"
#include	"system\gstate.h"
#include	"gfx\font.h"
#include	"gfx\fdata.h"


/*****************************************************************************/

FontBank			*CGameScene::s_genericFont;

/*****************************************************************************/

CGameScene	GameScene;

/*****************************************************************************/
// Note, do not load any data in this function
void 	CGameScene::Init()
{
	s_genericFont=new ("CGameScene::Init") FontBank();
	s_genericFont->initialise( &standardFont );
	s_genericFont->setColour( 128, 80, 100 );
}

/*****************************************************************************/

void CGameScene::Shutdown()
{
}

/*****************************************************************************/
void 	CGameScene::Render()
{
}

/*****************************************************************************/
bool 	CGameScene::Control()
{
	return 1;
}

/*****************************************************************************/
