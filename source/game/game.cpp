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
	s_genericFont->setColour( 255, 255 , 0 );
	VidSetClearScreen(1);
}

/*****************************************************************************/

void CGameScene::Shutdown()
{
}

/*****************************************************************************/
int		X=512/2;
int		Y=256/2;
int		Dx=+7;
int		Dy=-3;

void 	CGameScene::Render()
{
char	*Str="Sponge\nBob\nSquare\nPants";

	X+=Dx; Y+=Dy;
	if (X<0+64)		{X=0+64;	Dx=-Dx;}
	if (X>512-64)	{X=512-64;	Dx=-Dx;}
	if (Y<0+64)		{Y=0+64;	Dy=-Dy;}
	if (Y>256-64)	{Y=256-64;	Dy=-Dy;}


	s_genericFont->print(X,Y,(u8*)Str);
}

/*****************************************************************************/
bool 	CGameScene::Control()
{
	return 1;
}

/*****************************************************************************/
