/***************************/
/*** 3d Tile Layer Class ***/
/***************************/

#ifndef __LAYER_TILE_3D_H__
#define __LAYER_TILE_3D_H__


/*****************************************************************************/
class CLayerTile3d : public CLayerTile
{
public:
		CLayerTile3d(sLayerHdr *Hdr,sTile *TileBank);
		~CLayerTile3d();

		void			init(DVECTOR &MapPos,int Shift);
		void			shutdown();
		void			render();

protected:
		sLayer3d		*LayerInfo;
		sTri			*TriList;
		sQuad			*QuadList;
		sVtx			*VtxList;

};



/*****************************************************************************/

#endif