/******************/
/*** Energy Bar ***/
/******************/

#include 	"system\global.h"
#include 	"system\vid.h"
#include	<DStructs.h>
#include 	"utils\utils.h"
#include 	"gfx\prim.h"
#include	"gfx\sprbank.h"
#include	<sprites.h>
#include	"level\level.h"
#include	"game\game.h"

#include	"FX\FXNRGBar.h"
#include	"enemy\npc.h"

#include	"FX\FXBubble.h"

const int		NRGX=32;
const int		NRGY=188;
const int		NRGW=INGAME_SCREENW-64;

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CFXNRGBar::init(DVECTOR const &_Pos)
{
		CFX::init();
SpriteBank	*SprBank=CGameScene::getSpriteBank();

		GfxW=SprBank->getFrameWidth(FRM__BUBBLE_1);
		GfxHalfW=GfxW/2;
		CurrentHealth=0;
}

/*****************************************************************************/
void	CFXNRGBar::SetMax(int Max)
{
		MaxHealth=Max;
		Scale=((NRGW*4096)/32)/MaxHealth;
		XInc=NRGW/MaxHealth;
		CurrentHealth=Max;
}

/*****************************************************************************/
/*** Think *******************************************************************/
/*****************************************************************************/
//int		NRGMax=16;
//int		NRGH=NRGMax;
void	CFXNRGBar::think(int _frames)
{
CNpcEnemy	*P=(CNpcEnemy*)ParentThing;
int			Health=P->getHealth();

//		SetMax(NRGMax);
//		Health=NRGH;

		if (CurrentHealth<Health) CurrentHealth=Health;
		if (CurrentHealth>Health)
		{
			CurrentHealth--;
			for (int b=0; b<4; b++)
			{
				DVECTOR	Pos;
				Pos.vx=NRGX+(CurrentHealth*XInc);
				Pos.vy=NRGY;

				CFXBubble	*FX=(CFXBubble*)CFX::Create(CFX::FX_TYPE_BUBBLE_WATER,Pos);
				FX->Flags|=FX_FLAG_SCREEN_FX;
				FX->SetOtPos(0);
			}
		}

}

/*****************************************************************************/
/*** Render ******************************************************************/
/*****************************************************************************/
void	CFXNRGBar::render()
{
//		CFX::render();

int			x=NRGX;

SpriteBank	*SprBank=CGameScene::getSpriteBank();
POLY_FT4	*Ft4;
			for (int i=0; i<CurrentHealth; i++)
			{
				Ft4=SprBank->printRotatedScaledSprite(FRM__BUBBLE_1,x,NRGY,Scale,Scale,0,0);
				x+=XInc;
			}

}
