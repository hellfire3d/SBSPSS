/************************/
/*** Tile Layer Class ***/
/************************/

#ifndef __LAYER_TILE_H__
#define __LAYER_TILE_H__


/*****************************************************************************/
class CLayerTile : public CLayer
{
public:
		enum LAYER_TILE_SUBTYPE
		{
			LAYER_TILE_SUBTYPE_BACK=0,
			LAYER_TILE_SUBTYPE_MID,
			LAYER_TILE_SUBTYPE_ACTION,
			LAYER_TILE_SUBTYPE_FORE,
			LAYER_TILE_SUBTYPE_MAX
		};

		CLayerTile(sLayerHdr *Hdr,sTile *_TileList,sTri *_TriList,sQuad *_QuadList,sVtx *_VtxList);
		~CLayerTile();

		void	init();
		void	shutdown();
		void	render();
		void	think(int _frames);

private:
		sTile			*TileList;
		sTri			*TriList;
		sQuad			*QuadList;
		sVtx			*VtxList;
		sTileMapElem	*Map;

};



/*****************************************************************************/

#endif