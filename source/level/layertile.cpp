/************************/
/*** Tile Layer Class ***/
/************************/

#include 	"system\global.h"
#include	<DStructs.h>
#include 	"utils\utils.h"
#include	"system\vid.h"
#include 	"gfx\prim.h"

#include	"LayerTile.h"

const u32	XInc=16<<0;
const u32	YInc=16<<16;

/*****************************************************************************/
// Uses single buffer. Hopefully this will be adequate

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
CLayerTile::CLayerTile(sLayerHdr *Hdr,sTile *_TileList,sTri *_TriList,sQuad *_QuadList,sVtx *_VtxList)
{
		LayerHdr=Hdr;
		TileList=_TileList;
		TriList=_TriList;
		QuadList=_QuadList;
		VtxList=_VtxList;
		Map=(sTileMapElem*)MakePtr(Hdr,sizeof(sLayerHdr));
		MapWidth=LayerHdr->Width;
		MapHeight=LayerHdr->Height;

}

/*****************************************************************************/
CLayerTile::~CLayerTile()
{
		MemFree(PrimGrid);
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CLayerTile::init(VECTOR &MapPos,int Shift,int Width,int Height)
{
int			Size=Width*Height;

		ASSERT(Width>=SCREEN_TILE_WIDTH);
		ASSERT(Height>=SCREEN_TILE_HEIGHT);

		MapXYShift=Shift;
		PrimGridWidth=Width;
		PrimGridHeight=Height;

		PrimGrid=(sPrimGridElem*) MemAlloc(Size*sizeof(sPrimGridElem),"2d PrimGrid");
		ASSERT(PrimGrid);
		MapX=0;
		MapY=0;
		for (int Y=0; Y<PrimGridHeight; Y++)
		{
			for (int X=0; X<PrimGridWidth; X++)
			{
				sPrimGridElem	*ThisElem=GetGridPos(X,Y);
// Tile prim
				setTSprt16(&ThisElem->Prim);
				setTSetShadeTex(&ThisElem->Prim,1);
// Table			
				ThisElem->Right=GetGridPos(X+1,Y);
				ThisElem->Down=GetGridPos(X,Y+1);
			}
		}
		UpdateWholeMap();		
}

/*****************************************************************************/
// AS not critical, us row update routine for whole map
void	CLayerTile::UpdateWholeMap()
{
		for (int Y=0; Y<PrimGridHeight; Y++)
		{
			UpdateRow(MapX,MapY+Y);
		}
}

/*****************************************************************************/
void	CLayerTile::shutdown()
{

}

/*****************************************************************************/
// Get (wrapped) PrimGrid pos
sPrimGridElem	*CLayerTile::GetGridPos(int X,int Y)
{
int		Pos;

/**/	X%=PrimGridWidth;
/**/	Y%=PrimGridHeight;
/**/	Pos=(X+(Y*PrimGridWidth));

/**/	return(PrimGrid+Pos);
}

/*****************************************************************************/
// Get (wrapped) Map pos
sTileMapElem	*CLayerTile::GetMapPos(int X,int Y)
{
int		Pos;

/**/	X%=MapWidth;
/**/	Y%=MapHeight;
/**/	Pos=(X+(Y*MapWidth));
	
/**/	return(Map+Pos);

}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CLayerTile::think(VECTOR &MapPos)
{
// Update rows and Columns :o)
// As these are on the borders, they 'shouldnt' alter any being rendered
int			XPos=MapPos.vx>>MapXYShift;
int			YPos=MapPos.vy>>MapXYShift;
int			NewX=XPos>>4;
int			NewY=YPos>>4;

			ShiftX=XPos & 15;
			ShiftY=YPos & 15;

			if (NewX>MapX)
			{	// update right column
				UpdateColumn(NewX+SCREEN_TILE_WIDTH-1,MapY);
				MapX=NewX;
			}
			else
			if (NewX<MapX)
			{	// update left column
				UpdateColumn(NewX,MapY);
				MapX=NewX;
			}

			if (NewY>MapY)
			{	// update bottom row
				UpdateRow(MapX,NewY+SCREEN_TILE_HEIGHT-1);
				MapY=NewY;
			}
			else
			if (NewY<MapY)
			{	// update top row
				UpdateRow(MapX,NewY);
				MapY=NewY;
			}
}

/*****************************************************************************/
void	CLayerTile::UpdateRow(int X,int Y)
{
sPrimGridElem	*Table=GetGridPos(X,Y);
sTileMapElem	*MapPtr=GetMapPos(X,Y);

		for (int i=0; i<SCREEN_TILE_WIDTH; i++)
		{
// Tile prim
			TSPRT_16	*Prim=&Table->Prim;
/**/		sTile		*Tile=&TileList[MapPtr->Tile];
/**/		setTSprtTPage(Prim,Tile->TPage);
			*(u32*)&Prim->u0=*(u32*)&Tile->u0;	// copy uv AND clut
/**/		Table->Tile=MapPtr->Tile;
/**/		Table->Flags=MapPtr->Flags;
// Next Elem
			MapPtr++;
			Table=Table->Right;
		}

}

/*****************************************************************************/
void	CLayerTile::UpdateColumn(int X,int Y)
{
sPrimGridElem	*Table=GetGridPos(X,Y);
sTileMapElem	*MapPtr=GetMapPos(X,Y);

		for (int i=0; i<SCREEN_TILE_HEIGHT; i++)
		{
// Tile prim
			TSPRT_16	*Prim=&Table->Prim;
/**/		sTile		*Tile=&TileList[MapPtr->Tile];
/**/		setTSprtTPage(Prim,Tile->TPage);
			*(u32*)&Prim->u0=*(u32*)&Tile->u0;	// copy uv AND clut
/**/		Table->Tile=MapPtr->Tile;
/**/		Table->Flags=MapPtr->Flags;
// Next Elem
			MapPtr+=MapWidth;
			Table=Table->Down;
		}
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CLayerTile::render()
{
sPrimGridElem	*Table=GetGridPos(MapX,MapY);
s16				TileX,TileY;

// Setup shift bits of pos
		TileY=-ShiftY;

// Render it!!
		for (int Y=0; Y<SCREEN_TILE_HEIGHT; Y++)
		{
			sPrimGridElem	*TableDown=Table->Down;
			TileX=-ShiftX;

			for (int X=0; X<SCREEN_TILE_WIDTH; X++)
			{
				TSPRT_16	*Prim=&Table->Prim;
				if (Prim->clut)
				{
/**/				Prim->x0=TileX;
/**/				Prim->y0=TileY;
/**/				AddPrim(OtPtr,Prim);
				}
				Table=Table->Right;
				TileX+=TILE_WIDTH;
			}
			Table=TableDown;
			TileY+=TILE_HEIGHT;
		}
}

/*****************************************************************************/
void	CLayerTile::renderSolid()
{
sPrimGridElem	*Table=GetGridPos(MapX,MapY);
s16				TileX,TileY;

// Setup shift bits of pos
		TileY=-ShiftY;

// Render it!!
		for (int Y=0; Y<SCREEN_TILE_HEIGHT; Y++)
		{
			sPrimGridElem	*TableDown=Table->Down;
			TileX=-ShiftX;

			for (int X=0; X<SCREEN_TILE_WIDTH; X++)
			{
				TSPRT_16	*Prim=&Table->Prim;
/**/			Prim->x0=TileX;
/**/			Prim->y0=TileY;
/**/			AddPrim(OtPtr,Prim);
				Table=Table->Right;
				TileX+=TILE_WIDTH;
			}
			Table=TableDown;
			TileY+=TILE_HEIGHT;
		}
}

/*****************************************************************************/
#define	BLOCK_MULT	16
void	CLayerTile::render3d()
{
sPrimGridElem	*Table=GetGridPos(MapX,MapY);
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
			sPrimGridElem	*TableDown=Table->Down;
			TileX=-ShiftX;

			for (int X=0; X<SCREEN_TILE_WIDTH; X++)
			{
				TSPRT_16	*Prim=&Table->Prim;
				if (Prim->clut)
				{ // Has 2d Data
/**/				Prim->x0=TileX;
/**/				Prim->y0=TileY;
/**/				AddPrim(OtPtr,Prim);
				}
				if (Table->Flags)
				{ // Has 3d Data
/**/				CMX_SetTransMtxXY(&BlkPos);
/**/				RenderBlock(Table);
				}
				Table=Table->Right;
				TileX+=TILE_WIDTH;
				BlkPos.vx+=TILE_WIDTH*BLOCK_MULT;
			}
			Table=TableDown;
			TileY+=TILE_HEIGHT;
			BlkPos.vx=BlkXStore;
			BlkPos.vy+=TILE_HEIGHT*BLOCK_MULT;
		}
}

/*****************************************************************************/
// NOTE: Tiles will be sorted by z order (cos they 'should' be simple objects
// NOTE: Tiles are split into facing strips, to reduce overdraw :o)
// NOTE: Matrix already setup for block

void	CLayerTile::RenderBlock(sPrimGridElem *Elem)
{
sTile		*Tile=&TileList[Elem->Tile];
u32			Flags=Elem->Flags;
sVtx		*P0,*P1,*P2;
POLY_FT3	*TPrimPtr=(POLY_FT3*)GetPrimPtr();
sTileTable	*TileTable=Tile->TileTable;
u32			T0,T1,T2;

//--- Tris ---------------------------------------------------------------------------

			for (int i=0; i<TILE3D_FLAGS_MAX; i++)
			{
				int		TriCount=TileTable->TriCount;
				sTri	*TList=TriList+TileTable->TriList;
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
						addPrim(OtPtr,TPrimPtr);
						gte_stsxy3_ft3(TPrimPtr);
						TPrimPtr++;
						}
					}
				TileTable++;
				Flags>>=1;
			}
		SetPrimPtr((u8*)TPrimPtr);
}
