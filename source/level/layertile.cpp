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

	DAVE_DBGMSG("LayerTile\n");

}

/*****************************************************************************/
CLayerTile::~CLayerTile()
{
}

/*****************************************************************************/
int		CLayerTile::GetLayerType(int SubType)
{
	switch(SubType)
	{
		case CLayerTile::LAYER_TILE_SUBTYPE_BACK:	return(LAYER_TYPE_TILE_BACK);
		case CLayerTile::LAYER_TILE_SUBTYPE_MID:	return(LAYER_TYPE_TILE_MID);
		case CLayerTile::LAYER_TILE_SUBTYPE_ACTION: return(LAYER_TYPE_TILE_ACTION);
		case CLayerTile::LAYER_TILE_SUBTYPE_FORE:	return(LAYER_TYPE_TILE_FORE);
	}
	return(-1);
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CLayerTile::init()
{
	return;
int	Width=GetWidth();
int	Height=GetHeight();
int	Size=Width*Height;

	for (int i=0; i<Size; i++)
		Map[i].Tile =0;
/*
	for (int Y=0; Y<4; Y++)
	{
		for (int X=0; X<4; X++)
		{
			Map[X+(Y*Width)].Tile=X+(Y*4);
		}

	}
*/
	for (int i=0; i<16; i++) Map[i].Tile=i;
}

/*****************************************************************************/
void	CLayerTile::shutdown()
{
}

/*****************************************************************************/
int		XOfs=64;
int		YOfs=64;
int		Dx=0;
int		Dy=0;
void	CLayerTile::render()
{
int			Width=GetWidth();
int			Height=GetHeight();
POLY_FT4	*ft4;
sTileMapElem	*ThisElem=Map;
		XOfs+=Dx;
		YOfs+=Dy;
		for (int Y=0; Y<Height; Y++)
		{
			for (int X=0; X<Width; X++)
			{
			
				if (ThisElem->Tile)
				{
					sTile		*ThisTile=&TileList[ThisElem->Tile];
					if (ThisTile->TPage)
					{
						ft4=GetPrimFT4();
						setShadeTex(ft4,1);
						setSemiTrans(ft4,1);
						ft4->tpage=ThisTile->TPage;
						ft4->clut=ThisTile->Clut;
						ft4->x0=(X*TILE_WIDTH)+XOfs; ft4->y0=(Y*TILE_HEIGHT)+YOfs;
						ft4->x1=ft4->x0+TILE_WIDTH;	 ft4->y1=ft4->y0;
						ft4->x2=ft4->x0;			 ft4->y2=ft4->y0+TILE_HEIGHT;
						ft4->x3=ft4->x1;			 ft4->y3=ft4->y2;
						*(u16*)&ft4->u0=*(u16*)ThisTile->uv0;
						*(u16*)&ft4->u1=*(u16*)ThisTile->uv1;
						*(u16*)&ft4->u2=*(u16*)ThisTile->uv2;
						*(u16*)&ft4->u3=*(u16*)ThisTile->uv3;
						setRGB0(ft4,255,255,255);
						AddPrimToList(ft4,0);
					}
					else
					{
					}
				}
			ThisElem++;
			}
		}
}
/*
void	CLayerTile::render()
{
int			ListSize=186;
sTri		*Tri=TriList;
POLY_FT3	*ft3;
POLY_FT4	*ft4;
sTile		*ThisTile=TileList;

int		X,Y;
		X=XOfs;
		Y=YOfs;
		for (int i=0; i<ListSize; i++)
		{
			if (ThisTile->TriCount)
			{
			}
			ThisTile++;
			X+=16;
			if (X>(512-64))
			{
				X=XOfs;
				Y+=12;
			}
		}
}

*/
/*****************************************************************************/
void	CLayerTile::think(int _frames)
{
}

