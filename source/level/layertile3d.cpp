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
void	CLayerTile3d::init(VECTOR &MapPos,int Shift,int Width,int Height)
{
int		Size=Width*Height;

		ASSERT(Width>=SCREEN_TILE_WIDTH);
		ASSERT(Height>=SCREEN_TILE_HEIGHT);

		MapXYShift=Shift;
		PrimGridWidth=Width;
		PrimGridHeight=Height;

		PrimGrid=(sPrimGridElem3d*) MemAlloc(Size*sizeof(sPrimGridElem3d),"3d PrimGrid");
		ASSERT(PrimGrid);
		MapX=0;
		MapY=0;
		for (int Y=0; Y<PrimGridHeight; Y++)
		{
			for (int X=0; X<PrimGridWidth; X++)
			{
				sPrimGridElem3d	*ThisElem=GetGridPos3d(X,Y);
// Tile prim
				setTSprt16(&ThisElem->Prim);
				setTSetShadeTex(&ThisElem->Prim,1);
// Table			
				ThisElem->Right=GetGridPos3d(X+1,Y);
				ThisElem->Down=GetGridPos3d(X,Y+1);
			}
		}
		UpdateWholeMap();		

}

/*****************************************************************************/
void	CLayerTile3d::shutdown()
{
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
// Get (wrapped) PrimGrid pos
sPrimGridElem3d	*CLayerTile3d::GetGridPos3d(int X,int Y)
{
sPrimGridElem3d	*ThisGrid=(sPrimGridElem3d*)PrimGrid;
int		Pos;

/**/	X%=PrimGridWidth;
/**/	Y%=PrimGridHeight;
/**/	Pos=(X+(Y*PrimGridWidth));

/**/	return(ThisGrid+Pos);
}

/*****************************************************************************/
// Get (wrapped) Map pos
sTileMapElem3d	*CLayerTile3d::GetMapPos3d(int X,int Y)
{
sTileMapElem3d	*ThisMap=(sTileMapElem3d*)Map;
int		Pos;

/**/	X%=MapWidth;
/**/	Y%=MapHeight;
/**/	Pos=(X+(Y*MapWidth));
	
/**/	return(ThisMap+Pos);

}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CLayerTile3d::UpdateRow(int X,int Y)
{
sPrimGridElem3d	*Grid=GetGridPos3d(X,Y);
sTileMapElem3d	*MapPtr=GetMapPos3d(X,Y);

		for (int i=0; i<SCREEN_TILE_WIDTH; i++)
		{
// Tile prim
			TSPRT_16	*Prim=&Grid->Prim;
/**/		sTile		*Tile=&TileList[MapPtr->Tile];
/**/		setTSprtTPage(Prim,Tile->TPage);
			*(u32*)&Prim->u0=*(u32*)&Tile->u0;	// copy uv AND clut
/**/		Grid->Tile=MapPtr->Tile;
/**/		Grid->Flags=MapPtr->Flags;
// Next Elem
			MapPtr++;
			Grid=(sPrimGridElem3d *)Grid->Right;
		}

}

/*****************************************************************************/
void	CLayerTile3d::UpdateColumn(int X,int Y)
{
sPrimGridElem3d	*Grid=GetGridPos3d(X,Y);
sTileMapElem3d	*MapPtr=GetMapPos3d(X,Y);

		for (int i=0; i<SCREEN_TILE_HEIGHT; i++)
		{
// Tile prim
			TSPRT_16	*Prim=&Grid->Prim;
/**/		sTile		*Tile=&TileList[MapPtr->Tile];
/**/		setTSprtTPage(Prim,Tile->TPage);
			*(u32*)&Prim->u0=*(u32*)&Tile->u0;	// copy uv AND clut
/**/		Grid->Tile=MapPtr->Tile;
/**/		Grid->Flags=MapPtr->Flags;
// Next Elem
			MapPtr+=MapWidth;
			Grid=(sPrimGridElem3d *)Grid->Down;
		}
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#define	BLOCK_MULT	16
void	CLayerTile3d::render()
{
sPrimGridElem3d	*Grid=GetGridPos3d(MapX,MapY);
s16				TileX,TileY;
VECTOR			BlkPos;
s32				BlkXStore;

// Setup shift bits of pos
		TileY=-ShiftY;
		BlkPos.vx=((-15*TILE_WIDTH)-ShiftX)*BLOCK_MULT;
		BlkPos.vy=((-8*TILE_HEIGHT)-ShiftY)*BLOCK_MULT;
		BlkXStore=BlkPos.vx;

// Render it!!
		for (int Y=0; Y<SCREEN_TILE_HEIGHT; Y++)
		{
			sPrimGridElem	*GridDown=(sPrimGridElem3d *)Grid->Down;
			TileX=-ShiftX;

			for (int X=0; X<SCREEN_TILE_WIDTH; X++)
			{
				TSPRT_16	*Prim=&Grid->Prim;
				if (Prim->clut)
				{ // Has 2d Data
/**/				Prim->x0=TileX;
/**/				Prim->y0=TileY;
					addPrimNoCheck(OtPtr,Prim);
				}
				if (Grid->Flags)
				{ // Has 3d Data
/**/				CMX_SetTransMtxXY(&BlkPos);
/**/				RenderBlock(Grid);
				}
				Grid=(sPrimGridElem3d *)Grid->Right;
				TileX+=TILE_WIDTH;
				BlkPos.vx+=TILE_WIDTH*BLOCK_MULT;
			}
			Grid=(sPrimGridElem3d *)GridDown;
			TileY+=TILE_HEIGHT;
			BlkPos.vx=BlkXStore;
			BlkPos.vy+=TILE_HEIGHT*BLOCK_MULT;
		}

}

/*****************************************************************************/
// NOTE: Tiles will be sorted by z order (cos they 'should' be simple objects
// NOTE: Tiles are split into facing strips, to reduce overdraw :o)
// NOTE: Matrix already setup for block

void	CLayerTile3d::RenderBlock(sPrimGridElem3d *Elem)
{
sTile		*Tile=&TileList[Elem->Tile];
u32			Flags=Elem->Flags;
sVtx		*P0,*P1,*P2;
POLY_FT3	*TPrimPtr=(POLY_FT3*)GetPrimPtr();
u16			*TileTable=Tile->TileTable;
u32			T0,T1,T2;
sTri		*TList=TriList+Tile->TriStart;

//--- Tris ---------------------------------------------------------------------------

			for (int i=0; i<TILE3D_FLAGS_MAX; i++)
			{
				int		TriCount=*TileTable++;		// Get Tri Count
				sTri	*NextList=TList+TriCount;
				if (Flags & 1)
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
						addPrimNoCheck(OtPtr,TPrimPtr);
						gte_stsxy3_ft3(TPrimPtr);
						TPrimPtr++;
						}
					}
				TList=NextList;
				Flags>>=1;
			}
		SetPrimPtr((u8*)TPrimPtr);
}

