/************************/
/*** Tile Layer Class ***/
/************************/

#ifndef __LAYER_TILE_H__
#define __LAYER_TILE_H__


/*****************************************************************************/
struct	sPrimGridElem
{
	TSPRT_16		Prim;
	sPrimGridElem	*Right;
	sPrimGridElem	*Down;
};

struct	sPrimGridElem3d : public sPrimGridElem
{
	u16				Tile;
	u16				Flags;
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
			SCREEN_TILE_WIDTH=32,
			SCREEN_TILE_HEIGHT=16,
			SCREEN_TILE_SIZE=SCREEN_TILE_WIDTH*SCREEN_TILE_HEIGHT
		};


		CLayerTile(sLayerHdr *Hdr,sTile *TileList,sTri *TriList,sQuad *QuadList,sVtx *VtxList);
virtual	~CLayerTile();

virtual	void			init(DVECTOR &MapPos,int Shift,int Width=SCREEN_TILE_WIDTH,int Height=SCREEN_TILE_HEIGHT);
virtual	void			shutdown();
virtual	void			think(DVECTOR &MapPos);
virtual	void			render();

protected:
virtual	void			UpdateWholeMap();
virtual	void			UpdateRow(int MapX,int MapY);
virtual	void			UpdateColumn(int MapX,int MapY);

		sPrimGridElem	*GetGridPos(int X,int Y);
		sTileMapElem	*GetMapPos(int X,int Y);

		sLayerHdr		*LayerHdr;
		sTile			*TileList;
		sTri			*TriList;
		sQuad			*QuadList;
		sVtx			*VtxList;

		int				MapWidth,MapHeight,MapXYShift;
		int				PrimGridWidth,PrimGridHeight;

		int				MapX,MapY;
		u16				ShiftX,ShiftY;
		
		sTileMapElem	*Map;
		sPrimGridElem	*PrimGrid;

};



/*****************************************************************************/

#endif