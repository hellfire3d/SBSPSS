/**************/
/*** Smoke  ***/
/**************/

#include 	"system\global.h"
#include	<DStructs.h>
#include 	"utils\utils.h"
#include 	"gfx\prim.h"
#include	"gfx\sprbank.h"
#include	<sprites.h>
#include	"level\level.h"
#include	"game\game.h"

#include	"FX\FXSmoke.h"

static const int		SmokeStartRate=32;
static const int		SmokePuffScaleInc=64;
static const int		SmokePuffAngleInc=64;
static const int		SmokePuffRGBDec=4;

/*****************************************************************************/
void	CFXSmoke::init(DVECTOR const &_Pos)
{
		CFX::init(_Pos);
		Rate=SmokeStartRate;
		CurrentRate=Rate;

}

/*****************************************************************************/
/*** Think *******************************************************************/
/*****************************************************************************/
void	CFXSmoke::think(int _frames)
{
		CFX::think(_frames);
		Rate=SmokeStartRate;

		CurrentRate+=_frames;
		if (CurrentRate>=Rate)
		{
			CurrentRate=0;
			CFXSmokePuff *FX=(CFXSmokePuff*)CFX::Create(CFX::FX_TYPE_SMOKE_PUFF,Pos);
			FX->setRGB(255,255,255);
		}
}


/*****************************************************************************/
/*****************************************************************************/
/*** Smoke Puff **************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CFXSmokePuff::init(DVECTOR const &_Pos)
{
		CFX::init(_Pos);
		Velocity.vy=-1;

		CurrentAngle=getRndRange(4095);
		AngleInc=SmokePuffAngleInc+getRndRange(SmokePuffAngleInc);
		CurrentScale=1024;
		ScaleInc=SmokePuffScaleInc+getRndRange(SmokePuffScaleInc);
		setRGB(255,255,255);
}

/*****************************************************************************/
/*** Think *******************************************************************/
/*****************************************************************************/
void	CFXSmokePuff::think(int _frames)
{
		CFX::think(_frames);

		CurrentScale+=ScaleInc;
		CurrentAngle+=AngleInc;
		CurrentAngle&=4095;

int		R=RGB.R-SmokePuffRGBDec;
int		G=RGB.G-SmokePuffRGBDec;
int		B=RGB.B-SmokePuffRGBDec;

		if (R<0) R=0;
		if (G<0) G=0;
		if (B<0) B=0;

		RGB.R=R;
		RGB.G=G;
		RGB.B=B;

		if (RGB.R+RGB.G+RGB.B==0)
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


