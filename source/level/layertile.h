/************************/
/*** Tile Layer Class ***/
/************************/

#ifndef __LAYER_TILE_H__
#define __LAYER_TILE_H__

#include	<dstructs.h>
#include	"gfx\prim.h"


/*****************************************************************************/
const	s32	LayerOT=MAX_OT-1;

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
			SCREEN_TILE_WIDTH=34,
			SCREEN_TILE_HEIGHT=18,
			SCREEN_TILE_SIZE=SCREEN_TILE_WIDTH*SCREEN_TILE_HEIGHT
		};

		CLayerTile(sLayerHdr *Hdr,sTile *TileList,sTri *TriList,sQuad *QuadList,sVtx *VtxList);
virtual	~CLayerTile();

virtual	void			init(DVECTOR &MapPos,int Shift);
virtual	void			shutdown();
virtual	void			think(DVECTOR &MapPos);
virtual	void			render();

		int				GetMapOfs()			{return(MapXY.vx+(MapXY.vy*MapWidth));}
virtual	sTileMapElem	*GetMapPos()		{return(Map+GetMapOfs());}
protected:


		sLayerHdr		*LayerHdr;
		sTile			*TileList;
		sTri			*TriList;
		sQuad			*QuadList;
		sVtx			*VtxList;

		int				MapWidth,MapHeight,MapXYShift;
		int				RenderW,RenderH;

		DVECTOR			MapXY;
		u16				ShiftX,ShiftY;
		
		sTileMapElem	*Map;
};



/*****************************************************************************/

#endif