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
#include	"level\layer.h"
#include	"level\layertile.h"

#include	"pad\pads.h"

/*****************************************************************************/
CLevel::CLevel()
{
		for (int i=0; i<CLayer::LAYER_TYPE_MAX; i++)
		{
			TileLayers[i]=0;
		}
		DAVE_DBGMSG("sizeof(POLY_FT4)=%i\n",sizeof(POLY_FT4));
		DAVE_DBGMSG("sizeof(SPRT)=%i\n",sizeof(SPRT));
		
}

/*****************************************************************************/
CLevel::~CLevel()
{
		for (int i=0; i<CLayer::LAYER_TYPE_MAX; i++)
		{
			if (TileLayers[i]) delete TileLayers[i];
		}
}

/*****************************************************************************/
void 	CLevel::init()
{
		LevelHdr=(sLvlHdr *)CFileIO::loadFile(CHAPTER01_LEVEL01_LVL,"Level Data");
		TPLoadTex(CHAPTER01_LEVEL01_TEX);

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
			CLayer *NewLayer=new ("Back Layer") CLayerTile(Layer, TileList, TriList, QuadList, VtxList);
			NewLayer->init();
			TileLayers[CLayer::LAYER_TYPE_TILE_BACK]=NewLayer;
		}
// Mid
		if (LevelHdr->MidLayer) 
		{
			sLayerHdr	*Layer=(sLayerHdr*)MakePtr(LevelHdr,LevelHdr->MidLayer);
			CLayer *NewLayer=new ("Mid Layer") CLayerTile(Layer, TileList, TriList, QuadList, VtxList);
			NewLayer->init();
			TileLayers[CLayer::LAYER_TYPE_TILE_MID]=NewLayer;
		}
// Action
		if (LevelHdr->ActionLayer) 
		{
			sLayerHdr	*Layer=(sLayerHdr*)MakePtr(LevelHdr,LevelHdr->ActionLayer);
			CLayer *NewLayer=new ("Action Layer") CLayerTile(Layer, TileList, TriList, QuadList, VtxList);
			NewLayer->init();
			TileLayers[CLayer::LAYER_TYPE_TILE_ACTION]=NewLayer;
		}
// Fore
		if (LevelHdr->ForeLayer) 
		{
			sLayerHdr	*Layer=(sLayerHdr*)MakePtr(LevelHdr,LevelHdr->ForeLayer);
			CLayer *NewLayer=new ("Fore Layer") CLayerTile(Layer, TileList, TriList, QuadList, VtxList);
			NewLayer->init();
			TileLayers[CLayer::LAYER_TYPE_TILE_FORE]=NewLayer;
		}
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
		for (int i=0; i<CLayer::LAYER_TYPE_MAX; i++)
		{
			if (TileLayers[i]) TileLayers[i]->render();
		}
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
extern int	MapX;
extern int	MapY;

void	CLevel::think(int _frames)
{
int		padh = PadGetHeld( 0 );

		if (padh & PAD_LEFT)	MapX+=4;
		if (padh & PAD_RIGHT)	MapX-=4;
		if (padh & PAD_UP)		MapY+=4;
		if (padh & PAD_DOWN)	MapY-=4;

		for (int i=0; i<CLayer::LAYER_TYPE_MAX; i++)
		{
			if (TileLayers[i]) TileLayers[i]->think(_frames);
		}
}


/*****************************************************************************/
