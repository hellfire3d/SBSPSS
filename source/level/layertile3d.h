/***************************/
/*** 3d Tile Layer Class ***/
/***************************/

#ifndef __LAYER_TILE_3D_Hx__
#define __LAYER_TILE_3D_Hx__


/*****************************************************************************/
class	FontBank;
class CLayerTile3d : public CLayerTile
{
public:
		CLayerTile3d(sLvlHdr *LvlHdr,sLayerHdr *Hdr);
		~CLayerTile3d();

		enum
		{
			SCREEN_TILE_ADJ_UP		=(2*1),
			SCREEN_TILE_ADJ_DOWN	=(2*1),
			SCREEN_TILE_ADJ_LEFT	=(2*1),
			SCREEN_TILE_ADJ_RIGHT	=(5*1),
			SCREEN_TILE_WIDTH3D=	SCREEN_TILE_WIDTH+SCREEN_TILE_ADJ_RIGHT,
			SCREEN_TILE_HEIGHT3D=	SCREEN_TILE_HEIGHT+SCREEN_TILE_ADJ_DOWN,
			BLOCK_MULT			=	16
		};

		void			init(DVECTOR &MapPos,int Shift);
		void			shutdown();
		void			think(DVECTOR &MapPos);
		void			render();

protected:
		sTile3d			*TileBank3d;
		sTri			*TriList;
		sQuad			*QuadList;
		sVtx			*VtxList;
		DVECTOR			RenderOfs;
};



/*****************************************************************************/

#endif