/*************/
/*** Level ***/
/*************/

#include 	"system\global.h"
#include 	"fileio\fileio.h"
#include 	"utils\utils.h"
#include	"gfx\tpage.h"
#include	"gfx\prim.h"
#include	<DStructs.h>

#include	"level\level.h"
#include	"level\layertile.h"
#include	"level\layerback.h"

#include	"pad\pads.h"

/*****************************************************************************/
CLevel::CLevel()
{
		for (int i=0; i<CLayerTile::LAYER_TILE_TYPE_MAX; i++)
		{
			TileLayers[i]=0;
		}
		DAVE_DBGMSG("sizeof(POLY_FT4)=%i",sizeof(POLY_FT4));
		DAVE_DBGMSG("sizeof(TSPRT)=%i",sizeof(TSPRT));
		
}

/*****************************************************************************/
CLevel::~CLevel()
{
		for (int i=0; i<CLayerTile::LAYER_TILE_TYPE_MAX; i++)
		{
			if (TileLayers[i]) delete TileLayers[i];
		}
}

/*****************************************************************************/
void 	CLevel::init()
{
//		LevelHdr=(sLvlHdr *)CFileIO::loadFile(CHAPTER01_LEVEL01_LVL,"Level Data");
//		TPLoadTex(CHAPTER01_LEVEL01_TEX);
		LevelHdr=(sLvlHdr *)CFileIO::loadFile(LEVEL01_LEVEL01_LVL,"Level Data");
		TPLoadTex(LEVEL01_LEVEL01_TEX);

		initLayers();
}

/*****************************************************************************/
void	CLevel::initLayers()
{
sTri	*TriList=(sTri*)MakePtr(LevelHdr,LevelHdr->TriList);
sQuad	*QuadList=(sQuad*)MakePtr(LevelHdr,LevelHdr->QuadList);
sVtx	*VtxList=(sVtx*)MakePtr(LevelHdr,LevelHdr->VtxList);
sTile	*TileList=(sTile*)MakePtr(LevelHdr,LevelHdr->TileList);

// Back
		if (LevelHdr->BackLayer) 
		{
			sLayerHdr	*Layer=(sLayerHdr*)MakePtr(LevelHdr,LevelHdr->BackLayer);
			CLayerTile *NewLayer=new ("Back Layer") CLayerBack(Layer, TileList);
			NewLayer->init();
			TileLayers[CLayerTile::LAYER_TILE_TYPE_BACK]=NewLayer;
		}
/*
// Mid
		if (LevelHdr->MidLayer) 
		{
			sLayerHdr	*Layer=(sLayerHdr*)MakePtr(LevelHdr,LevelHdr->MidLayer);
			CLayerTile *NewLayer=new ("Mid Layer") CLayerTile(Layer, TileList, TriList, QuadList, VtxList);
			NewLayer->init();
			TileLayers[CLayerTile::LAYER_TILE_TYPE_MID]=NewLayer;
		}
// Action
		if (LevelHdr->ActionLayer) 
		{
			sLayerHdr	*Layer=(sLayerHdr*)MakePtr(LevelHdr,LevelHdr->ActionLayer);
			CLayerTile *NewLayer=new ("Action Layer") CLayerTile(Layer, TileList, TriList, QuadList, VtxList);
			NewLayer->init();
			TileLayers[CLayerTile::LAYER_TILE_TYPE_ACTION]=NewLayer;
		}
// Fore
		if (LevelHdr->ForeLayer) 
		{
			sLayerHdr	*Layer=(sLayerHdr*)MakePtr(LevelHdr,LevelHdr->ForeLayer);
			CLayerTile *NewLayer=new ("Fore Layer") CLayerTile(Layer, TileList, TriList, QuadList, VtxList);
			NewLayer->init();
			TileLayers[CLayerTile::LAYER_TILE_TYPE_FORE]=NewLayer;
		}
*/
}

/*****************************************************************************/
void	CLevel::shutdown()
{
		MemFree(LevelHdr);
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void 	CLevel::render()
{
		for (int i=0; i<CLayerTile::LAYER_TILE_TYPE_MAX; i++)
		{
			if (TileLayers[i]) TileLayers[i]->render();
		}
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
extern int	MapX;
extern int	MapY;
int	MapSpd=8;
void	CLevel::think(int _frames)
{
int		padh = PadGetHeld( 0 );

		if (padh & PAD_LEFT)	MapX-=MapSpd;
		if (padh & PAD_RIGHT)	MapX+=MapSpd;
		if (padh & PAD_UP)		MapY-=MapSpd;
		if (padh & PAD_DOWN)	MapY+=MapSpd;

		if (MapX<0) MapX=0;
		if (MapY<0) MapY=0;

		for (int i=0; i<CLayerTile::LAYER_TILE_TYPE_MAX; i++)
		{
			if (TileLayers[i]) TileLayers[i]->think(_frames);
		}
}


/*****************************************************************************/
