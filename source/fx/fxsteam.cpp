/************************/
/*** Cloud Base Class ***/
/************************/

#include 	"system\global.h"
#include	<DStructs.h>
#include 	"utils\utils.h"
#include 	"gfx\prim.h"
#include	"gfx\sprbank.h"
#include	<sprites.h>
#include	"level\level.h"

#include	"FX\FXcloud.h"


DVECTOR		SmokeVel;
u16			AngleInc=16;

u8			StartR=255;
u8			StartG=255;
u8			StartB=255;
s8			RInc=-8;
s8			BInc=-8;
s8			GInc=-8;
s16			StartScaleX=256;
s16			ScaleXInc=256;
s16			StartScaleY=256;
s16			ScaleYInc=256;

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CFXCloud::init(DVECTOR const &_Pos)
{
		CFX::init();
		Pos=_Pos;
		HeadIdx=0;
		ListCount=1;
		Angle=0;
}

/*****************************************************************************/
void	CFXCloud::shutdown()
{
		CFX::shutdown();
}

/*****************************************************************************/
/*** Think *******************************************************************/
/*****************************************************************************/
int	SS=4;
void	CFXCloud::think(int _frames)
{
int		ThisIdx=HeadIdx;
DVECTOR	Vel=SmokeVel;
		if (Parent)
		{
			this->setPos(Parent->getPos());
		}

		CFX::think(_frames);

		HeadIdx--;
		if (HeadIdx<0) HeadIdx+=ListCount;

		if (Parent)
		{
			setPos(Parent->getPos());
		}
		List[HeadIdx].Ofs=getPos();


		for (int i=0; i<ListCount-1; i++)
		{
			sList	&ThisElem=List[ThisIdx++];
			ThisIdx&=MAX_TRAIL-1;
			ThisElem.Ofs.vx+=Vel.vx>>SS;
			ThisElem.Ofs.vy+=Vel.vy>>SS;
			Vel.vx+=SmokeVel.vx;
			Vel.vy+=SmokeVel.vy;
		}
		ListCount++;
		if (ListCount>MAX_TRAIL)
		{
			ListCount=MAX_TRAIL;
		}

}

/*****************************************************************************/
/*** Render ******************************************************************/
/*****************************************************************************/
int	ST=3;

void	CFXCloud::render()
{
		CFX::render();

		if (canRender())
		{
			DVECTOR	const &MapOfs=CLevel::getCameraPos();
			DVECTOR	RenderPos;
			int		ThisIdx=HeadIdx;

			int		ThisAngle=Angle;
			int		ThisScaleX=StartScaleX;
			int		ThisScaleY=StartScaleY;
			u8		ThisR=StartR;
			u8		ThisG=StartG;
			u8		ThisB=StartB;
			
			for (int i=0; i<ListCount; i++)
			{
				sList		&ThisOfs=List[ThisIdx];
				POLY_FT4	*Ft4;

				RenderPos.vx=ThisOfs.Ofs.vx-MapOfs.vx;
				RenderPos.vy=ThisOfs.Ofs.vy-MapOfs.vy;

				Ft4=m_spriteBank->printRotatedScaledSprite(FRM__SMOKE,RenderPos.vx,RenderPos.vy,ThisScaleX,ThisScaleY,ThisAngle,OtPos*0);
				setShadeTex(Ft4,0);
				setSemiTrans(Ft4,1);
				setRGB0(Ft4,ThisR,ThisB,ThisB);
				Ft4->tpage|=ST<<5;

				ThisR+=RInc;
				ThisG+=GInc;
				ThisB+=BInc;

				ThisAngle+=AngleInc;
				ThisAngle&=4095;
				ThisScaleX+=ScaleXInc;
				ThisScaleY+=ScaleYInc;

				ThisIdx++;
				ThisIdx&=MAX_TRAIL-1;
			}
			Angle+=AngleInc/2;
			Angle&=4095;

		}
}
