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

const int		NRGX=96;
const int		NRGY=188;
const int		NRGXInc=32;
const int		NRGMaxHealth=10;
const int		NRGW=INGAME_SCREENW-(NRGMaxHealth*NRGXInc);
const int		NRGBaseRGB=240;
const int		NRGRGBInc=NRGBaseRGB/NRGMaxHealth;

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CFXNRGBar::init(DVECTOR const &_Pos)
{
		CFX::init();
SpriteBank	*SprBank=CGameScene::getSpriteBank();

		CurrentHealth=0;
}

/*****************************************************************************/
void	CFXNRGBar::SetMax(int Max)
{
		MaxHealth=Max;
		CurrentHealth=Max;
}

/*****************************************************************************/
/*** Think *******************************************************************/
/*****************************************************************************/
void	CFXNRGBar::think(int _frames)
{
CNpcEnemy	*P=(CNpcEnemy*)ParentThing;
int			Health=P->getHealth();

		if (CurrentHealth<Health) CurrentHealth=Health;
		if (CurrentHealth>Health)
		{
			CurrentHealth--;
			for (int b=0; b<4; b++)
			{
				DVECTOR	Pos;
				Pos.vx=NRGX+(CurrentHealth*NRGXInc);
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

SpriteBank	*SprBank=CGameScene::getSpriteBank();
POLY_FT4	*Ft4;
int			RGB=NRGBaseRGB;
int			x=NRGX;

			for (int i=0; i<CurrentHealth; i++)
			{
				Ft4=SprBank->printFT4(FRM__HEALTHBUBBLE,x,NRGY,0,0,0);
				setRGB0(Ft4,RGB,NRGBaseRGB-RGB,0);
				setSemiTrans(Ft4,1);
				RGB-=NRGRGBInc;
				x+=NRGXInc;
			}

}
