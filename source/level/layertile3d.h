/***************************/
/*** 3d Tile Layer Class ***/
/***************************/

#ifndef __LAYER_TILE_3D_Hx__
#define __LAYER_TILE_3D_Hx__


#define CMX_SetRotMatrixXY( r0 ) __asm__  (       \
    "lw $12, 0( %0 );"                  \
    "lw $13, 4( %0 );"                  \
    "ctc2   $12, $0;"                   \
    "ctc2   $13, $2;"                   \
    :                           \
    : "r"( r0 )                     \
    : "$12", "$13")

struct	sFlipTable
{
	s16		Mtx[4];
	s32		ClipCode;
};
extern	sFlipTable	FlipTable[];

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