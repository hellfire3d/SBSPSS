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
//DVECTOR	TileMapOfs={0,4};	// To line layers up :oP

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
CLayerTile::CLayerTile(sLvlHdr *LvlHdr,sLayerHdr *Hdr)
{
		LayerHdr=Hdr;
		MapWidth=LayerHdr->Width;
		MapHeight=LayerHdr->Height;

		printf("%i %i\n",MapWidth,MapHeight);
		TileBank2d=LvlHdr->TileBank2d;
		Map=(sTileMapElem*)MakePtr(Hdr,sizeof(sLayerHdr));
}

/*****************************************************************************/
CLayerTile::~CLayerTile()
{
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
			MapXY.vy=YPos/12;

/*			if (LayerHdr->SubType==1)	// BODGE AND A HALF
			{
				MapXY.vx+=TileMapOfs.vx; MapXY.vy+=TileMapOfs.vy;
			}
*/
			ShiftX=XPos & 15;
//			ShiftY=YPos & 15;
			ShiftY=YPos%12;

			if (MapXY.vx+SCREEN_TILE_WIDTH<=MapWidth)
				RenderW=SCREEN_TILE_WIDTH;
			else
				RenderW=MapWidth-MapXY.vx;

			if (MapXY.vy+SCREEN_TILE_HEIGHT<=MapHeight)
				RenderH=SCREEN_TILE_HEIGHT;
			else
				RenderH=MapHeight-MapXY.vy;

}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#if	1	// FT4 version
void	CLayerTile::render()
{

sTileMapElem	*MapPtr=GetMapPos();
u8				*PrimPtr=GetPrimPtr();
s16				TileX,TileY;
sOT				*ThisOT=OtPtr+LayerOT;

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
					sTile2d		*Tile=&TileBank2d[ThisTile];
					POLY_FT4	*Ft4=(POLY_FT4*)PrimPtr;
					setPolyFT4(Ft4);
					setShadeTex(Ft4,1);
					setXYWH(Ft4,TileX,TileY,16,12);
					setUVWH(Ft4,Tile->u0,Tile->v0,15,15);
					Ft4->tpage=Tile->TPage;
					Ft4->clut=Tile->Clut;
//					addPrimNoCheck(ThisOT,Ft4);
					addPrim(ThisOT,Ft4);
					PrimPtr+=sizeof(POLY_FT4);
				}
				TileX+=TILE_WIDTH;
			}
			MapPtr+=MapWidth;
			TileY+=TILE_HEIGHT;
		}
		SetPrimPtr(PrimPtr);

}

#else
void	CLayerTile::render()
{
sTileMapElem	*MapPtr=GetMapPos();
u8				*PrimPtr=GetPrimPtr();
s16				TileX,TileY;
sOT				*ThisOT=OtPtr+LayerOT;

// Setup shift bits of pos
		TileY=-ShiftY;

// Render it!!
		for (int Y=0; Y<RenderH; Y++)
		{
			sTileMapElem	*MapRow=MapPtr;
			TileX=-ShiftX;

			for (int X=0; X<RenderW; X++)
			{
				int	ThisTile=*MapRow++;
				if (ThisTile)
				{
/**/				sTile2d		*Tile=&TileBank2d[ThisTile];
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
//				MapRow++;
				TileX+=TILE_WIDTH;
			}
			MapPtr+=MapWidth;
			TileY+=TILE_HEIGHT;
		}
		SetPrimPtr(PrimPtr);

}


#endif