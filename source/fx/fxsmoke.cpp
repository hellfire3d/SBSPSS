/******************/
/*** Smoke Puff ***/
/******************/

#include 	"system\global.h"
#include	<DStructs.h>
#include 	"utils\utils.h"
#include 	"gfx\prim.h"
#include	"gfx\sprbank.h"
#include	<sprites.h>
#include	"level\level.h"
#include	"game\game.h"

#include	"FX\FXSmokePuff.h"

int		SmokePuffStartScale=ONE*2;
int		SmokePuffScaleInc=128;
int		SmokePuffAngleInc=128;

/*****************************************************************************/
void	CFXSmokePuff::init(DVECTOR const &_Pos)
{
		CFX::init(_Pos);
		Velocity.vy=-1;

		CurrentAngle=0;
		CurrentScale=SmokePuffStartScale;
		RGBDec=255/(CurrentScale/SmokePuffScaleInc);
		RGB.R=RGB.G=RGB.B=255;
}

/*****************************************************************************/
/*** Think *******************************************************************/
/*****************************************************************************/
void	CFXSmokePuff::think(int _frames)
{
		CFX::think(_frames);

		CurrentScale-=SmokePuffScaleInc;
		RGB.R-=RGBDec;
		RGB.G-=RGBDec;
		RGB.B-=RGBDec;

		CurrentAngle+=SmokePuffAngleInc;
		CurrentAngle&=4095;
		if (CurrentScale<0)
		{
			killFX();
		}
}

/*****************************************************************************/
/*****************************************************************************/
/*** Render ******************************************************************/
/*****************************************************************************/
void	CFXSmokePuff::render()
{
DVECTOR	RenderPos;

		getFXRenderPos(RenderPos);
		if (!canRender()) return;

SpriteBank	*SprBank=CGameScene::getSpriteBank();
POLY_FT4	*Ft4=SprBank->printRotatedScaledSprite(FRM__SMOKE,RenderPos.vx,RenderPos.vy,CurrentScale,CurrentScale,CurrentAngle,OtPos);
			setShadeTex(Ft4,0);
			setRGB0(Ft4,RGB.R,RGB.G,RGB.B);
			setSemiTrans(Ft4,1);
			Ft4->tpage|=3<<5;
			Frame=Ft4;
}


