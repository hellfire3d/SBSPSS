/***************************/
/*** 3d Tile Layer Class ***/
/***************************/

#include 	"system\global.h"
#include	<DStructs.h>
#include 	"utils\utils.h"
#include 	"gfx\prim.h"


#include	"LayerTile.h"
#include	"LayerTile3d.h"


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
CLayerTile3d::CLayerTile3d(sLayerHdr *Hdr,sTile *TileBank) : CLayerTile(Hdr,TileBank)
{
	
	LayerInfo=(sLayer3d*)MakePtr(Hdr,sizeof(sLayerHdr));

	TriList=(sTri*)		MakePtr(Hdr,LayerInfo->TriList);
	QuadList=(sQuad*)	MakePtr(Hdr,LayerInfo->QuadList);
	VtxList=(sVtx*)		MakePtr(Hdr,LayerInfo->VtxList);

	Map=(sTileMapElem*)MakePtr(Hdr,sizeof(sLayerHdr)+sizeof(sLayer3d));

}

/*****************************************************************************/
CLayerTile3d::~CLayerTile3d()
{
}


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/

void	CLayerTile3d::init(DVECTOR &MapPos,int Shift)
{
		CLayerTile::init(MapPos,Shift);
}

/*****************************************************************************/
void	CLayerTile3d::shutdown()
{
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#define	BLOCK_MULT	16
int	XOfs=-16;
int	YOfs=-7;
int	asd=16;
int	TriMax=0;
void	CLayerTile3d::render()
{
sTileMapElem	*MapPtr=GetMapPos();
//sTileMapElem	*MapPtr=Map;

u8				*PrimPtr=GetPrimPtr();
POLY_FT3		*TPrimPtr=(POLY_FT3*)PrimPtr;
VECTOR			BlkPos;
sOT				*ThisOT=OtPtr+LayerOT;
sVtx			*P0,*P1,*P2;
u32				T0,T1,T2;

		TriMax=0;
// Setup shift bits of pos
		BlkPos.vx=-(((MapXY.vx-XOfs)*asd)+ShiftX)*asd;
		BlkPos.vy=-(((MapXY.vy-YOfs)*asd)+ShiftY)*asd;

		CMX_SetTransMtxXY(&BlkPos);

		for (int Y=0; Y<RenderH; Y++)
		{
			// Get strip render size
			int		TriStart=MapPtr[0];
			int		TriEnd=MapPtr[RenderW];
			int		TriCount=TriEnd-TriStart;	

			sTri	*TList=&TriList[TriStart];

			while (TriCount>0)
			{
				P0=&VtxList[TList->P0]; P1=&VtxList[TList->P1]; P2=&VtxList[TList->P2];
				gte_ldv3(P0,P1,P2);
				setPolyFT3(TPrimPtr);
				setShadeTex(TPrimPtr,1);
				setlen(TPrimPtr, GPU_PolyFT3Tag);
				gte_rtpt_b();

				T0=*(u32*)&TList->uv0;		// Get UV0 & TPage
				T1=*(u32*)&TList->uv1;		// Get UV1 & Clut
				T2=*(u16*)&TList->uv2;		// Get UV2
				*(u32*)&TPrimPtr->u0=T0;	// Set UV0
				*(u32*)&TPrimPtr->u1=T1;	// Set UV1
				*(u16*)&TPrimPtr->u2=T2;	// Set UV2

				TList++;
				gte_stsxy3_ft3(TPrimPtr);
				int	ClipZ=NormalClip(*(u32*)&TPrimPtr->x0,*(u32*)&TPrimPtr->x1,*(u32*)&TPrimPtr->x2);
				if (ClipZ<=0)
				{
					addPrimNoCheck(ThisOT,TPrimPtr);
					TPrimPtr++;
				}

				TriCount--;
			}
			MapPtr+=MapWidth;
		}

		TriMax=((u8*)TPrimPtr-PrimPtr)/sizeof(POLY_FT3);

// Render it!!
/*
		for (int Y=0; Y<RenderH; Y++)
		{
			sTileMapElem	*MapRow=MapPtr;
			TileX=-ShiftX;

			for (int X=0; X<RenderW; X++)
			{
			sTile		*Tile=&TileList[MapRow->Elem];

				if (MapRow->Flags)
				{ // Has 3d Data
					CMX_SetTransMtxXY(&BlkPos);
				}
				MapRow++;
				TileX+=TILE_WIDTH;
				BlkPos.vx+=TILE_WIDTH*BLOCK_MULT;
			}
			MapPtr+=MapWidth;
			TileY+=TILE_HEIGHT;
			BlkPos.vx=BlkXStore;
			BlkPos.vy+=TILE_HEIGHT*BLOCK_MULT;
		}
*/
		SetPrimPtr((u8*)TPrimPtr);
}

/*****************************************************************************/
// NOTE: Tiles will be sorted by z order (cos they 'should' be simple objects
// NOTE: Tiles are split into facing strips, to reduce overdraw :o)
// NOTE: Matrix already setup for block
#if	0
u8			*CLayerTile3d::RenderBlock(sTile *Tile,s16 RenderFlags,u8 *PrimPtr)
{
sVtx		*P0,*P1,*P2;
POLY_FT3	*TPrimPtr=(POLY_FT3*)PrimPtr;
u16			*TileTable=Tile->TileTable;
u32			T0,T1,T2;
sTri		*TList=TriList+Tile->TriStart;
sOT			*ThisOT=OtPtr+LayerOT;

//--- Tris ---------------------------------------------------------------------------
			
			for (int i=0; i<TILE3D_FLAGS_MAX; i++)
			{
				int		TriCount=*TileTable++;		// Get Tri Count
				sTri	*NextList=TList+TriCount;
//				if (RenderFlags & 1)
				{
					while (TriCount--)
						{
						P0=&VtxList[TList->P0]; P1=&VtxList[TList->P1]; P2=&VtxList[TList->P2];
						gte_ldv3(P0,P1,P2);
/**/					setPolyFT3(TPrimPtr);
/**/					setShadeTex(TPrimPtr,1);
/**/					setlen(TPrimPtr, GPU_PolyFT3Tag);
						gte_rtpt_b();

						T0=*(u32*)&TList->uv0;		// Get UV0 & TPage
						T1=*(u32*)&TList->uv1;		// Get UV1 & Clut
						T2=*(u16*)&TList->uv2;		// Get UV2
						*(u32*)&TPrimPtr->u0=T0;	// Set UV0
						*(u32*)&TPrimPtr->u1=T1;	// Set UV1
						*(u16*)&TPrimPtr->u2=T2;	// Set UV2

						TList++;
						addPrimNoCheck(ThisOT,TPrimPtr);
						gte_stsxy3_ft3(TPrimPtr);
						TPrimPtr++;
						}
					}
				TList=NextList;
				RenderFlags>>=1;
			}
		return((u8*)TPrimPtr);

}
#endif