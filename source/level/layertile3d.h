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

		void			init(DVECTOR &MapPos,int Shift);
		void			shutdown();
		void			think(DVECTOR &MapPos);
		void			render();

protected:
		sElem3d			*ElemBank3d;
		sTri			*TriList;
		sQuad			*QuadList;
		sVtx			*VtxList;
		DVECTOR			RenderOfs;
};



/*****************************************************************************/

#endif