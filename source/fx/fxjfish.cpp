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
void	CFXJellyFishLegs::Setup(int XOfs,int YOfs,bool _XFlip)
{
	Ofs.vx=XOfs; 
	Ofs.vy=YOfs;
	XFlip=_XFlip;
}

/*****************************************************************************/
/*** Think *******************************************************************/
/*****************************************************************************/

void	CFXJellyFishLegs::think(int _frames)
{
		CFX::think(_frames);
		Angle++; Angle&=CIRCLE_TAB_MASK;
		AngleInc=LegAngleInc;
}

/*****************************************************************************/
/*** Render ******************************************************************/
/*****************************************************************************/
const int	LegXInc=4;


void	CFXJellyFishLegs::render()
{
CThing	*Parent=getParent();

		CFX::render();
//		if (!canRender()) return;
		if (!Parent->canRender()) return;
			
SpriteBank	*SprBank=CGameScene::getSpriteBank();;
DVECTOR		RenderPos=getRenderPos();
int			WOfs=0;	
int			H;
int			ThisAngle=Angle;
int			LegHeight=SprBank->getFrameHeight(FRM__LEG)-4;
int			ScaleWInc=(Scale*LegWInc)>>12;
int			ScaleHInc=(Scale*LegHInc)>>12;
int			XInc=LegXInc;

			RenderPos.vx+=Ofs.vx;
			RenderPos.vy+=Ofs.vy;

			if (!XFlip) 
			{
				XInc=-XInc-1;	
			}
			for (int i=0; i<LegCount; i++)
			{
				ThisAngle+=AngleInc;
				ThisAngle&=CIRCLE_TAB_MASK;
				H=LegHeight+(CircleTable[ThisAngle]>>5);


				POLY_FT4	*Ft4;


				Ft4=SprBank->printFT4Scaled(FRM__LEG,RenderPos.vx+(i*XInc),RenderPos.vy,XFlip,0,OtPos,Scale>>4);

				if (!XFlip)
				{
					Ft4->x0+=WOfs/2;
					Ft4->x2+=WOfs/2;
					Ft4->x1-=WOfs/2;
					Ft4->x3-=WOfs/2;
				}
				else
				{
					Ft4->x0+=WOfs/2;
					Ft4->x2+=WOfs/2;
					Ft4->x1-=WOfs/2;
					Ft4->x3-=WOfs/2;
				}

		
				Ft4->y2=Ft4->y0+H;
				Ft4->y3=Ft4->y1+H;
				RenderPos.vy+=H+ScaleHInc;
				WOfs+=ScaleWInc;

			}
}


/*****************************************************************************/
/*** Render on map ***********************************************************/
/*****************************************************************************/
void	CFXJellyFishLegs::renderOnMapScreen( DVECTOR drawPos, int XOfs, int YOfs )
{
SpriteBank	*SprBank=CGameScene::getSpriteBank();;
DVECTOR		RenderPos=drawPos;
int			WOfs=0;	
int			H;
int			ThisAngle=0;
int			LegHeight=SprBank->getFrameHeight(FRM__LEG)-4;
int			ScaleWInc=(ONE*LegWInc)>>12;
int			ScaleHInc=(ONE*LegHInc)>>12;
int			XInc=LegXInc;

			RenderPos.vx+=XOfs;
			RenderPos.vy+=YOfs;

			if (!XFlip) XInc=-XInc;		
			for (int i=0; i<LegCount; i++)
			{
				ThisAngle+=AngleInc;
				ThisAngle&=CIRCLE_TAB_MASK;
				H=LegHeight+(CircleTable[ThisAngle]>>5);


				POLY_FT4	*Ft4;

				Ft4=SprBank->printFT4Scaled(FRM__LEG,RenderPos.vx+(i*XInc),RenderPos.vy,XFlip,0,OtPos,ONE>>4);

				if (!XFlip)
				{
					Ft4->x0+=WOfs/2;
					Ft4->x2+=WOfs/2;
					Ft4->x1-=WOfs/2;
					Ft4->x3-=WOfs/2;
				}
				else
				{
					Ft4->x0+=WOfs/2;
					Ft4->x2+=WOfs/2;
					Ft4->x1-=WOfs/2;
					Ft4->x3-=WOfs/2;
				}

		
				Ft4->y2=Ft4->y0+H;
				Ft4->y3=Ft4->y1+H;
				RenderPos.vy+=H+ScaleHInc;
				WOfs+=ScaleWInc;

			}
}
