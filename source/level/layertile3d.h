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
	s16		Mtx[4];				// 8
	DVECTOR	*DeltaTab[8];		// 32
	s32		ClipCode;			// 4
	s8		Pad[20];
};
extern	sFlipTable	FlipTable[];

/*****************************************************************************/
class	FontBank;
class CLayerTile3d : public CLayerTile
{
public:
		CLayerTile3d(sLevelHdr *LevelHdr,sLayerHdr *Hdr,u8 *_RGBMap,u8 *_RGBTable);
		~CLayerTile3d();

		void			init(DVECTOR &MapPos,int Shift);
		void			shutdown();
		void			think(DVECTOR &MapPos);
		void			render();

protected:
		void			CacheElemVtx(sElem3d *Elem);
		void			CalcDelta();

		sElem3d			*ElemBank3d;
		sTri			*TriList;
		sQuad			*QuadList;
		sVtx			*VtxList;
		u16				*VtxIdxList;
		DVECTOR			RenderOfs;
		u8				*RGBMap;
		u8				*RGBTable;

		s16				*DeltaTableX[16];
		s16				*DeltaTableY[16];
//		s16				*BTableX[16];
//		s16				*BTableY[16];
		DVECTOR			DeltaFOfs;
		DVECTOR			DeltaBOfs;
		s16				DeltaF,DeltaB;


};



/*****************************************************************************/

#endif