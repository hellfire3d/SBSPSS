/*****************/
/*** Thwack!!! ***/
/*****************/

#include 	"system\global.h"
#include	<DStructs.h>
#include 	"utils\utils.h"
#include 	"gfx\prim.h"
#include	"gfx\sprbank.h"
#include	<sprites.h>
#include	"level\level.h"
#include	"game\game.h"
#include	"gfx\otpos.h"

#include	"FX\FXThwack.h"

const int		ThwackLife=12;

/*****************************************************************************/
void	CFXThwack::init(DVECTOR const &_Pos)
{
		CFX::init(_Pos);
		Life=ThwackLife;
		OtPos=OTPOS__ACTOR_POS-1;
		Angle=getRnd()&4095;
		Scale=2048+1024+(getRnd()&2047);
		Flags |=FX_FLAG_HAS_LIFE;
//		CSoundMediator::playSfx(CSoundMediator::SFX_KAPOW_V1,false);
}

/*****************************************************************************/
/*** Render ******************************************************************/
/*****************************************************************************/

void	CFXThwack::render()
{
DVECTOR	RenderPos;

		getFXRenderPos(RenderPos);
		if (!canRender() || Flags & FX_FLAG_HIDDEN) return;

SpriteBank	*SprBank=CGameScene::getSpriteBank();
POLY_FT4	*Ft4=SprBank->printRotatedScaledSprite(FRM__THWACK,RenderPos.vx,RenderPos.vy,Scale,Scale,Angle,OtPos);
			setSemiTrans(Ft4,1);
			Ft4->tpage|=1<<5;
int			Col=(256/ThwackLife)*Life;
			setRGB0(Ft4,Col,Col,Col);

}

