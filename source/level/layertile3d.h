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

		void			init(DVECTOR &MapPos,int Shift,int Width,int Height);
		void			shutdown();
		void			render();

protected:
		void			UpdateRow(int MapX,int MapY);
		void			UpdateColumn(int MapX,int MapY);

		sPrimGridElem3d	*GetGridPos3d(int X,int Y);
		sTileMapElem3d	*GetMapPos3d(int X,int Y);

		void	RenderBlock(sPrimGridElem3d *Elem);

};



/*****************************************************************************/

#endif