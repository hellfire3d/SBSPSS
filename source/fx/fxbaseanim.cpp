/***********************/
/*** Anim Base Class ***/
/***********************/

#include 	"system\global.h"
#include	<DStructs.h>
#include 	"utils\utils.h"
#include 	"gfx\prim.h"
#include	"gfx\sprbank.h"
#include	<sprites.h>
#include	"level\level.h"
#include	"game\game.h"
#include	"level\layercollision.h"
#include	"FX\FXBaseAnim.h"


/*****************************************************************************/
void	CFXBaseAnim::init(DVECTOR const &_Pos)
{
		CFX::init(_Pos);
		CurrentFrame=0;
		CurrentScaleX=CurrentScaleY=ONE;
		CurrentHeading = 0;
		HasInit=false;

}

/*****************************************************************************/
void	CFXBaseAnim::setBaseData(void *Data)
{
		CFX::setBaseData(Data);
		BaseData=(sFXBaseData*)Data;
}

/*****************************************************************************/
/*** Think *******************************************************************/
/*****************************************************************************/
void	CFXBaseAnim::think(int _frames)
{
		if (!HasInit)
		{
			MaxFrame=((BaseData->EndFrame-BaseData->StartFrame)<<BaseData->FrameShift)-1;
			Flags|=BaseData->Flags;
			renderFrame=BaseData->StartFrame;

			if (Flags & FX_FLAG_LOOP)
			{
				Life=-1;
			}
			else
			{
				Life=MaxFrame;
			}
			HasInit=true;
		}

		CFX::think(_frames);

		if (BaseData->StartFrame!=BaseData->EndFrame)
		{
			CurrentFrame++;

			if (CurrentFrame>MaxFrame)
			{
				CurrentFrame=0;
			}
int		ThisFrame=CurrentFrame>>BaseData->FrameShift;
		renderFrame=BaseData->StartFrame+ThisFrame;
		}

}

/*****************************************************************************/
/*****************************************************************************/
/*** Render ******************************************************************/
/*****************************************************************************/
void	CFXBaseAnim::render()
{
DVECTOR	RenderPos;

		getFXRenderPos(RenderPos);
		if (!canRender() || Flags & FX_FLAG_HIDDEN) return;

SpriteBank	*SprBank=CGameScene::getSpriteBank();
POLY_FT4	*Ft4=SprBank->printRotatedScaledSprite(renderFrame,RenderPos.vx,RenderPos.vy,CurrentScaleX,CurrentScaleY,CurrentHeading,OtPos);
			setShadeTex(Ft4,0);
			setRGB0(Ft4,RGB.R,RGB.G,RGB.B);
			setSemiTrans(Ft4,Flags & FX_FLAG_TRANS);
			Frame=Ft4;
}


