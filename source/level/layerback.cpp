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

		PosDx=0;
		PosDy=0;
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
int		_XOfs=MapPos.vx>>MapXYShift;
int		_YOfs=MapPos.vy>>MapXYShift;

		PosDx=(_XOfs-XOfs)<<MOVE_SHIFT;
		PosDy=(_YOfs-YOfs)<<MOVE_SHIFT;

		XOfs=_XOfs;
		YOfs=_YOfs;
		if (YOfs<1) YOfs=1;
}

/*****************************************************************************/
int	GfxW=64/16;
int	GfxH=64/16;

void	CLayerBack::render()
{
sOT			*ThisOT=OtPtr+(MAX_OT-1);
int			i,ThisY=-YOfs;
// Render Back Sprites

sLayerShadeBackGfx		*GfxList=Data->GfxList;

			for (i=0; i<Data->GfxCount; i++)
			{
				POLY_GT4	*Gt4=GetPrimGT4();
				int			PosX=(GfxList->PosX<<MapXYShift)-XOfs;
				int			PosY=(GfxList->PosY<<MapXYShift)-YOfs;
				sLayerShadeBackGfxType	&ThisType=Data->TypeList[GfxList->Type];

				Gt4->x0=PosX+(GfxList->Ofs[0][0]-(GfxW/2)*16); Gt4->y0=PosY+(GfxList->Ofs[0][1]-(GfxH/2)*16);
				Gt4->x1=PosX+(GfxList->Ofs[1][0]+(GfxW/2)*16); Gt4->y1=PosY+(GfxList->Ofs[1][1]-(GfxH/2)*16);
				Gt4->x2=PosX+(GfxList->Ofs[2][0]-(GfxW/2)*16); Gt4->y2=PosY+(GfxList->Ofs[2][1]+(GfxH/2)*16);
				Gt4->x3=PosX+(GfxList->Ofs[3][0]+(GfxW/2)*16); Gt4->y3=PosY+(GfxList->Ofs[3][1]+(GfxH/2)*16);

				setRGB0(Gt4,GfxList->RGB[0][0],GfxList->RGB[0][1],GfxList->RGB[0][2]);
				setRGB1(Gt4,GfxList->RGB[1][0],GfxList->RGB[1][1],GfxList->RGB[1][2]);
				setRGB2(Gt4,GfxList->RGB[2][0],GfxList->RGB[2][1],GfxList->RGB[2][2]);
				setRGB3(Gt4,GfxList->RGB[3][0],GfxList->RGB[3][1],GfxList->RGB[3][2]);

				setUVWH(Gt4,ThisType.U,ThisType.V,ThisType.W,ThisType.H);
				Gt4->u1--; Gt4->u3--;
				Gt4->v2--; Gt4->v3--;
				Gt4->clut=ThisType.Clut;
				Gt4->tpage=ThisType.TPage;
				AddPrim(ThisOT,Gt4);
				GfxList++;
			}

// Render Back Shade


		for (i=0; i<BandCount; i++)
		{
			setXYWH(&Band[i],0,ThisY,512,BandHeight*2);
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
