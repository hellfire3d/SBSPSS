/************************/
/*** Tile Layer Class ***/
/************************/

#ifndef __LAYER_TILE_H__
#define __LAYER_TILE_H__


/*****************************************************************************/
struct	sPrimGridElem
{
	TSPRT_16		Prim;
	u16				Tile;
	u16				Flags;
	sPrimGridElem	*Right;
	sPrimGridElem	*Down;
};

/*****************************************************************************/
class CLayerTile
{
public:
		enum LAYER_TILE_TYPE
		{ // Stored reverse for rendering :o)
			LAYER_TILE_TYPE_FORE,
			LAYER_TILE_TYPE_ACTION,
			LAYER_TILE_TYPE_MID,
			LAYER_TILE_TYPE_BACK,
			LAYER_TILE_TYPE_MAX
		};

		enum
		{
			TILE_WIDTH=16,
			TILE_HEIGHT=16,
			SCREEN_TILE_WIDTH=32,		// fast bits ops :o)
			SCREEN_TILE_HEIGHT=16,		// fast bits ops :o)
			SCREEN_TILE_SIZE=SCREEN_TILE_WIDTH*SCREEN_TILE_HEIGHT
		};


		CLayerTile(sLayerHdr *Hdr,sTile *TileList,sTri *TriList,sQuad *QuadList,sVtx *VtxList);
virtual	~CLayerTile();

virtual	void			init(VECTOR &MapPos,int Shift,int Width=SCREEN_TILE_WIDTH,int Height=SCREEN_TILE_HEIGHT);
virtual	void			shutdown();
virtual	void			think(VECTOR &MapPos);
virtual	void			render();


protected:
		void			UpdateWholeMap();
		sPrimGridElem	*GetGridPos(int X,int Y);
		sTileMapElem	*GetMapPos(int X,int Y);
		void			UpdateRow(int MapX,int MapY);
		void			UpdateColumn(int MapX,int MapY);
		
		void			renderSolid();
		void			render3d();
		void			RenderBlock(sPrimGridElem *Elem);

		sLayerHdr		*LayerHdr;
		sTile			*TileList;
		sTri			*TriList;
		sQuad			*QuadList;
		sVtx			*VtxList;
		sTileMapElem	*Map;

		int				MapWidth,MapHeight,MapXYShift;
		int				PrimGridWidth,PrimGridHeight;

		int				MapX,MapY;
		u16				ShiftX,ShiftY;
		sPrimGridElem	*PrimGrid;

};



/*****************************************************************************/

#endif