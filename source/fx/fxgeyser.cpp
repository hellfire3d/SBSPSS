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

#include	"FX\FXGeyser.h"

int		GeyserSpeed=4;

/*****************************************************************************/
void	CFXGeyser::init(DVECTOR const &_Pos)
{
		CFX::init(_Pos);
		Height=TargetHeight=8;
		currentFrame=FRM__GUSH000;
}

/*****************************************************************************/
void	CFXGeyser::think(int Frames)
{
		CFX::think(Frames);

		currentFrame++;
		if (currentFrame>FRM__GUSH003)
		{
			currentFrame=FRM__GUSH000;
		}
//		TargetHeight=MaxHeight;
}

/*****************************************************************************/
/*** Render ******************************************************************/
/*****************************************************************************/
void	CFXGeyser::render()
{
DVECTOR		RenderPos;
POLY_FT4	*Ft4;
SpriteBank	*SprBank=CGameScene::getSpriteBank();
CThing		*Parent=getParent();

		getFXRenderPos(RenderPos);
		if (!canRender() || Flags & FX_FLAG_HIDDEN) return;

// is it attached to a platform?
		if (Parent) 
		{ // yes, so get pos, and return
			Height=RenderPos.vy-Parent->getRenderPos().vy;
		}
		else
		{
			int		HDiff=TargetHeight-Height;
			Height+=(HDiff+(GeyserSpeed-1))/GeyserSpeed;
		}

// top
		Ft4=SprBank->printFT4(currentFrame,RenderPos.vx,RenderPos.vy-Height,0,0,OtPos);
		setShadeTex(Ft4,0);
		setRGB0(Ft4,RGB.R,RGB.G,RGB.B);
		setSemiTrans(Ft4,Flags & FX_FLAG_TRANS);

int		FrameW=Ft4->x1-Ft4->x0;
		setCollisionCentreOffset(FrameW>>1,-Height/2);
		setCollisionCentreOffset(0,-Height/2);
		setCollisionSize(FrameW,Height);

// Base
		Ft4=SprBank->printFT4(FRM__GUSHBASE,RenderPos.vx,RenderPos.vy,0,0,OtPos);
		setShadeTex(Ft4,0);
		setRGB0(Ft4,RGB.R,RGB.G,RGB.B);
		setSemiTrans(Ft4,Flags & FX_FLAG_TRANS);
		Ft4->y0=Ft4->y2-Height;
		Ft4->y1=Ft4->y3-Height;
		Ft4->v2--; Ft4->v3--;

}

