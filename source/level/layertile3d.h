/***************************/
/*** 3d Tile Layer Class ***/
/***************************/

#ifndef __LAYER_TILE_3D_Hx__
#define __LAYER_TILE_3D_Hx__


/*****************************************************************************/
class CLayerTile3d : public CLayerTile
{
public:
		CLayerTile3d(sLayerHdr *Hdr,sTile *TileBank);
		~CLayerTile3d();

		enum
		{
			SCREEN_TILE_ADJ_UP=2,
			SCREEN_TILE_ADJ_DOWN=2,
			SCREEN_TILE_ADJ_LEFT=2,
			SCREEN_TILE_ADJ_RIGHT=5,
			SCREEN_TILE_WIDTH3D=	SCREEN_TILE_WIDTH+SCREEN_TILE_ADJ_RIGHT,
			SCREEN_TILE_HEIGHT3D=	SCREEN_TILE_HEIGHT+SCREEN_TILE_ADJ_DOWN,
		};

		void			init(DVECTOR &MapPos,int Shift);
		void			shutdown();
		void			think(DVECTOR &MapPos);
		void			render();

protected:
		sLayer3d		*LayerInfo;
		sTri			*TriList;
		sQuad			*QuadList;
		sVtx			*VtxList;
};



/*****************************************************************************/

#endif