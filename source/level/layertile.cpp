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
// Changed from strip scroll to whole map update (cos of camera)

DVECTOR	TileMapOfs={0,4};	// To line layers up :oP

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
CLayerTile::CLayerTile(sLayerHdr *Hdr,sTile *_TileList,sTri *_TriList,sQuad *_QuadList,sVtx *_VtxList)
{
		LayerHdr=Hdr;
		MapWidth=LayerHdr->Width;
		MapHeight=LayerHdr->Height;

		printf("%i %i\n",MapWidth,MapHeight);
		TileList=_TileList;
		TriList=_TriList;
		QuadList=_QuadList;
		VtxList=_VtxList;
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
			MapXY.vy=YPos>>4;
			
			if (LayerHdr->SubType==1)	// BODGE AND A HALF
			{
/**/			MapXY.vx+=TileMapOfs.vx;
/**/			MapXY.vy+=TileMapOfs.vy;
			}

			ShiftX=XPos & 15;
			ShiftY=YPos & 15;

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
/**/			sTile		*Tile=&TileList[MapRow->Tile];

				if (Tile->Clut)
				{
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
				MapRow++;
				TileX+=TILE_WIDTH;
			}
			MapPtr+=MapWidth;
			TileY+=TILE_HEIGHT;
		}
		SetPrimPtr(PrimPtr);

}

