/******************************/
/*** Solid Tile Layer Class ***/
/******************************/

#ifndef __LAYER_TILE_SOLID_H__
#define __LAYER_TILE_SOLID_H__


/*****************************************************************************/
class CLayerTileSolid : public CLayerTile
{
public:
		CLayerTileSolid(sLayerHdr *Hdr,sTile *TileList,sTri *TriList,sQuad *QuadList,sVtx *VtxList);
		~CLayerTileSolid();

		void	init(DVECTOR &MapPos,int Shift,int Width,int Height);
		void	shutdown();
		void	think(DVECTOR &MapPos);
		void	render();

protected:

};



/*****************************************************************************/

#endif