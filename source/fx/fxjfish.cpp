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

#include	"FX\FXjfish.h"


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CFXJellyFishLegs::init()
{
		CFX::init();
		FXList=0;
}

/*****************************************************************************/
void	CFXJellyFishLegs::shutdown()
{
		for (int L=0; L<Width; L++)
		{
			MemFree(FXList[L]);
		}

		MemFree(FXList);
		MemFree(WidthTable);
		MemFree(HeightTable);
		CFX::shutdown();
}

/*****************************************************************************/
/*****************************************************************************/
void	CFXJellyFishLegs::SetUp(int _Width,int _Gap,int _Length,int _Count)
{

		XOfs=-(_Width/2);
		Width=_Width/_Gap;
		Gap=_Gap;
		Length=_Length;
		Count=_Count;

		FXList=(sList**)MemAlloc(Width*sizeof(sList*),"FishLegTable");
		for (int L=0; L<Width; L++)
		{
			sList	*ThisList=(sList*)MemAlloc(Count*sizeof(sList),"FishLeg");
			FXList[L]=ThisList;
		}
		ListIdx=0;

		WidthTable=(s16*)MemAlloc(Count*sizeof(s16),"FishLegWTable");
		for (int W=0; W<Count; W++)
		{
			int	dW=((8<<12)/Count)/2;
			WidthTable[W]=(dW*W)>>12;
		}

		HeightTable=(s16*)MemAlloc(Width*sizeof(s16),"FishLegHTable");
int		AInc=1024/Width;
		for (int H=0; H<Width; H++)
		{
			int	Ofs=abs(H-(Width/2));
			int	dH=8-Ofs;
			dH*=2;
			if (dH>8) dH=8;
			HeightTable[H]=(s16)dH;
//			printf("%i %i =%i\n",H,Ofs,HeightTable[H]);
		}

}

/*****************************************************************************/
/*** Think *******************************************************************/
/*****************************************************************************/
//int	XT[]={-3,-3,-3,-2,-2,-2,-1,-1,	+1,+1,+2,+2,+2,+3,+3,+3};
int	XT[]={-3,-2,-2,-2,-2,-1,-1,-1,	+1,+1,+1,+2,+2,+2,+2,+3};
void	CFXJellyFishLegs::think(int _frames)
{
		ASSERT(FXList);
		CFX::think(_frames);

		ListIdx--;
		if (ListIdx<0) ListIdx+=Count;
		for (int i=0; i<Width; i++)
		{
			sList	*ThisList=FXList[i];
			ThisList[ListIdx].Ofs=Parent->getPos();
//			ThisList[ListIdx].Ofs.vx/=16;
//			ThisList[ListIdx].Ofs.vy/=16;
			int	XO=getRnd();
			XO&=15;

			ThisList[ListIdx].Ofs.vx+=XT[XO];
		}
}

/*****************************************************************************/
/*** Render ******************************************************************/
/*****************************************************************************/
const int	Trans=3;
int	LX=8;
int	LY=32;

void	CFXJellyFishLegs::render()
{
DVECTOR	_MapOfs=CLevel::getCameraPos();
DVECTOR	MapOfs;
int		dRGB=256/Count;
s16		*dH=HeightTable;		

		_MapOfs.vx+=LX;
		_MapOfs.vy+=LY;

		if (!Parent->getOnScreenFlag()) return;		
		MapOfs.vx=_MapOfs.vx+XOfs;

		for (int L=0; L<Width; L++)
		{
			u8		C=255;
			s16		*dW=WidthTable;
			int		Idx=ListIdx;
			sList	*List=FXList[L];
			DVECTOR	LastPos;
			int		LastdW;
			int		Height=*dH++;

			MapOfs.vy=_MapOfs.vy;
			LastPos.vx=List[Idx].Ofs.vx-MapOfs.vx;
			LastPos.vy=List[Idx].Ofs.vy-MapOfs.vy;
			LastdW=*dW++;
			MapOfs.vy-=Length;
			Idx++;

			for (int i=0; i<Count-1; i++)
			{
				if (Idx>=Count) Idx=0;
				POLY_FT4	*Ft4=m_spriteBank->printFT4(FRM__TENTACLE,LastPos.vx,LastPos.vy,0,0,6);

				Ft4->x0=LastPos.vx+0+LastdW; Ft4->y0=LastPos.vy;
				Ft4->x1=LastPos.vx+8-LastdW; Ft4->y1=LastPos.vy;
				setSemiTrans(Ft4,1);
				Ft4->tpage|=Trans<<5;
				LastPos.vx=List[Idx].Ofs.vx-MapOfs.vx;
				LastPos.vy=List[Idx].Ofs.vy-MapOfs.vy;
				LastdW=*dW++;
				Ft4->x2=LastPos.vx+0+LastdW; Ft4->y2=LastPos.vy;
				Ft4->x3=LastPos.vx+8-LastdW; Ft4->y3=LastPos.vy;
				setRGB0(Ft4,C,C,C);
				C-=dRGB;
				MapOfs.vy-=Height;
				Idx++;
			}
			MapOfs.vx+=Gap;
		}
}
