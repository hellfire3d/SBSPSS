/******************************/
/*** Solid Tile Layer Class ***/
/******************************/

#include 	"system\global.h"
#include	<DStructs.h>
#include 	"utils\utils.h"
#include 	"gfx\prim.h"


#include	"LayerTile.h"
#include	"LayerTileSolid.h"


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
CLayerTileSolid::CLayerTileSolid(sLayerHdr *Hdr,sTile *TileList,sTri *TriList,sQuad *QuadList,sVtx *VtxList) : CLayerTile(Hdr,TileList,TriList,QuadList,VtxList)
{
}

/*****************************************************************************/
CLayerTileSolid::~CLayerTileSolid()
{
}


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CLayerTileSolid::init(DVECTOR &MapPos,int Shift,int Width,int Height)
{
		CLayerTile::init(MapPos,Shift,MapWidth,MapHeight);
}

/*****************************************************************************/
void	CLayerTileSolid::shutdown()
{
}

/*****************************************************************************/
void	CLayerTileSolid::think(DVECTOR &MapPos)
{ // Overide default strip scroll update
int			XPos=MapPos.vx>>MapXYShift;
int			YPos=MapPos.vy>>MapXYShift;

			ShiftX=XPos&15;
			ShiftY=YPos&15;

			MapX=XPos>>4;
			MapY=YPos>>4;
}

/*****************************************************************************/
void	CLayerTileSolid::render()
{
sPrimGridElem	*Grid=GetGridPos(MapX,MapY);
s16				TileX,TileY;
sOT				*ThisOT=OtPtr+LayerOT;

// Setup shift bits of pos
		TileY=-ShiftY;

// Render it!!
		for (int Y=0; Y<SCREEN_TILE_HEIGHT; Y++)
		{
			sPrimGridElem	*GridDown= Grid->Down;
			TileX=-ShiftX;

			for (int X=0; X<SCREEN_TILE_WIDTH; X++)
			{
				TSPRT_16	*Prim=&Grid->Prim;
/**/			Prim->x0=TileX;
/**/			Prim->y0=TileY;
				addPrimNoCheck(ThisOT,Prim);
				Grid=Grid->Right;
				TileX+=TILE_WIDTH;
			}
			Grid=GridDown;
			TileY+=TILE_HEIGHT;
		}
}



