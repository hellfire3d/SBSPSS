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



/*****************************************************************************/

FontBank			*CGameScene::s_genericFont;

BubicleEmitterData bubData=
{
	206,200,55,45,		// m_x,m_y,m_w,m_h
	10,1,				// m_birthRate,m_birthAmount
	-1,					// m_life
	{					// m_bubicleBase
		100,				// m_life
		0,0,0,				// m_vx,m_vdx,m_vxmax
		-40,-15,-100,		// m_vy,m_vdy,m_vymax
		13,10,				// m_w,m_h
		10,					// m_dvSizeChange
		0,100,				// m_theta,m_vtheta
		100,2,0,			// m_wobbleWidth,m_vwobbleWidth,m_vdwobbleWidth
		40,					// m_ot
		{ 128,128,128,	}	// m_colour
	},
	{					// m_bubicleRange
		100,				// m_life
		0,0,0,				// m_vx,m_vdx,m_vxmax
		20,10,0,			// m_vy,m_vdy,m_vymax
		5,5,				// m_w,m_h
		10,					// m_dvSizeChange
		4095,50,			// m_theta,m_vtheta
		100,5,0,			// m_wobbleWidth,m_vwobbleWidth,m_vdwobbleWidth
		0,					// m_ot
		{   0, 64,127,	}	// m_colour
	}
};
CBubicleEmitter	*testBub;

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

	CBubicleFactory::init();
	testBub=CBubicleFactory::spawnEmitter(&bubData);
}

/*****************************************************************************/

void CGameScene::shutdown()
{
	CBubicleFactory::shutdown();
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

	CBubicleFactory::render();
}

/*****************************************************************************/
void	CGameScene::think()
{
	for(int i=0;i<GameState::getFramesSinceLast();i++)
	{
		X+=Dx; Y+=Dy;
		if (X<0+64)		{X=0+64;	Dx=-Dx;}
		if (X>512-64)	{X=512-64;	Dx=-Dx;}
		if (Y<0+64)		{Y=0+64;	Dy=-Dy;}
		if (Y>256-64)	{Y=256-64;	Dy=-Dy;}
	}

	testBub->setPos(X,Y);

	CBubicleFactory::think();
}

/*****************************************************************************/
