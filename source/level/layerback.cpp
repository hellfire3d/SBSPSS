/******************************/
/*** Solid Tile Layer Class ***/
/******************************/

#include 	"system\global.h"
#include	<DStructs.h>
#include 	"utils\utils.h"
#include 	"gfx\prim.h"


#include	"LayerTile.h"
#include	"LayerBack.h"


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
CLayerBack::CLayerBack(sLayerHdr *Hdr,sTile *TileList,sTri *TriList,sQuad *QuadList,sVtx *VtxList) : CLayerTile(Hdr,TileList,TriList,QuadList,VtxList)// : CLayerTile(Hdr)
{
//		RGB[0]=(u8*)MakePtr(Hdr,sizeof(sLayerHdr));
//		RGB[1]=(u8*)MakePtr(RGB[0],3);
//		RGB[2]=(u8*)MakePtr(RGB[1],3);
//		RGB[3]=(u8*)MakePtr(RGB[2],3);
		Data=(sLayerShadeHdr*)(Hdr,sizeof(sLayerHdr));

		for (int i=0; i<Data->Count; i++)
		{
			SetPolyG4(&Poly[i]);
			setRGB0(&Poly[i],Data->Data[i].RGB[0],Data->Data[i].RGB[1],Data->Data[i].RGB[2]);
			setRGB1(&Poly[i],Data->Data[i].RGB[0],Data->Data[i].RGB[1],Data->Data[i].RGB[2]);
//			setRGB2(G4,0,0,128-Col);
//			setRGB3(G4,0,0,128-Col);

		}

}

/*****************************************************************************/
CLayerBack::~CLayerBack()
{
}


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CLayerBack::init(DVECTOR &MapPos,int Shift,int Width,int Height)
{
}

/*****************************************************************************/
void	CLayerBack::shutdown()
{
}

/*****************************************************************************/
void	CLayerBack::think(DVECTOR &MapPos)
{ 
		YOfs=MapPos.vy>>2;
}

/*****************************************************************************/
int		BH=256;
void	CLayerBack::render()
{
sOT			*ThisOT=OtPtr+(MAX_OT-1);
//printf("%i %i\n",MapWidth,MapHeight);
			setXYWH(&Poly[0],0,-YOfs,512,BH);
			addPrim(ThisOT,&Poly[0]);

}



