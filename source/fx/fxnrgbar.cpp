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

int		LegCount=3;
int		LegWInc=32/LegCount;
int		LegHInc=-4;
int		LegAngleInc=7;

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
		
			RenderPos.vx+=Ofs.vx;
			RenderPos.vy+=Ofs.vy;

			for (int i=0; i<LegCount; i++)
			{
				ThisAngle+=AngleInc;
				ThisAngle&=CIRCLE_TAB_MASK;
				H=LegHeight+(CircleTable[ThisAngle]>>5);

				POLY_FT4	*Ft4=SprBank->printFT4(FRM__LEG,RenderPos.vx,RenderPos.vy,XFlip,0,OtPos*0);

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
				RenderPos.vy+=H+LegHInc;
				WOfs+=LegWInc;
			}
}
