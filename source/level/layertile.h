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
			LAYER_TILE_TYPE_ACTION,
			LAYER_TILE_TYPE_MID,
			LAYER_TILE_TYPE_BACK,
			LAYER_TILE_TYPE_MAX
		};

		CLayerTile(sLevelHdr *LevelHdr,sLayerHdr *Hdr);
virtual	~CLayerTile();

virtual	void			init(DVECTOR &MapPos,int Shift);
virtual	void			shutdown();
virtual	void			think(DVECTOR &MapPos);
virtual	void			render();

		int				GetMapOfs()			{return(MapXY.vx+(MapXY.vy*MapWidth));}
//virtual	sTileMapElem	*GetMapPos()		{return(Map+GetMapOfs());}
virtual	sTileMapElem	*getMapPtr(int _x,int _y)			{return(&Map[(_x>>4)+((_y>>4)*MapWidth)]);}

protected:

		sLayerHdr		*LayerHdr;
		sElem2d			*ElemBank2d;

		int				MapWidth,MapHeight,MapXYShift;
		int				RenderW,RenderH;

		DVECTOR			MapXY;
		u16				ShiftX,ShiftY;
		
		sTileMapElem	*Map;
		TSPRT			*PrimBank[2];
		int				PrimBankID;
};



/*****************************************************************************/

#endif