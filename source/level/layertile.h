/************************/
/*** Tile Layer Class ***/
/************************/

#ifndef __LAYER_TILE_H__
#define __LAYER_TILE_H__


/*****************************************************************************/
struct	sTileMap2dElem
{
	TSPRT_16		Tile;
	sTileMap2dElem	*Right;
	sTileMap2dElem	*Down;
};

struct	sTileMap2d
{
	int				X,Y;
	sTileMap2dElem	*List;
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


		CLayerTile(sLayerHdr *Hdr,sTile *_TileList,sTri *_TriList,sQuad *_QuadList,sVtx *_VtxList);
virtual	~CLayerTile();

virtual	void			init()=0;
virtual	void			shutdown()=0;
virtual	void			render()=0;
virtual	void			think(int _frames)=0;

		void			InitTileMap2d(int X,int Y,int Width=SCREEN_TILE_WIDTH,int Height=SCREEN_TILE_HEIGHT);

		int				GetWidth()		{return(LayerHdr->Width);}
		int				GetHeight()		{return(LayerHdr->Height);}


protected:
		void			RenderTileMap2d(int X,int Y);

		sLayerHdr		*LayerHdr;
		sTile			*TileList;
		sTri			*TriList;
		sQuad			*QuadList;
		sVtx			*VtxList;
		sTileMapElem	*Map;

		int				TileMapWidth,TileMapHeight;
		sTileMap2d		TileMap2d[2];			// Double Buffered
};



/*****************************************************************************/

#endif