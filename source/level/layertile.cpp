/************************/
/*** Tile Layer Class ***/
/************************/

#include 	"system\global.h"
#include	<DStructs.h>
#include 	"utils\utils.h"
#include	"system\vid.h"
#include 	"gfx\prim.h"

#include	"LayerTile.h"

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
// NEED TO UPDATE FOR DIFF MAP POS's
void	CLayerTile::InitTileMap2d(int InitX,int InitY,int Width,int Height)
{
int		Size=Width*Height;
int		MapWidth=GetWidth();
int		MapHeight=GetWidth();

		ASSERT(Width>=SCREEN_TILE_WIDTH);
		ASSERT(Height>=SCREEN_TILE_HEIGHT);

		TileMapWidth=Width;
		TileMapHeight=Height;

		for (int Buffer=0; Buffer<2; Buffer++)
		{
			sTileMapElem	*MapPtr=Map;
			sTileMap2dElem	*List=(sTileMap2dElem*) MemAlloc(Size*sizeof(sTileMap2dElem),"2d TileMap");
			TileMap2d[Buffer].List=List;

			for (int Y=0; Y<Height; Y++)
			{
				for (int X=0; X<Width; X++)
				{
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

// Table			
					int TableR=(X+1) % Width;
					int TableD=(Y+1) % Height;
					ThisElem->Right=&List[TableR+(Y*Width)];
					ThisElem->Down=&List[X+(TableD*Width)];

					MapPtr++;
				}

			}
		}
		
}

/*****************************************************************************/
void	CLayerTile::RenderTileMap2d(int MapX,int MapY)
{
sTileMap2dElem	*Table=TileMap2d[FrameFlipFlag].List;
int				XShift,YShift;
u32				XYPos;
const u32		XInc=16<<0;
const u32		YInc=16<<16;

// Setup shift bits of pos
		XShift=15-(MapX&15);
		YShift=15-(MapY&15);
		XYPos=YShift<<16 | XShift<<0;

		MapX>>=4;
		MapY>>=4;
// Calc (wrapped) Start pos
/**/	MapX=MapX % TileMapWidth;
/**/	MapY=MapY % TileMapHeight;

/**/	Table+=MapX;
/**/	Table+=MapY*TileMapWidth;

// Render it!!		
		for (int TileY=0; TileY<SCREEN_TILE_HEIGHT; TileY++)
		{
			sTileMap2dElem	*TableDown=Table->Down;
			u32				XYPosDown=XYPos+YInc;
			for (int TileX=0; TileX<SCREEN_TILE_WIDTH; TileX++)
			{
				TSPRT_16	*TileData=&Table->Tile;
				*(u32*)&TileData->x0=XYPos;
/**/			AddPrim(OtPtr,TileData);
				Table=Table->Right;
				XYPos+=XInc;
			}
			Table=TableDown;
			XYPos=XYPosDown;
		}
}


/*****************************************************************************/
