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

}

/*****************************************************************************/
CLayerTile::~CLayerTile()
{
	if (TileMap2d[0].List) MemFree(TileMap2d[0].List);
	if (TileMap2d[1].List) MemFree(TileMap2d[1].List);
}

/*****************************************************************************/
sTileMap2d	&CLayerTile::GetTileMap()
{
	return(TileMap2d[FrameFlipFlag]);
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
// NEED TO UPDATE FOR DIFF MAP POS's
void	CLayerTile::init(VECTOR &MapPos,int Shift,int Width,int Height)
{
int		Size=Width*Height;
int		MapWidth=GetWidth();
int		MapHeight=GetWidth();

		ASSERT(Width>=SCREEN_TILE_WIDTH);
		ASSERT(Height>=SCREEN_TILE_HEIGHT);

		MapShift=Shift;
		TileMapWidth=Width;
		TileMapHeight=Height;

		for (int Buffer=0; Buffer<2; Buffer++)
		{
			sTileMap2dElem	*List=(sTileMap2dElem*) MemAlloc(Size*sizeof(sTileMap2dElem),"2d TileMap");
			TileMap2d[Buffer].List=List;
			TileMap2d[Buffer].MapX=0;
			TileMap2d[Buffer].MapY=0;

			for (int Y=0; Y<Height; Y++)
			{

				for (int X=0; X<Width; X++)
				{
					sTileMapElem	*MapPtr=&Map[X+(Y*MapWidth)];
					sTileMap2dElem	*ThisElem=&List[X+(Y*Width)];
// Tile prim
					TSPRT_16	*ThisTile=&ThisElem->Tile;
					sTile		*SrcTile=&TileList[MapPtr->Tile];
					setTSprt16(ThisTile);
					setTSprtTPage(ThisTile,SrcTile->TPage);
					ThisTile->r0=128;
					ThisTile->g0=128;
					ThisTile->b0=128;
					ThisTile->clut=SrcTile->Clut;
					ThisTile->u0=SrcTile->uv0[0];
					ThisTile->v0=SrcTile->uv0[1];
					if (!MapPtr->Tile)
						ThisTile->clut=0;

// Table			
					int TableR=(X+1) % Width;
					int TableD=(Y+1) % Height;
					ThisElem->Right=&List[TableR+(Y*Width)];
					ThisElem->Down=&List[X+(TableD*Width)];

//					MapPtr++;
				}

			}
		}
		
}

/*****************************************************************************/
void	CLayerTile::shutdown()
{

}

/*****************************************************************************/
int		CLayerTile::CalcTableOfs(int X,int Y)
{
/**/	X%=TileMapWidth;
/**/	Y%=TileMapHeight;

/**/	return(X+(Y*TileMapWidth));

}

/*****************************************************************************/
int		CLayerTile::CalcMapOfs(int X,int Y)
{
int		MapWidth=GetWidth();
int		MapHeight=GetHeight();
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
sTileMap2d	&ThisTable=TileMap2d[FrameFlipFlag];
int			XPos=MapPos.vx>>MapShift;
int			YPos=MapPos.vy>>MapShift;
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
				UpdateRow(NewX,NewY+SCREEN_TILE_HEIGHT-1,ThisTable);
			}
			else
			if (NewY<OldY)
			{	// update top row
				UpdateRow(NewX,NewY,ThisTable);
			}

			ThisTable.MapX=NewX;
			ThisTable.MapY=NewY;

}

/*****************************************************************************/
void	CLayerTile::UpdateRow(int MapX,int MapY,sTileMap2d &ThisTable)
{
sTileMap2dElem	*Table=ThisTable.List;
sTileMapElem	*MapPtr=Map;
//int				MapWidth=GetWidth();

// Calc (wrapped) Map/Table pos
		Table+=CalcTableOfs(MapX,MapY);
		MapPtr+=CalcMapOfs(MapX,MapY);

		for (int i=0; i<SCREEN_TILE_WIDTH-1; i++)
		{
// Tile prim
			TSPRT_16	*ThisTile=&Table->Tile;
			sTile		*SrcTile=&TileList[MapPtr->Tile];

			setTSprtTPage(ThisTile,SrcTile->TPage);
			ThisTile->clut=SrcTile->Clut;
			ThisTile->u0=SrcTile->uv0[0];
			ThisTile->v0=SrcTile->uv0[1];

			MapPtr+=1;
			Table=Table->Right;
		}

}

/*****************************************************************************/
// As column is smaller than row, it takes the corner tiles on, to balance the flow
void	CLayerTile::UpdateColumn(int MapX,int MapY,sTileMap2d &ThisTable)
{
sTileMap2dElem	*Table=ThisTable.List;
sTileMapElem	*MapPtr=Map;
int				MapWidth=GetWidth();

// Calc (wrapped) Map/Table pos
		Table+=CalcTableOfs(MapX,MapY);
		MapPtr+=CalcMapOfs(MapX,MapY);

		for (int i=0; i<SCREEN_TILE_HEIGHT; i++)
		{
// Tile prim
			TSPRT_16	*ThisTile=&Table->Tile;
			sTile		*SrcTile=&TileList[MapPtr->Tile];

			setTSprtTPage(ThisTile,SrcTile->TPage);
			ThisTile->clut=SrcTile->Clut;
			ThisTile->u0=SrcTile->uv0[0];
			ThisTile->v0=SrcTile->uv0[1];

			MapPtr+=MapWidth;
			Table=Table->Down;
		}
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CLayerTile::render()
{
sTileMap2d		&ThisTable=TileMap2d[FrameFlipFlag];
sTileMap2dElem	*Table=ThisTable.List;
u32				XYPos;

// Setup shift bits of pos
		XYPos=ThisTable.ShiftXY;

// Calc (wrapped) Start pos
		Table+=CalcTableOfs(ThisTable.MapX,ThisTable.MapY);

// Render it!!		
		for (int TileY=0; TileY<SCREEN_TILE_HEIGHT; TileY++)
		{
			sTileMap2dElem	*TableDown=Table->Down;
			u32				XYPosDown=XYPos+YInc;
			for (int TileX=0; TileX<SCREEN_TILE_WIDTH; TileX++)
			{
				TSPRT_16	*Tile=&Table->Tile;
				if (Tile->clut)
				{
					*(u32*)&Tile->x0=XYPos;
/**/				AddPrim(OtPtr,Tile);
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
sTileMap2d		&ThisTable=TileMap2d[FrameFlipFlag];
sTileMap2dElem	*Table=ThisTable.List;
u32				XYPos;

// Setup shift bits of pos
		XYPos=ThisTable.ShiftXY;

// Calc (wrapped) Start pos
		Table+=CalcTableOfs(ThisTable.MapX,ThisTable.MapY);

// Render it!!		
		for (int TileY=0; TileY<SCREEN_TILE_HEIGHT; TileY++)
		{
			sTileMap2dElem	*TableDown=Table->Down;
			u32				XYPosDown=XYPos+YInc;
			for (int TileX=0; TileX<SCREEN_TILE_WIDTH; TileX++)
			{
				TSPRT_16	*Tile=&Table->Tile;
				*(u32*)&Tile->x0=XYPos;
/**/			AddPrim(OtPtr,Tile);
				Table=Table->Right;
				XYPos+=XInc;
			}
			Table=TableDown;
			XYPos=XYPosDown;
		}
}

/*****************************************************************************/
