/******************/
/*** Energy Bar ***/
/******************/

#include 	"system\global.h"
#include	<DStructs.h>
#include 	"utils\utils.h"
#include 	"gfx\prim.h"
#include	"gfx\sprbank.h"
#include	<sprites.h>
#include	"level\level.h"
#include	"game\game.h"

#include	"FX\FXNRGBar.h"
#include	"enemy\npc.h"

int		NRGX=32;
int		NRGY=188;
int		NRGW=512-64;


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CFXNRGBar::init(DVECTOR const &_Pos)
{
		CFX::init();
SpriteBank	*SprBank=CGameScene::getSpriteBank();

		GfxW=SprBank->getFrameWidth(FRM__BUBBLE_1);
		GfxHalfW=GfxW/2;
		DrawW=NRGW;
}

/*****************************************************************************/
void	CFXNRGBar::shutdown()
{
		CFX::shutdown();
}

/*****************************************************************************/
/*** Think *******************************************************************/
/*****************************************************************************/

void	CFXNRGBar::think(int _frames)
{

CNpcEnemy	*P=(CNpcEnemy*)ParentThing;
int		Health=P->getHealth();
		CurrentW=((NRGW/MaxHealth)*Health);

int		Diff=DrawW-CurrentW;

		DrawW-=(Diff+1)>>1;
		if (DrawW<=0 && Health==0)
		{
			setToShutdown();
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
// Draw Start
			Ft4=SprBank->printFT4(FRM__BUBBLE_1,NRGX,NRGY,0,0,0);
			Ft4->x1-=GfxHalfW;
			Ft4->x3-=GfxHalfW;
			Ft4->u1-=GfxHalfW;
			Ft4->u3-=GfxHalfW;

// Draw Start
			Ft4=SprBank->printFT4(FRM__BUBBLE_1,NRGX+GfxHalfW,NRGY,0,0,0);
			Ft4->x1+=DrawW-GfxW;
			Ft4->x3+=DrawW-GfxW;
			Ft4->u0+=GfxHalfW-1;
			Ft4->u1-=GfxHalfW-1;
			Ft4->u2+=GfxHalfW-1;
			Ft4->u3-=GfxHalfW-1;

// Draw End
			Ft4=SprBank->printFT4(FRM__BUBBLE_1,NRGX+DrawW+GfxHalfW,NRGY,0,0,0);
			Ft4->x1-=GfxHalfW;
			Ft4->x3-=GfxHalfW;
			Ft4->u0+=GfxHalfW;
			Ft4->u2+=GfxHalfW;

}
