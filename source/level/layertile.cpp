/************************/
/*** Tile Layer Class ***/
/************************/

#include 	"system\global.h"
#include	<DStructs.h>
#include 	"utils\utils.h"
#include 	"gfx\prim.h"

#include	"Layer.h"
#include	"LayerTile.h"

enum
{
	TILE_WIDTH=16,
	TILE_HEIGHT=16,
};

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
CLayerTile::CLayerTile(sLayerHdr *Hdr,sTile *_TileList,sTri *_TriList,sQuad *_QuadList,sVtx *_VtxList) : CLayer(Hdr)
{
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
void	CLayerTile::init()
{
}

/*****************************************************************************/
void	CLayerTile::shutdown()
{
}

/*****************************************************************************/
int		MapX=0;
int		MapY=0;


void	CLayerTile::render()
{
int				Width=32;//GetWidth();
int				Height=21;//GetHeight();
POLY_FT4		*ft4;
sTileMapElem	*ThisElem=Map;
int				XOfs=MapX;
int				YOfs=MapY;
int				XTileOfs,YTileOfs;
		

		if (LayerHdr->SubType==LAYER_TILE_SUBTYPE_BACK)
		{
			XOfs/=4;
			YOfs/=4;
		}
		if (LayerHdr->SubType==LAYER_TILE_SUBTYPE_MID)
		{
			XOfs/=2;
			YOfs/=2;
		}

		YTileOfs=YOfs;
		for (int Y=0; Y<Height; Y++)
		{
			XTileOfs=XOfs;
			for (int X=0; X<Width; X++)
			{
				if (ThisElem->Tile)
				{
					sTile		*ThisTile=&TileList[ThisElem->Tile];
					if (ThisTile->TPage)
					{
						s16		x0=XTileOfs;
						s16		x1=XTileOfs+TILE_WIDTH;
						s16		y0=YTileOfs;
						s16		y1=YTileOfs+TILE_HEIGHT;

						ft4=GetPrimFT4();
						setShadeTex(ft4,1);
						ft4->tpage=ThisTile->TPage;
						ft4->clut=ThisTile->Clut;
						ft4->x0=x0	; ft4->y0=y0;
						ft4->x1=x1	; ft4->y1=y0;
						ft4->x2=x0	; ft4->y2=y1;
						ft4->x3=x1	; ft4->y3=y1;
						*(u16*)&ft4->u0=*(u16*)ThisTile->uv0;
						*(u16*)&ft4->u1=*(u16*)ThisTile->uv1;
						*(u16*)&ft4->u2=*(u16*)ThisTile->uv2;
						*(u16*)&ft4->u3=*(u16*)ThisTile->uv3;
						AddPrimToList(ft4,0);
					}
					else
					{
					}
				}
				ThisElem++;
				XTileOfs+=TILE_WIDTH;
			}
			YTileOfs+=TILE_HEIGHT;
		}
}

/*****************************************************************************/
void	CLayerTile::think(int _frames)
{
}

