/***************/
/*** Fire FX ***/
/***************/

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
#include	"FX\FXFire.h"

/*****************************************************************************/
void	CFXFire::init(DVECTOR const &_Pos)
{
		CFXBaseAnim::init(_Pos);
		OtPos++;
}

/*****************************************************************************/
/*** Render ******************************************************************/
/*****************************************************************************/
static const int	FT=3;
static const int	FC=128;
static const int	FO=2;

void	CFXFire::render()
{
		if (!HasInit) return;


DVECTOR	RenderPos;
		getFXRenderPos(RenderPos);
		if (!canRender() || Flags & FX_FLAG_HIDDEN) return;

POLY_FT4	*Ft4;
SpriteBank	*SprBank=CGameScene::getSpriteBank();
sFrameHdr	*Hdr=SprBank->getFrameHeader(renderFrame);
int			SW=Hdr->W;
int			SH=Hdr->H;

int			W=SprW;
int			HW=SprW/2;
int			H=SprH;
int			HH=SprH/2;
int			FXO=0;
int			FYO=0;
			Frame=SprBank->printFT4(renderFrame,RenderPos.vx,RenderPos.vy,0,0,OtPos);
			setShadeTex(Frame,0);
			setRGB0(Frame,FC,FC,FC);
			setSemiTrans(Frame,1);
			Frame->tpage|=FT<<5;
			Frame->u1--; Frame->u3--;
			Frame->v2--; Frame->v3--;

			if (CurrentHeading<1024*1)
			{ // up
				RenderPos.vy+=HH;
				Frame->x0=RenderPos.vx-HW;	Frame->y0=RenderPos.vy-H;
				Frame->x1=RenderPos.vx+HW;	Frame->y1=RenderPos.vy-H;
				Frame->x2=RenderPos.vx-HW;	Frame->y2=RenderPos.vy;
				Frame->x3=RenderPos.vx+HW;	Frame->y3=RenderPos.vy;
				FYO=-FO;
			}
			else
			if (CurrentHeading<1024*2)
			{ // Right
				RenderPos.vx-=HH;
				Frame->x0=RenderPos.vx-W;	Frame->y0=RenderPos.vy+HW;
				Frame->x1=RenderPos.vx-W;	Frame->y1=RenderPos.vy-HW;
				Frame->x2=RenderPos.vx;		Frame->y2=RenderPos.vy+HW;
				Frame->x3=RenderPos.vx;		Frame->y3=RenderPos.vy-HW;
				FXO=+FO;
			}
			else
			if (CurrentHeading<1024*3)
			{ // Down
				RenderPos.vy-=HH;
				Frame->x0=RenderPos.vx-HW;	Frame->y0=RenderPos.vy+H;
				Frame->x1=RenderPos.vx+HW;	Frame->y1=RenderPos.vy+H;
				Frame->x2=RenderPos.vx-HW;	Frame->y2=RenderPos.vy;
				Frame->x3=RenderPos.vx+HW;	Frame->y3=RenderPos.vy;
				FYO=+FO;
			}
			else
			if (CurrentHeading<1024*4)
			{ // Left
				RenderPos.vx+=HH;
				Frame->x0=RenderPos.vx-H;	Frame->y0=RenderPos.vy+HW;
				Frame->x1=RenderPos.vx-H;	Frame->y1=RenderPos.vy-HW;
				Frame->x2=RenderPos.vx;		Frame->y2=RenderPos.vy+HW;
				Frame->x3=RenderPos.vx;		Frame->y3=RenderPos.vy-HW;
				FXO=-FO;
			}
		Ft4=GetPrimFT4();
		*Ft4=*Frame;
		AddPrimToList(Ft4,OtPos);
		Ft4->x0+=FXO; Ft4->y0+=FYO; 
		Ft4->x1+=FXO; Ft4->y1+=FYO; 
		Ft4->x2+=FXO; Ft4->y2+=FYO; 
		Ft4->x3+=FXO; Ft4->y3+=FYO; 

}


