/***************************/
/*** 3d Tile Layer Class ***/
/***************************/

#ifndef __LAYER_TILE_3D_H__
#define __LAYER_TILE_3D_H__


/*****************************************************************************/
class CLayerTile3d : public CLayerTile
{
public:
		CLayerTile3d(sLayerHdr *Hdr,sTile *TileList,sTri *TriList,sQuad *QuadList,sVtx *VtxList);
		~CLayerTile3d();

		void	init(VECTOR &MapPos,int Shift,int Width,int Height);
		void	shutdown();
		void	render();

protected:

};



/*****************************************************************************/

#endif