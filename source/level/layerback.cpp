/*************************/
/*** Shade Layer Class ***/
/*************************/

#include 	"system\vid.h"
#include 	"system\global.h"
#include	<DStructs.h>
#include 	"utils\utils.h"
#include 	"gfx\prim.h"

#include	"gfx\sprbank.h"

#include	"LayerTile.h"
#include	"LayerBack.h"

#include	"Game\game.h"

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
CLayerBack::CLayerBack(sLevelHdr *LevelHdr,sLayerHdr *Hdr) : CLayerTile(LevelHdr,Hdr)
{
		Data=(sLayerShadeHdr*)MakePtr(Hdr,sizeof(sLayerHdr));

		Data->GfxList=(sLayerShadeBackGfx*)MakePtr(Hdr,(int)Data->GfxList);
		Data->TypeList=(sLayerShadeBackGfxType*)MakePtr(Hdr,(int)Data->TypeList);
		ASSERT(Data->BandCount<=LAYER_SHADE_RGB_MAX);
		BandCount=Data->BandCount-1;
}

/*****************************************************************************/
CLayerBack::~CLayerBack()
{
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
			setRGB0(&Band[i],Data->RGB[i+0][0],Data->RGB[i+0][1],Data->RGB[i+0][2]);
			setRGB1(&Band[i],Data->RGB[i+0][0],Data->RGB[i+0][1],Data->RGB[i+0][2]);
			setRGB2(&Band[i],Data->RGB[i+1][0],Data->RGB[i+1][1],Data->RGB[i+1][2]);
			setRGB3(&Band[i],Data->RGB[i+1][0],Data->RGB[i+1][1],Data->RGB[i+1][2]);
		}

	
		XOfs=MapPos.vy;
		YOfs=MapPos.vy;
}

/*****************************************************************************/
void	CLayerBack::shutdown()
{
}

/*****************************************************************************/
void	CLayerBack::think(DVECTOR &MapPos)
{ 
		XOfs=MapPos.vx>>MapXYShift;
		YOfs=MapPos.vy>>MapXYShift;

		if (YOfs<1) YOfs=1;
}

/*****************************************************************************/
const int	BM=16;
const int	BB=16;

void	CLayerBack::render()
{
sOT			*ThisOT=OtPtr+(MAX_OT-1);
int			i,ThisY=-YOfs;
// Render Back Sprites

sLayerShadeBackGfx		*GfxList=Data->GfxList;

			for (i=0; i<Data->GfxCount; i++)
			{
				int			PosX=(GfxList->PosX*BB)-XOfs;
				int			PosY=(GfxList->PosY*BB)-YOfs;
				if (PosX>0-64 && PosX<INGAME_SCREENW+64)
				if (PosY>0-64 && PosY<INGAME_SCREENH+64)
				{
					POLY_GT4	*sGt4,*Gt4=GetPrimGT4();
					sLayerShadeBackGfxType	&ThisType=Data->TypeList[GfxList->Type];

					Gt4->x0=PosX+(GfxList->Ofs[0][0])*BM; Gt4->y0=PosY+(GfxList->Ofs[0][1])*BM;
					Gt4->x1=PosX+(GfxList->Ofs[1][0])*BM; Gt4->y1=PosY+(GfxList->Ofs[1][1])*BM;
					Gt4->x2=PosX+(GfxList->Ofs[2][0])*BM; Gt4->y2=PosY+(GfxList->Ofs[2][1])*BM;
					Gt4->x3=PosX+(GfxList->Ofs[3][0])*BM; Gt4->y3=PosY+(GfxList->Ofs[3][1])*BM;

					setRGB0(Gt4,GfxList->RGB[0][0]>>1,GfxList->RGB[0][1]>>1,GfxList->RGB[0][2]>>1);
					setRGB1(Gt4,GfxList->RGB[1][0]>>1,GfxList->RGB[1][1]>>1,GfxList->RGB[1][2]>>1);
					setRGB2(Gt4,GfxList->RGB[2][0]>>1,GfxList->RGB[2][1]>>1,GfxList->RGB[2][2]>>1);
					setRGB3(Gt4,GfxList->RGB[3][0]>>1,GfxList->RGB[3][1]>>1,GfxList->RGB[3][2]>>1);

					setUVWH(Gt4,ThisType.U,ThisType.V,ThisType.W,ThisType.H);
					Gt4->u1--; Gt4->u3--;
					Gt4->v2--; Gt4->v3--;
					Gt4->clut=ThisType.Clut;
					setSemiTrans(Gt4,1);
					Gt4->tpage=ThisType.TPage;// | GfxList->Trans<<5;
					AddPrim(ThisOT,Gt4);
				}
/*
// Bodge it cos artist made the fuckers too big
				sGt4=GetPrimGT4(); 	*sGt4=*Gt4; 
				Gt4->x0--; Gt4->y0--; Gt4->x1++; Gt4->y1--; Gt4->x2--; Gt4->y2++; Gt4->x3++; Gt4->y3++;
				AddPrim(ThisOT,sGt4);
*/

				GfxList++;
			}

// Render Back Shade


			if (BandHeight>256) BandHeight=256;
//			if (ThisY<0) BandHeight=256;
		for (i=0; i<BandCount; i++)
		{
			setXYWH(&Band[i],0,ThisY,INGAME_SCREENW,BandHeight);
			addPrim(ThisOT,&Band[i]);
			ThisY+=BandHeight;
		}
		if (Band[BandCount-1].y1<0)
		{
			Band[BandCount-1].y0=0;
			Band[BandCount-1].y1=0;
			Band[BandCount-1].y2=256;
			Band[BandCount-1].y3=256;
		}

}
