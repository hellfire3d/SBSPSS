/************************/
/*** Tile Layer Class ***/
/************************/

#include 	"system\global.h"
#include	<DStructs.h>
#include 	"utils\utils.h"
#include	"system\vid.h"
#include 	"gfx\prim.h"

#include	"LayerTile.h"

const u32		XInc=16<<0;
const u32		YInc=16<<16;

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
	if (TileTable[0].Table) MemFree(TileTable[0].Table);
	if (TileTable[1].Table) MemFree(TileTable[1].Table);
}

/*****************************************************************************/
sTileTable	&CLayerTile::GetTileTable()
{
	return(TileTable[FrameFlipFlag]);
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CLayerTile::init(VECTOR &MapPos,int Shift,int Width,int Height)
{
int		Size=Width*Height;

		ASSERT(Width>=SCREEN_TILE_WIDTH);
		ASSERT(Height>=SCREEN_TILE_HEIGHT);

		MapXYShift=Shift;
		TileTableWidth=Width;
		TileTableHeight=Height;

		for (int Buffer=0; Buffer<2; Buffer++)
		{
			sTileTableElem	*Table=(sTileTableElem*) MemAlloc(Size*sizeof(sTileTableElem),"2d TileTable");
			TileTable[Buffer].Table=Table;
			TileTable[Buffer].MapX=0;
			TileTable[Buffer].MapY=0;

			for (int Y=0; Y<Height; Y++)
			{

				for (int X=0; X<Width; X++)
				{
					sTileTableElem	*ThisElem=&Table[X+(Y*Width)];
// Tile prim
					TSPRT_16		*Prim=&ThisElem->Prim;
					setTSprt16(Prim);
					setTSetShadeTex(Prim,1);
// Table			
					int TableR=(X+1) % Width;
					int TableD=(Y+1) % Height;
					ThisElem->Right=&Table[TableR+(Y*Width)];
					ThisElem->Down=&Table[X+(TableD*Width)];
				}

			}
			UpdateWholeMap(TileTable[Buffer]);		
		}
}

/*****************************************************************************/
void	CLayerTile::UpdateWholeMap(sTileTable &ThisTable)
{
sTileTableElem	*Table=ThisTable.Table;
sTileMapElem	*MapPtr=Map;

// Calc (wrapped) Map/Table pos
			Table+=CalcTableOfs(ThisTable.MapX,ThisTable.MapY);
			MapPtr+=CalcMapOfs(ThisTable.MapX,ThisTable.MapY);

			for (int Y=0; Y<TileTableHeight; Y++)
			{
				sTileTableElem	*TableDown=Table->Down;
				sTileMapElem	*MapDown=MapPtr+MapWidth;
				
				for (int X=0; X<TileTableWidth; X++)
				{
					TSPRT_16	*Prim=&Table->Prim;
/**/				sTile		*Tile=&TileList[MapPtr->Tile];
					setTSprtTPage(Prim,Tile->TPage);
					*(u32*)&Prim->u0=*(u32*)&Tile->u0;			// copy uv AND clut
					Table->Tile=Tile;
					Table->TileFlags=Tile->TriCount;
					Table++;
					MapPtr++;
				}
				Table=TableDown;
				MapPtr=MapDown;
			}
				
}

/*****************************************************************************/
void	CLayerTile::shutdown()
{

}

/*****************************************************************************/
int		CLayerTile::CalcTableOfs(int X,int Y)
{
/**/	X%=TileTableWidth;
/**/	Y%=TileTableHeight;

/**/	return(X+(Y*TileTableWidth));

}

/*****************************************************************************/
int		CLayerTile::CalcMapOfs(int X,int Y)
{
/**/	X%=MapWidth;
/**/	Y%=MapHeight;

/**/	return(X+(Y*MapWidth));

}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CLayerTile::think(VECTOR &MapPos)
{
// Update rows and Columns :o)
sTileTable	&ThisTable=TileTable[FrameFlipFlag];
int			XPos=MapPos.vx>>MapXYShift;
int			YPos=MapPos.vy>>MapXYShift;
int			ShiftX=15-(XPos&15);
int			ShiftY=15-(YPos&15);

			ThisTable.ShiftXY=ShiftY<<16 | ShiftX<<0;

int			NewX=XPos>>4;
int			NewY=YPos>>4;
int			OldX=ThisTable.MapX;
int			OldY=ThisTable.MapY;

			if (NewX>OldX)
			{	// update right column
				UpdateColumn(NewX+SCREEN_TILE_WIDTH-1,NewY,ThisTable);
			}
			else
			if (NewX<OldX)
			{	// update left column
				UpdateColumn(NewX,NewY,ThisTable);
			}

			if (NewY>OldY)
			{	// update bottom row
				UpdateRow(NewX,NewY+SCREEN_TILE_HEIGHT-2,ThisTable);
			}
			else
			if (NewY<OldY)
			{	// update top row
				UpdateRow(NewX,NewY,ThisTable);
			}

			ThisTable.MapX=NewX;
			ThisTable.MapY=NewY;

			ThisTable.MapX=NewX;
			ThisTable.MapY=NewY;

}

/*****************************************************************************/
void	CLayerTile::UpdateRow(int MapX,int MapY,sTileTable &ThisTable)
{
sTileTableElem	*Table=ThisTable.Table;
sTileMapElem	*MapPtr=Map;

// Calc (wrapped) Map/Table pos
		Table+=CalcTableOfs(MapX,MapY);
		MapPtr+=CalcMapOfs(MapX,MapY);

		for (int i=0; i<SCREEN_TILE_WIDTH; i++)
		{
// Tile prim
			TSPRT_16	*Prim=&Table->Prim;
/**/		sTile		*Tile=&TileList[MapPtr->Tile];
/**/		setTSprtTPage(Prim,Tile->TPage);
			*(u32*)&Prim->u0=*(u32*)&Tile->u0;	// copy uv AND clut
			Table->Tile=Tile;
			Table->TileFlags=Tile->TriCount;
			MapPtr+=1;
			Table=Table->Right;
		}

}

/*****************************************************************************/
void	CLayerTile::UpdateColumn(int MapX,int MapY,sTileTable &ThisTable)
{
sTileTableElem	*Table=ThisTable.Table;
sTileMapElem	*MapPtr=Map;

// Calc (wrapped) Map/Table pos
		Table+=CalcTableOfs(MapX,MapY);
		MapPtr+=CalcMapOfs(MapX,MapY);

		for (int i=0; i<SCREEN_TILE_HEIGHT; i++)
		{
// Tile prim
			TSPRT_16	*Prim=&Table->Prim;
/**/		sTile		*Tile=&TileList[MapPtr->Tile];
/**/		setTSprtTPage(Prim,Tile->TPage);
			*(u32*)&Prim->u0=*(u32*)&Tile->u0;	// copy uv AND clut
			Table->Tile=Tile;
			Table->TileFlags=Tile->TriCount;


			MapPtr+=MapWidth;
			Table=Table->Down;
		}
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CLayerTile::render()
{
sTileTable		&ThisTable=TileTable[FrameFlipFlag];
sTileTableElem	*Table=ThisTable.Table;
u32				XYPos;

// Setup shift bits of pos
		XYPos=ThisTable.ShiftXY;

// Calc (wrapped) Start pos
		Table+=CalcTableOfs(ThisTable.MapX,ThisTable.MapY);

// Render it!!		
		for (int TileY=0; TileY<SCREEN_TILE_HEIGHT; TileY++)
		{
			sTileTableElem	*TableDown=Table->Down;
			u32				XYPosDown=XYPos+YInc;
			for (int TileX=0; TileX<SCREEN_TILE_WIDTH; TileX++)
			{
				TSPRT_16	*Prim=&Table->Prim;
				if (Prim->clut)
				{
					*(u32*)&Prim->x0=XYPos;
/**/				AddPrim(OtPtr,Prim);
				}
				Table=Table->Right;
				XYPos+=XInc;
			}
			Table=TableDown;
			XYPos=XYPosDown;
		}
}

/*****************************************************************************/
void	CLayerTile::renderSolid()
{
sTileTable		&ThisTable=TileTable[FrameFlipFlag];
sTileTableElem	*Table=ThisTable.Table;
u32				XYPos;

// Setup shift bits of pos
		XYPos=ThisTable.ShiftXY;

// Calc (wrapped) Start pos
		Table+=CalcTableOfs(ThisTable.MapX,ThisTable.MapY);

// Render it!!		
		for (int TileY=0; TileY<SCREEN_TILE_HEIGHT; TileY++)
		{
			sTileTableElem	*TableDown=Table->Down;
			u32				XYPosDown=XYPos+YInc;
			for (int TileX=0; TileX<SCREEN_TILE_WIDTH; TileX++)
			{
				TSPRT_16	*Prim=&Table->Prim;
				*(u32*)&Prim->x0=XYPos;
/**/			AddPrim(OtPtr,Prim);
				Table=Table->Right;
				XYPos+=XInc;
			}
			Table=TableDown;
			XYPos=XYPosDown;
		}
}


/*****************************************************************************/
VECTOR	asd={0,0,512};

void	CLayerTile::render3d()
{
sTileTable		&ThisTable=TileTable[FrameFlipFlag];
sTileTableElem	*Table=ThisTable.Table;
u32				XYPos,XYPos3d;
MATRIX			_Mtx,*Mtx=&_Mtx;
	
		SetIdent(Mtx);
		Mtx->t[2]=asd.vz;
		gte_SetRotMatrix(Mtx);

// Setup shift bits of pos
		XYPos=ThisTable.ShiftXY;

// Calc (wrapped) Start pos
		Table+=CalcTableOfs(ThisTable.MapX,ThisTable.MapY);

// Render it!!		
		for (int TileY=0; TileY<SCREEN_TILE_HEIGHT; TileY++)
		{
			sTileTableElem	*TableDown=Table->Down;
			u32				XYPosDown=XYPos+YInc;
			for (int TileX=0; TileX<SCREEN_TILE_WIDTH; TileX++)
			{
				TSPRT_16	*Prim=&Table->Prim;
				if (Prim->clut)
				{ // 2d tile
					*(u32*)&Prim->x0=XYPos;
/**/				AddPrim(OtPtr,Prim);
				}
				if (Table->TileFlags)
				{ // 3d tile
					u32	SX=(ThisTable.ShiftXY &15);
					u32	SY=(ThisTable.ShiftXY>>16);
					Mtx->t[0]=((TileX-15)*16)+SX;
					Mtx->t[1]=((TileY-10)*16)+SY;
					gte_SetTransMatrix(Mtx);
					RenderBlock(Table->Tile,Table->TileFlags);
				}
				Table=Table->Right;
				XYPos+=XInc;
			}
			Table=TableDown;
			XYPos=XYPosDown;
		}
}

/*****************************************************************************/
// NOTE: Tiles will be sorted by z order (cos they 'should' be simple objects
// NOTE: Tiles will be split into facing strips, to reduce overdraw :o)
// NOTE: Matrix already setup for tile
void	CLayerTile::RenderBlock(sTile *Tile,u32 Flags)
{
sVtx		*P0,*P1,*P2;
s32			ClipZ=0;
POLY_FT3	*TPrimPtr=(POLY_FT3*)GetPrimPtr();
int			TriCount=Tile->TriCount;
sTri		*TList=TriList+Tile->TriList;
u32			T0,T1,T2;

//--- Tris ---------------------------------------------------------------------------
			
			while (TriCount--)
		        {
				P0=&VtxList[TList->P0]; P1=&VtxList[TList->P1]; P2=&VtxList[TList->P2];
				gte_ldv3(P0,P1,P2);
				setPolyFT3(TPrimPtr);
				setShadeTex(TPrimPtr,1);
				setlen( TPrimPtr, GPU_PolyFT3Tag);
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

		SetPrimPtr((u8*)TPrimPtr);
}
