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
		CLayerTile3d(sLevelHdr *LevelHdr,sLayerHdr *Hdr);
		~CLayerTile3d();

		enum
		{
			TILE3D_WIDTH			=16,
			TILE3D_HEIGHT			=16,
			BLOCK_MULT				=16,
			SCREEN_TILE_ADJ_UP		=1,
			SCREEN_TILE_ADJ_LEFT	=3,
			SCREEN_TILE3D_WIDTH		=40,
			SCREEN_TILE3D_HEIGHT	=18,
			RENDER_X_PIX_OFS		=-8,
			RENDER_Y_PIX_OFS		=0,

			RENDER_X_OFS			=-(BLOCK_MULT*15)-(SCREEN_TILE_ADJ_LEFT*BLOCK_MULT)+RENDER_X_PIX_OFS,
			RENDER_Y_OFS			=-(BLOCK_MULT*7)-(SCREEN_TILE_ADJ_UP*BLOCK_MULT)+RENDER_Y_PIX_OFS,

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