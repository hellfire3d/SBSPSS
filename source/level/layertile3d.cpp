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
CLayerTile3d::CLayerTile3d(sLayerHdr *Hdr,sTile *TileList,sTri *TriList,sQuad *QuadList,sVtx *VtxList) : CLayerTile(Hdr,TileList,TriList,QuadList,VtxList)
{
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
		CreateRenderFlagTable();
}

/*****************************************************************************/
void	CLayerTile3d::shutdown()
{
		MemFree(RenderFlagTable);
}

/*****************************************************************************/
void	CLayerTile3d::CreateRenderFlagTable()
{
s16		*Ptr;

		RenderFlagTable=(s16*) MemAlloc(SCREEN_TILE_WIDTH*SCREEN_TILE_HEIGHT*sizeof(s32),"RenderFlagTable");
		Ptr=RenderFlagTable;

		for (int Y=0; Y<SCREEN_TILE_HEIGHT; Y++)
		{
			for (int X=0; X<SCREEN_TILE_WIDTH; X++)
			{
				s16	Flag=1<<TILE3D_FLAGS_F;
				if (X<(SCREEN_TILE_WIDTH/2)+1) 
				{
					Flag|=1<<TILE3D_FLAGS_R;
				}
				if (X>(SCREEN_TILE_WIDTH/2)-1) 
				{
					Flag|=1<<TILE3D_FLAGS_L;
				}
				if (Y<(SCREEN_TILE_HEIGHT/2)+1) 
				{
					Flag|=1<<TILE3D_FLAGS_D;
				}
				if (Y>(SCREEN_TILE_HEIGHT/2)-1) 
				{
					Flag|=1<<TILE3D_FLAGS_U;
				}
				*Ptr++=Flag;
			}

		}
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#define	BLOCK_MULT	16

void	CLayerTile3d::render()
{
sTileMapElem3d	*MapPtr=GetMapPos3d();
u8				*PrimPtr=GetPrimPtr();
s16				TileX,TileY;
VECTOR			BlkPos;
s32				BlkXStore;
sOT				*ThisOT=OtPtr+LayerOT;
s16				*RenderFlags=RenderFlagTable;

// Setup shift bits of pos
		TileY=-ShiftY;
		BlkPos.vx=((-15*TILE_WIDTH)-ShiftX)*BLOCK_MULT;
		BlkPos.vy=((-7*TILE_HEIGHT)-ShiftY)*BLOCK_MULT;
		BlkXStore=BlkPos.vx;

// Render it!!
		for (int Y=0; Y<RenderH; Y++)
		{
			sTileMapElem3d	*MapRow=MapPtr;
			TileX=-ShiftX;

			for (int X=0; X<RenderW; X++)
			{
/**/			sTile		*Tile=&TileList[MapRow->Tile];

				if (Tile->Clut)
				{
					TSPRT_16	*SprPtr=(TSPRT_16*)PrimPtr;
					setTSprt16(SprPtr);
					setTSetShadeTex(SprPtr,1);
/**/				SprPtr->x0=TileX;
/**/				SprPtr->y0=TileY;
/**/				setTSprtTPage(SprPtr,Tile->TPage);
					*(u32*)&SprPtr->u0=*(u32*)&Tile->u0;	// copy uv AND clut
					addPrimNoCheck(ThisOT,SprPtr);
					PrimPtr+=sizeof(TSPRT_16);
				}
				if (MapRow->Flags)
				{ // Has 3d Data
					CMX_SetTransMtxXY(&BlkPos);
					PrimPtr=RenderBlock(Tile,MapRow->Flags & *RenderFlags,PrimPtr);
				}
				MapRow++;
				TileX+=TILE_WIDTH;
				BlkPos.vx+=TILE_WIDTH*BLOCK_MULT;
				RenderFlags++;
			}
			MapPtr+=MapWidth;
			TileY+=TILE_HEIGHT;
			BlkPos.vx=BlkXStore;
			BlkPos.vy+=TILE_HEIGHT*BLOCK_MULT;
		}
		SetPrimPtr(PrimPtr);
}

/*****************************************************************************/
// NOTE: Tiles will be sorted by z order (cos they 'should' be simple objects
// NOTE: Tiles are split into facing strips, to reduce overdraw :o)
// NOTE: Matrix already setup for block

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
