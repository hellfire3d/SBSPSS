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

		void			init(DVECTOR &MapPos,int Shift);
		void			shutdown();
		void			render();

		sTileMapElem3d	*GetMapPos3d()		{return(((sTileMapElem3d*)Map)+GetMapOfs());}

protected:
		void			CreateRenderFlagTable();
		u8				*RenderBlock(sTile *Tile,s16 RenderFlags,u8 *PrimPtr);

		s16				*RenderFlagTable;

};



/*****************************************************************************/

#endif