/************************/
/*** Tile Layer Class ***/
/************************/

#ifndef __LAYER_TILE_H__
#define __LAYER_TILE_H__


/*****************************************************************************/
struct	sTileTableElem
{
	TSPRT_16		Prim;
	sTile			*Tile;
	u32				TileFlags;
	sTileTableElem	*Right;
	sTileTableElem	*Down;
};

struct	sTileTable
{
	int				MapX,MapY;
	u32				ShiftXY;
	sTileTableElem	*Table;
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
		void			UpdateWholeMap(sTileTable &ThisMap);
		sTileTable		&GetTileTable();
		int				CalcTableOfs(int X,int Y);
		int				CalcMapOfs(int X,int Y);
		void			UpdateRow(int MapX,int MapY,sTileTable &ThisMap);
		void			UpdateColumn(int MapX,int MapY,sTileTable &ThisMap);
		
		void			renderSolid();
		void			render3d();
		void			RenderBlock(sTile *Tile,u32 Flags);

		sLayerHdr		*LayerHdr;
		sTile			*TileList;
		sTri			*TriList;
		sQuad			*QuadList;
		sVtx			*VtxList;
		sTileMapElem	*Map;

		int				MapWidth,MapHeight,MapXYShift;
		int				TileTableWidth,TileTableHeight;
		sTileTable		TileTable[2];			// Double Buffered
};



/*****************************************************************************/

#endif