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
CLayerBack::CLayerBack(sLayerHdr *Hdr,sTile *TileList,sTri *TriList,sQuad *QuadList,sVtx *VtxList) : CLayerTile(Hdr,TileList,TriList,QuadList,VtxList)
{
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
int	YY;
void	CLayerBack::think(DVECTOR &MapPos)
{ // Overide default strip scroll update
int			XPos=MapPos.vx>>MapXYShift;
int			YPos=MapPos.vy>>MapXYShift;

			ShiftX=XPos&15;
			ShiftY=YPos&15;

			MapX=XPos>>4;
			MapY=YPos>>4;

			YY=MapPos.vy>>2;
}

/*****************************************************************************/
void	CLayerBack::render()
{
POLY_G4		*G4;
sOT			*ThisOT=OtPtr+LayerOT;
int			Col;

			Col=YY;
			if (Col>127) Col=127;

			G4=GetPrimG4();
			setXYWH(G4,0,0,512,256);
			setRGB0(G4,0,255-Col,255-Col);
			setRGB1(G4,0,255-Col,255-Col);
			setRGB2(G4,0,0,128-Col);
			setRGB3(G4,0,0,128-Col);
			addPrimNoCheck(ThisOT,G4);
}



