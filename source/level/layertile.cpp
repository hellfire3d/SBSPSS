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
#if defined(__TERRITORY_EUR__)
static const int	SCREEN_TILE_ADJ_W		=1;
static const int	SCREEN_TILE_ADJ_H		=2; 	// Extra line needed :o(
#else
static const int	SCREEN_TILE_ADJ_W		=1;
static const int	SCREEN_TILE_ADJ_H		=1;
#endif
static const int	SCREEN_TILE2D_WIDTH=((512/TILE2D_WIDTH)+SCREEN_TILE_ADJ_W);
static const int	SCREEN_TILE2D_HEIGHT=((256/TILE2D_HEIGHT)+SCREEN_TILE_ADJ_H);
static const int	PrimCount=SCREEN_TILE2D_WIDTH*SCREEN_TILE2D_HEIGHT;
static const int	PrimMemSize=PrimCount*sizeof(TSPRT);

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/

CLayerTile::CLayerTile(sLevelHdr *LevelHdr,sLayerHdr *Hdr)
{
		LayerHdr=Hdr;
		MapWidth=LayerHdr->Width;
		MapHeight=LayerHdr->Height;

		ElemBank2d=LevelHdr->ElemBank2d;
		Map=(sTileMapElem*)MakePtr(Hdr,sizeof(sLayerHdr));
		
		PrimBankID=0;
		PrimBank[0]=0; PrimBank[1]=0;

		if (Hdr->SubType!=LAYER_TILE_TYPE_MID) return;
//-----------------
// anything below here is Mid layer only - how did I miss something like this, 104k WASTED!!

// Create Mid Tile Prim Banks
		for (int b=0; b<2; b++)
		{
			PrimBank[b]=(TSPRT*)MemAlloc(PrimMemSize,"Mid Polyz");
			TSPRT *PrimPtr=PrimBank[b];
			for (int i=0; i<PrimCount; i++)
			{
				setTSprt(PrimPtr);
				setTShadeTex(PrimPtr,1);
				PrimPtr->w=TILE2D_WIDTH;
				PrimPtr->h=TILE2D_HEIGHT;
				PrimPtr++;
			}
		}

// precalc Mid tile offsets
sTileMapElem	*MapPtr=Map;
		for (int Y=0; Y<MapHeight; Y++)
		{
			for (int X=0; X<MapWidth; X++)
			{
				MapPtr->Tile*=sizeof(sElem2d);
				MapPtr++;
			}
		}
}

/*****************************************************************************/
CLayerTile::~CLayerTile()
{
		for (int b=0; b<2; b++)
		{
			if (PrimBank[b]) MemFree(PrimBank[b]);
		}
		
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
TSPRT			*PrimPtr=PrimBank[PrimBankID];
u8				*TileBank=(u8*)ElemBank2d;
u32				T0,T1;

			PrimBankID^=1;

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
				if (ThisTile)
				{
					sElem2d		*Tile=(sElem2d*)(TileBank+ThisTile);
					PrimPtr->x0=TileX;
					PrimPtr->y0=TileY;
					T0=Tile->TPage;
					T1=*(u32*)&Tile->u0;
					PrimPtr->t_code=T0;		//	pregen'd setTSprtTPage(PrimPtr,Tile->TPage);
					*(u32*)&PrimPtr->u0=T1;	// copy uv AND clut
					addPrim(ThisOT,PrimPtr);
					PrimPtr++;
				}
				MapRow++;
				TileX+=TILE2D_WIDTH;
			}
			MapPtr+=MapWidth;
			TileY+=TILE2D_HEIGHT;
		}

}
