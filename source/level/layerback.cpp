/*************************/
/*** Shade Layer Class ***/
/*************************/

#include 	"system\global.h"
#include	<DStructs.h>
#include 	"utils\utils.h"
#include 	"gfx\prim.h"

#include	"gfx\sprbank.h"

#include	"LayerTile.h"
#include	"LayerBack.h"

struct sBackRGBTable
{
		u8	R,G,B,P;
};

sBackRGBTable	BackRGBTable[]=
{
	{255,  0,  0,0},
	{255,255,  0,0},
	{  0,255,  0,0},
	{  0,255,255,0},
	{  0,  0,255,0},
	{255,  0,255,0},
	{255,255,255,0},
	{128,  0,  0,0},
	{128,128,  0,0},
	{  0,128,  0,0},
	{  0,128,128,0},
	{  0,  0,128,0},
	{128,  0,128,0},
	{128,128,128,0},
};

#define	BackRGBTableSize	sizeof(BackRGBTable)/sizeof(sBackRGBTable)

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
CLayerBack::CLayerBack(sLayerHdr *Hdr,sTile *TileBank) : CLayerTile(Hdr,TileBank)
{
		Data=(sLayerShadeHdr*)MakePtr(Hdr,sizeof(sLayerHdr));

		ASSERT(Data->Count<=LAYER_SHADE_RGB_MAX);
		BandCount=Data->Count-1;

		Sprites=new ("BackGfx Sprites") SpriteBank();
		Sprites->load(LEVELS_LEVELBACKGFX_SPR);
}

/*****************************************************************************/
CLayerBack::~CLayerBack()
{
		Sprites->dump();	delete Sprites;
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CLayerBack::init(DVECTOR &MapPos,int Shift)
{
		MapXYShift=Shift;

		BandHeight=(MapHeight*16)/(BandCount);
		if (BandHeight>=512) BandHeight=511;

		for (int i=0; i<BandCount; i++)
		{
			SetPolyG4(&Band[i]);
			setRGB0(&Band[i],Data->Data[i+0].RGB[0],Data->Data[i+0].RGB[1],Data->Data[i+0].RGB[2]);
			setRGB1(&Band[i],Data->Data[i+0].RGB[0],Data->Data[i+0].RGB[1],Data->Data[i+0].RGB[2]);
			setRGB2(&Band[i],Data->Data[i+1].RGB[0],Data->Data[i+1].RGB[1],Data->Data[i+1].RGB[2]);
			setRGB3(&Band[i],Data->Data[i+1].RGB[0],Data->Data[i+1].RGB[1],Data->Data[i+1].RGB[2]);
		}

		for (int i=0; i<SPRITE_MAX; i++)
		{
			POLY_GT4	*Gt4=&SpriteList[i].Poly;
			sFrameHdr	*Frm=Sprites->getFrameHeader(SprFrame[i&1]);
			Sprites->prepareGT4(Gt4,Frm,0,0,0,0);
			SpriteList[i].W=Frm->W;
			SpriteList[i].H=Frm->W;
			setSemiTrans(Gt4,1);
//			Gt4->tpage|=Tran<<5;
			if (Frm->Rotated)
			{
				Gt4->u2--; 	Gt4->u3--;
				Gt4->v0--; 	Gt4->v2--;
			}
			else
			{
				Gt4->u1--; 	Gt4->u3--;
				Gt4->v2--; 	Gt4->v3--;
			}
			InitSprite(&SpriteList[i]);
			SpriteList[i].Pos.vx=getRndRange(512<<MOVE_SHIFT);
			SpriteList[i].Pos.vy=getRndRange(256<<MOVE_SHIFT);
		}
		PosDx=0;
		PosDy=0;
		XOfs=MapPos.vy;
		YOfs=MapPos.vy;
}

/*****************************************************************************/
void	CLayerBack::SetFrames(int Frm0,int Frm1)
{
		SprFrame[0]=Frm0;
		SprFrame[1]=Frm1;

}

/*****************************************************************************/
void	CLayerBack::shutdown()
{
}

/*****************************************************************************/
void	CLayerBack::InitSprite(sBackSprite *SpritePtr)
{
int		StartPos=getRnd();
int		Pos=getRnd();

int		XInc=(getRndRange((1<<((MOVE_SHIFT*2)/3))-1)+1)<<MOVE_SHIFT;
int		YInc=(getRndRange((1<<((MOVE_SHIFT*2)/3))-1)+1)<<MOVE_SHIFT;

		switch(StartPos&3)
		{
			case 0:	// Left
				SpritePtr->Pos.vx=-63;
				SpritePtr->Pos.vy=(Pos%(256+128))-63;
				SpritePtr->PosInc.vx=XInc;
				if (XInc&1)
					SpritePtr->PosInc.vy=+YInc;
				else
					SpritePtr->PosInc.vy=-YInc;
				break;

			case 1:	// Right
				SpritePtr->Pos.vx=512+63;
				SpritePtr->Pos.vy=(Pos%(256+128))-63;
				SpritePtr->PosInc.vx=-XInc;
				if (XInc&1)
					SpritePtr->PosInc.vy=+YInc;
				else
					SpritePtr->PosInc.vy=-YInc;
				break;
			case 2:	// Top
				SpritePtr->Pos.vx=(Pos%(512+128))-63;
				SpritePtr->Pos.vy=-63;
				if (YInc&1)
					SpritePtr->PosInc.vx=+XInc;
				else
					SpritePtr->PosInc.vx=-XInc;
				SpritePtr->PosInc.vy=YInc;
				break;
			case 3:	// Bottom
				SpritePtr->Pos.vx=(Pos%(512+128))-63;
				SpritePtr->Pos.vy=256+63;
				if (YInc&1)
					SpritePtr->PosInc.vx=+XInc;
				else
					SpritePtr->PosInc.vx=-XInc;
				SpritePtr->PosInc.vy=-YInc;
				break;
		}
		SpritePtr->Pos.vx<<=MOVE_SHIFT;
		SpritePtr->Pos.vy<<=MOVE_SHIFT;
		
		SpritePtr->Scale.vx=getRndRange(4095);
		SpritePtr->Scale.vy=getRndRange(4095);
		SpritePtr->ScaleInc.vx=getRndRange(31)+31;
		if (SpritePtr->ScaleInc.vx&1) SpritePtr->ScaleInc.vx=-SpritePtr->ScaleInc.vx;
		SpritePtr->ScaleInc.vy=getRndRange(31)+31;
		if (SpritePtr->ScaleInc.vy&1) SpritePtr->ScaleInc.vy=-SpritePtr->ScaleInc.vy;
		SpritePtr->AngleInc=getRndRange(31)+31;
		if (SpritePtr->AngleInc&1) SpritePtr->AngleInc=-SpritePtr->AngleInc;

int	i;
		i=getRndRange(BackRGBTableSize-1); SpritePtr->Poly.r0=BackRGBTable[i].R; SpritePtr->Poly.g0=BackRGBTable[i].G; SpritePtr->Poly.b0=BackRGBTable[i].B;
		i=getRndRange(BackRGBTableSize-1); SpritePtr->Poly.r1=BackRGBTable[i].R; SpritePtr->Poly.g1=BackRGBTable[i].G; SpritePtr->Poly.b1=BackRGBTable[i].B;
		i=getRndRange(BackRGBTableSize-1); SpritePtr->Poly.r2=BackRGBTable[i].R; SpritePtr->Poly.g2=BackRGBTable[i].G; SpritePtr->Poly.b2=BackRGBTable[i].B;
		i=getRndRange(BackRGBTableSize-1); SpritePtr->Poly.r3=BackRGBTable[i].R; SpritePtr->Poly.g3=BackRGBTable[i].G; SpritePtr->Poly.b3=BackRGBTable[i].B;


}

/*****************************************************************************/
extern	DVECTOR	TileMapOfs;
void	CLayerBack::think(DVECTOR &MapPos)
{ 
int		_XOfs=MapPos.vx>>MapXYShift;
int		_YOfs=MapPos.vy>>MapXYShift;
		_YOfs+=TileMapOfs.vy;

		PosDx=(_XOfs-XOfs)<<MOVE_SHIFT;
		PosDy=(_YOfs-YOfs)<<MOVE_SHIFT;

		XOfs=_XOfs;
		YOfs=_YOfs;
}

/*****************************************************************************/
int	Tran=0;

void	CLayerBack::render()
{
const	int	OTPos=(MAX_OT-1);
sOT		*ThisOT=OtPtr+OTPos;
int		i,ThisY=-YOfs;

// Render Back Sprites
sBackSprite		*SpritePtr=SpriteList;
		for (i=0; i<SPRITE_MAX; i++)
		{
			SpritePtr->Pos.vx+=SpritePtr->PosInc.vx;
			SpritePtr->Pos.vy+=SpritePtr->PosInc.vy;
			SpritePtr->Pos.vx-=PosDx;
			SpritePtr->Pos.vy-=PosDy;

			SpritePtr->Angle+=SpritePtr->AngleInc;
			SpritePtr->Angle&=4095;
			SpritePtr->Scale.vx+=SpritePtr->ScaleInc.vx;
			SpritePtr->Scale.vx&=4095;
			SpritePtr->Scale.vy+=SpritePtr->ScaleInc.vy;
			SpritePtr->Scale.vy&=4095;

			int	X=(SpritePtr->Pos.vx>>MOVE_SHIFT);
			int	Y=(SpritePtr->Pos.vy>>MOVE_SHIFT);

			
			if (X<-64 || X>512+64 || Y<-64 || Y>256+64)
			{
				InitSprite(SpritePtr);
			}


POLY_GT4	*Gt4=&SpriteList[i].Poly;
int			ScaleX=msin(SpritePtr->Scale.vx);
int			ScaleY=msin(SpritePtr->Scale.vy);
			Gt4->tpage|=Tran<<5;

sBox		Box;
			Sprites->RotateBox(&Box,SpritePtr->W,SpritePtr->W,ScaleX,ScaleY,SpritePtr->Angle);

			Gt4->x0=X+Box.x0; Gt4->y0=Y+Box.y0;
			Gt4->x1=X+Box.x1; Gt4->y1=Y+Box.y1;
			Gt4->x2=X+Box.x2; Gt4->y2=Y+Box.y2;
			Gt4->x3=X+Box.x3; Gt4->y3=Y+Box.y3;
			AddPrim(ThisOT,Gt4);
			SpritePtr++;
		}
// Render Back Shade
		for (i=0; i<BandCount; i++)
		{
			setXYWH(&Band[i],0,ThisY,512,BandHeight);
			addPrim(ThisOT,&Band[i]);
			ThisY+=BandHeight;
		}
}
