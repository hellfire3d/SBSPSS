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
static const int	TILE2D_WIDTH=16;
static const int	TILE2D_HEIGHT=12;
static const int	SCREEN_TILE2D_WIDTH=((512/TILE2D_WIDTH)+1);
static const int	SCREEN_TILE2D_HEIGHT=((256/TILE2D_HEIGHT)+1);

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/

CLayerTile::CLayerTile(sLevelHdr *LevelHdr,sLayerHdr *Hdr)
{
int		Count=SCREEN_TILE2D_WIDTH*SCREEN_TILE2D_HEIGHT;
int		MemSize=Count*sizeof(TSPRT);

		LayerHdr=Hdr;
		MapWidth=LayerHdr->Width;
		MapHeight=LayerHdr->Height;

		ElemBank2d=LevelHdr->ElemBank2d;
		Map=(sTileMapElem*)MakePtr(Hdr,sizeof(sLayerHdr));
		
		PrimBank=(TSPRT*)MemAlloc(MemSize,"Mid Polyz");

TSPRT	*PrimPtr=PrimBank;
		for (int i=0; i<Count; i++)
		{
			setTSprt(PrimPtr);
			setTSetShadeTex(PrimPtr,1);
			PrimPtr->w=TILE2D_WIDTH;
			PrimPtr->h=TILE2D_HEIGHT;
			PrimPtr++;
		}
}

/*****************************************************************************/
CLayerTile::~CLayerTile()
{
		MemFree(PrimBank);
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CLayerTile::init(DVECTOR &MapPos,int Shift)
{
		MapXYShift=Shift;
		MapXY=MapPos;
}

/*****************************************************************************/
void	CLayerTile::shutdown()
{
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CLayerTile::think(DVECTOR &MapPos)
{
int			XPos=MapPos.vx>>MapXYShift;
int			YPos=MapPos.vy>>MapXYShift;

			MapXY.vx=XPos>>4;
			MapXY.vy=YPos/TILE2D_HEIGHT;

			ShiftX=XPos & 15;
			ShiftY=YPos%TILE2D_HEIGHT;

			if (MapXY.vx<0)
			{
				MapXY.vx=0;
				ShiftX=0;
			}
			if (MapXY.vy<0) 
			{
				MapXY.vy=0;
				ShiftY=0;
			}

			if (MapXY.vx+SCREEN_TILE2D_WIDTH<=MapWidth)
				RenderW=SCREEN_TILE2D_WIDTH;
			else
				RenderW=MapWidth-MapXY.vx;

			if (MapXY.vy+SCREEN_TILE2D_HEIGHT<=MapHeight)
				RenderH=SCREEN_TILE2D_HEIGHT;
			else
				RenderH=MapHeight-MapXY.vy;

}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CLayerTile::render()
{
sTileMapElem	*MapPtr=Map+GetMapOfs();
s16				TileX,TileY;
sOT				*ThisOT=OtPtr+LayerOT;
TSPRT			*PrimPtr=PrimBank;

// Setup shift bits of pos
		TileY=-ShiftY;

// Render it!!

		for (int Y=0; Y<RenderH; Y++)
		{
			sTileMapElem	*MapRow=MapPtr;
			TileX=-ShiftX;

			for (int X=0; X<RenderW; X++)
			{
				int	ThisTile=MapRow->Tile;
				MapRow++;
				if (ThisTile)
				{
					sElem2d		*Tile=&ElemBank2d[ThisTile];
					PrimPtr->x0=TileX;
					PrimPtr->y0=TileY;
					setTSprtTPage(PrimPtr,Tile->TPage);
					*(u32*)&PrimPtr->u0=*(u32*)&Tile->u0;	// copy uv AND clut
					addPrim(ThisOT,PrimPtr);
					PrimPtr++;
				}
				TileX+=TILE2D_WIDTH;
			}
			MapPtr+=MapWidth;
			TileY+=TILE2D_HEIGHT;
		}

}
