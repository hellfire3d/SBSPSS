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
// Note, do not load any data in this function
void 	CGameScene::init()
{
	s_genericFont=new ("CGameScene::Init") FontBank();
	s_genericFont->initialise( &standardFont );
	s_genericFont->setColour( 255, 255 , 0 );
	VidSetClearScreen(1);

}

/*****************************************************************************/

void CGameScene::shutdown()
{
	s_genericFont->dump();		delete s_genericFont;
}

/*****************************************************************************/
int		X=512/2;
int		Y=256/2;
int		Dx=+3;
int		Dy=-2;

void 	CGameScene::render()
{
char	*Str="Sponge\nBob\nSquare\nPants";

	s_genericFont->print(X,Y,Str);
}

/*****************************************************************************/
void	CGameScene::think(int _frames)
{
	for(int i=0;i<_frames;i++)
	{
		X+=Dx; Y+=Dy;
		if (X<0+4)		{X=0+4;		Dx=getRndRange(5)+1;}
		if (X>512-64)	{X=512-64;	Dx=-(getRndRange(5)+1);}
		if (Y<+4)		{Y=0+4;		Dy=getRndRange(5)+1;}
		if (Y>256-64)	{Y=256-64;	Dy=-(getRndRange(5)+1);}
	}
}

/*****************************************************************************/
int		CGameScene::readyToShutdown()
{
	return false;
}

/*****************************************************************************/
