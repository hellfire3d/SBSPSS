/**********************/
/*** JellyFish Legs ***/
/**********************/

#include 	"system\global.h"
#include	<DStructs.h>
#include 	"utils\utils.h"
#include 	"gfx\prim.h"
#include	"gfx\sprbank.h"
#include	<sprites.h>
#include	"level\level.h"
#include	"game\game.h"

#include	"FX\FXjfish.h"

static const int		LegCount=3;
static const int		LegWInc=32/LegCount;
static const int		LegHInc=-4;
static const int		LegAngleInc=7;

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CFXJellyFishLegs::init(DVECTOR const &_Pos)
{
		CFX::init();
		Pos=_Pos;
	
		Ofs.vx=0; Ofs.vy=0;
		Angle=getRnd();
		AngleInc=LegAngleInc+getRndRange(3);
		Scale = ONE;
}

/*****************************************************************************/
void	CFXJellyFishLegs::shutdown()
{
		CFX::shutdown();
}

/*****************************************************************************/
void	CFXJellyFishLegs::Setup(int XOfs,int YOfs,bool XFlip)
{
	Ofs.vx=XOfs; 
	Ofs.vy=YOfs;
	this->XFlip=XFlip;
}

/*****************************************************************************/
/*** Think *******************************************************************/
/*****************************************************************************/

void	CFXJellyFishLegs::think(int _frames)
{
		Pos=getParent()->getPos();

		CFX::think(_frames);
		Angle++; Angle&=CIRCLE_TAB_MASK;
		AngleInc=LegAngleInc;
}

/*****************************************************************************/
/*** Render ******************************************************************/
/*****************************************************************************/
void	CFXJellyFishLegs::render()
{
		CFX::render();
		if (!canRender()) return;

SpriteBank	*SprBank=CGameScene::getSpriteBank();;
DVECTOR		RenderPos=getRenderPos();
int			WOfs=0;	
int			H;
int			ThisAngle=Angle;
int			LegHeight=SprBank->getFrameHeight(FRM__LEG)-4;
int			ScaleWInc=(Scale*LegWInc)>>12;
int			ScaleHInc=(Scale*LegHInc)>>12;
		
			RenderPos.vx+=Ofs.vx;
			RenderPos.vy+=Ofs.vy;

			for (int i=0; i<LegCount; i++)
			{
				ThisAngle+=AngleInc;
				ThisAngle&=CIRCLE_TAB_MASK;
				H=LegHeight+(CircleTable[ThisAngle]>>5);

				int		spriteWidth = ( Scale * CGameScene::getSpriteBank()->getFrameWidth(FRM__LEG) ) >> 12;

				POLY_FT4	*Ft4;

				if ( XFlip )
				{
					Ft4=SprBank->printFT4Scaled(FRM__LEG,RenderPos.vx + 6 + ( spriteWidth >> 1 ),RenderPos.vy,XFlip,0,OtPos,Scale>>4);
				}
				else
				{
					Ft4=SprBank->printFT4Scaled(FRM__LEG,RenderPos.vx - 6 - ( spriteWidth >> 1 ),RenderPos.vy,XFlip,0,OtPos,Scale>>4);
				}

				if (!XFlip)
				{
					Ft4->x1-=WOfs;
					Ft4->x3-=WOfs;
				}
				else
				{
					Ft4->x0+=WOfs;
					Ft4->x2+=WOfs;
				}

		
				Ft4->y2=Ft4->y0+H;
				Ft4->y3=Ft4->y1+H;
//				RenderPos.vy+=H+LegHInc;
//				WOfs+=LegWInc;
				RenderPos.vy+=H+ScaleHInc;
				WOfs+=ScaleWInc;

			}
}
