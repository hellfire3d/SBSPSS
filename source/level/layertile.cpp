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
void	CLayerTile::init(DVECTOR &MapPos,int Shift,int Width,int Height)
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
// Grid			
				ThisElem->Right=GetGridPos(X+1,Y);
				ThisElem->Down=GetGridPos(X,Y+1);
			}
		}
		UpdateWholeMap();		
}

/*****************************************************************************/
// AS not time critical, use row update routine for whole map
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
sPrimGridElem	*ThisGrid=(sPrimGridElem *)PrimGrid;
int		Pos;

/**/	X%=PrimGridWidth;
/**/	Y%=PrimGridHeight;
/**/	Pos=(X+(Y*PrimGridWidth));

/**/	return(ThisGrid+Pos);
}

/*****************************************************************************/
// Get (wrapped) Map pos
sTileMapElem	*CLayerTile::GetMapPos(int X,int Y)
{
sTileMapElem	*ThisMap=(sTileMapElem *)Map;
int		Pos;

/**/	X%=MapWidth;
/**/	Y%=MapHeight;
/**/	Pos=(X+(Y*MapWidth));
	
/**/	return(ThisMap+Pos);

}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CLayerTile::think(DVECTOR &MapPos)
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
sPrimGridElem	*Grid=GetGridPos(X,Y);
sTileMapElem	*MapPtr=GetMapPos(X,Y);

		for (int i=0; i<SCREEN_TILE_WIDTH; i++)
		{
// Tile prim
			TSPRT_16	*Prim=&Grid->Prim;
/**/		sTile		*Tile=&TileList[MapPtr->Tile];
/**/		setTSprtTPage(Prim,Tile->TPage);
			*(u32*)&Prim->u0=*(u32*)&Tile->u0;	// copy uv AND clut
// Next Elem
			MapPtr++;
			Grid=Grid->Right;
		}

}

/*****************************************************************************/
void	CLayerTile::UpdateColumn(int X,int Y)
{
sPrimGridElem	*Grid=GetGridPos(X,Y);
sTileMapElem	*MapPtr=GetMapPos(X,Y);

		for (int i=0; i<SCREEN_TILE_HEIGHT; i++)
		{
// Tile prim
			TSPRT_16	*Prim=&Grid->Prim;
/**/		sTile		*Tile=&TileList[MapPtr->Tile];
/**/		setTSprtTPage(Prim,Tile->TPage);
			*(u32*)&Prim->u0=*(u32*)&Tile->u0;	// copy uv AND clut
// Next Elem
			MapPtr+=MapWidth;
			Grid=Grid->Down;
		}
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CLayerTile::render()
{
sPrimGridElem	*Grid=GetGridPos(MapX,MapY);
s16				TileX,TileY;
sOT				*ThisOT=OtPtr+LayerOT;
// Setup shift bits of pos
		TileY=-ShiftY;

// Render it!!
		for (int Y=0; Y<SCREEN_TILE_HEIGHT; Y++)
		{
			sPrimGridElem	*GridDown=Grid->Down;
			TileX=-ShiftX;

			for (int X=0; X<SCREEN_TILE_WIDTH; X++)
			{
				TSPRT_16	*Prim=&Grid->Prim;
				if (Prim->clut)
				{
/**/				Prim->x0=TileX;
/**/				Prim->y0=TileY;
					addPrimNoCheck(ThisOT,Prim);
				}
				Grid=Grid->Right;
				TileX+=TILE_WIDTH;
			}
			Grid=GridDown;
			TileY+=TILE_HEIGHT;
		}
}

