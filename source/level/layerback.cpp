/*************************/
/*** Shade Layer Class ***/
/*************************/

#include 	"system\global.h"
#include	<DStructs.h>
#include 	"utils\utils.h"
#include 	"gfx\prim.h"


#include	"LayerTile.h"
#include	"LayerBack.h"


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
CLayerBack::CLayerBack(sLayerHdr *Hdr,sTile *TileBank) : CLayerTile(Hdr,TileBank)
{
		Data=(sLayerShadeHdr*)MakePtr(Hdr,sizeof(sLayerHdr));

		ASSERT(Data->Count<=LAYER_SHADE_RGB_MAX);
		BandCount=Data->Count-1;
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
			setRGB0(&Band[i],Data->Data[i+0].RGB[0],Data->Data[i+0].RGB[1],Data->Data[i+0].RGB[2]);
			setRGB1(&Band[i],Data->Data[i+0].RGB[0],Data->Data[i+0].RGB[1],Data->Data[i+0].RGB[2]);
			setRGB2(&Band[i],Data->Data[i+1].RGB[0],Data->Data[i+1].RGB[1],Data->Data[i+1].RGB[2]);
			setRGB3(&Band[i],Data->Data[i+1].RGB[0],Data->Data[i+1].RGB[1],Data->Data[i+1].RGB[2]);
		}
}

/*****************************************************************************/
void	CLayerBack::shutdown()
{
}

/*****************************************************************************/
extern	DVECTOR	TileMapOfs;
void	CLayerBack::think(DVECTOR &MapPos)
{ 
		YOfs=MapPos.vy>>MapXYShift;
		YOfs+=TileMapOfs.vy;
}

/*****************************************************************************/
void	CLayerBack::render()
{
sOT		*ThisOT=OtPtr+(MAX_OT-1);
int		ThisY=-YOfs;

		for (int i=0; i<BandCount; i++)
		{
			setXYWH(&Band[i],0,ThisY,512,BandHeight);
			addPrim(ThisOT,&Band[i]);
			ThisY+=BandHeight;
		}

}



