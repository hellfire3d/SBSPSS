/*******************************/
/*** Action Tile Layer Class ***/
/*******************************/

#include 	"system\global.h"
#include	<DStructs.h>
#include 	"utils\utils.h"
#include 	"gfx\prim.h"


#include	"LayerTile.h"
#include	"LayerAction.h"


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
CLayerAction::CLayerAction(sLayerHdr *Hdr,sTile *TileList,sTri *TriList,sQuad *QuadList,sVtx *VtxList) : CLayerTile(Hdr,TileList,TriList,QuadList,VtxList)
{
}

/*****************************************************************************/
CLayerAction::~CLayerAction()
{
}


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CLayerAction::init(VECTOR &MapPos,int Shift,int Width,int Height)
{
		CLayerTile::init(MapPos,Shift);
}

/*****************************************************************************/
void	CLayerAction::shutdown()
{
}

/*****************************************************************************/
void	CLayerAction::render()
{
//		CLayerTile::render();
		CLayerTile::render3d();
}
