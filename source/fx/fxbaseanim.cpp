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
#include	"FX\FXBaseAnim.h"

/*****************************************************************************/
void	CFXBaseAnim::init(DVECTOR const &_Pos)
{
		CFX::init(_Pos);
		CurrentFrame=0;
		CurrentScale=DataPtr->Scale;
		Life=((DataPtr->EndFrame-DataPtr->StartFrame)<<DataPtr->FrameShift)-1;
}

/*****************************************************************************/
void	CFXBaseAnim::shutdown()
{
		CFX::shutdown();
}

/*****************************************************************************/
/*** Think *******************************************************************/
/*****************************************************************************/
void	CFXBaseAnim::think(int _frames)
{
		CFX::think(_frames);
		CurrentFrame+=_frames;
		Pos.vy-=4;
}

/*****************************************************************************/
/*** Render ******************************************************************/
/*****************************************************************************/
void	CFXBaseAnim::render()
{
DVECTOR	RenderPos;

		getFXRenderPos(RenderPos);
		if (!canRender()) return;

SpriteBank	*SprBank=CGameScene::getSpriteBank();

int			ThisFrame=CurrentFrame>>DataPtr->FrameShift;

POLY_FT4	*Ft4=SprBank->printFT4Scaled(DataPtr->StartFrame+ThisFrame,RenderPos.vx,RenderPos.vy,0,0,OtPos,CurrentScale>>4);
			Ft4->u1--; Ft4->u3--;
			Ft4->v2--; Ft4->v3--;
			setShadeTex(Ft4,0);
			setRGB0(Ft4,DataPtr->R,DataPtr->G,DataPtr->B);
}
