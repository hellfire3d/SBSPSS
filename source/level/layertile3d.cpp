/***************************/
/*** 3d Tile Layer Class ***/
/***************************/

#include 	"system\global.h"
#include	<DStructs.h>
#include 	"utils\utils.h"
#include 	"gfx\prim.h"


#include	"LayerTile.h"
#include	"LayerTile3d.h"


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
CLayerTile3d::CLayerTile3d(sLayerHdr *Hdr,sTile *TileList,sTri *TriList,sQuad *QuadList,sVtx *VtxList) : CLayerTile(Hdr,TileList,TriList,QuadList,VtxList)
{
}

/*****************************************************************************/
CLayerTile3d::~CLayerTile3d()
{
}


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CLayerTile3d::init(VECTOR &MapPos,int Shift,int Width,int Height)
{
		CLayerTile::init(MapPos,Shift);
}

/*****************************************************************************/
void	CLayerTile3d::shutdown()
{
}

/*****************************************************************************/
void	CLayerTile3d::render()
{
//		CLayerTile::render();
		CLayerTile::render3d();
}
