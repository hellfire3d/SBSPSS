/*************/
/*** Level ***/
/*************/

#include 	"system\global.h"
#include 	"fileio\fileio.h"
#include 	"utils\utils.h"
#include	"gfx\tpage.h"
#include	"gfx\prim.h"
#include	<DStructs.h>
#include	"game\game.h"

#include	"level\level.h"
#include	"level\layertile.h"
#include	"level\layerback.h"
#include	"level\layertilesolid.h"
#include	"level\layertile3d.h"
#include	"level\layercollision.h"

#include	"pad\pads.h"

DVECTOR CLevel::MapPos;

/*****************************************************************************/
void 	CLevel::init()
{
		for (int i=0; i<CLayerTile::LAYER_TILE_TYPE_MAX; i++)
		{
			TileLayers[i]=0;
		}
		MapPos.vx=0;		
		MapPos.vy=0;

		TileBankHdr=(sTileBankHdr *)CFileIO::loadFile(LEVELS_CHAPTER02_LEVEL04_TBK,"Tile Bank Data");
		LevelHdr=(sLvlHdr *)CFileIO::loadFile(LEVELS_CHAPTER02_LEVEL0401_LVL,"Level Data");
		m_levelTPage=TPLoadTex(LEVELS_CHAPTER02_LEVEL04_TEX);

		initLayers();
}

/*****************************************************************************/
void	CLevel::initLayers()
{
sTri	*TriList=(sTri*)MakePtr(TileBankHdr,TileBankHdr->TriList);
sQuad	*QuadList=(sQuad*)MakePtr(TileBankHdr,TileBankHdr->QuadList);
sVtx	*VtxList=(sVtx*)MakePtr(TileBankHdr,TileBankHdr->VtxList);
sTile	*TileList=(sTile*)MakePtr(TileBankHdr,TileBankHdr->TileList);

// Back
		if (LevelHdr->BackLayer) 
		{
			sLayerHdr	*Layer=(sLayerHdr*)MakePtr(LevelHdr,LevelHdr->BackLayer);
//			CLayerTile *NewLayer=new ("Back Layer") CLayerTileSolid(Layer,  TileList, TriList, QuadList, VtxList);
			CLayerTile *NewLayer=new ("Back Layer") CLayerBack(Layer,  TileList, TriList, QuadList, VtxList);
			NewLayer->init(MapPos,3);
			TileLayers[CLayerTile::LAYER_TILE_TYPE_BACK]=NewLayer;
		}

// Mid
		if (LevelHdr->MidLayer) 
		{
			sLayerHdr	*Layer=(sLayerHdr*)MakePtr(LevelHdr,LevelHdr->MidLayer);
			CLayerTile *NewLayer=new ("Mid Layer") CLayerTile(Layer, TileList, TriList, QuadList, VtxList);
			NewLayer->init(MapPos,2);
			TileLayers[CLayerTile::LAYER_TILE_TYPE_MID]=NewLayer;
		}

// Action
		if (LevelHdr->ActionLayer) 
		{
			sLayerHdr	*Layer=(sLayerHdr*)MakePtr(LevelHdr,LevelHdr->ActionLayer);
			CLayerTile *NewLayer=new ("Action Layer") CLayerTile3d(Layer, TileList, TriList, QuadList, VtxList);
			NewLayer->init(MapPos,0);
			TileLayers[CLayerTile::LAYER_TILE_TYPE_ACTION]=NewLayer;
		}

/*
// Fore
		if (LevelHdr->ForeLayer) 
		{
			sLayerHdr	*Layer=(sLayerHdr*)MakePtr(LevelHdr,LevelHdr->ForeLayer);
			CLayerTile *NewLayer=new ("Fore Layer") CLayerTile(Layer, TileList, TriList, QuadList, VtxList);
			NewLayer->init();
			TileLayers[CLayerTile::LAYER_TILE_TYPE_FORE]=NewLayer;
		}
*/
// Collision
		if (LevelHdr->CollisionLayer) 
		{
			sLayerHdr	*Layer=(sLayerHdr*)MakePtr(LevelHdr,LevelHdr->CollisionLayer);
			CollisionLayer=new ("Collision Layer") CLayerCollision(Layer);
		}

}

/*****************************************************************************/
void	CLevel::shutdown()
{
		for (int i=0; i<CLayerTile::LAYER_TILE_TYPE_MAX; i++)
		{
			if (TileLayers[i])
			{
				TileLayers[i]->shutdown();
				delete TileLayers[i];
			}
		}

		TPFree(m_levelTPage);
		CollisionLayer->shutdown();	MemFree(CollisionLayer);
		MemFree(TileBankHdr);
		MemFree(LevelHdr);
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void 	CLevel::render()
{
// Setup Constant Rot Matrix
MATRIX	*Mtx=CGameScene::GetCamMtx();

		SetRotMatrix(Mtx);
		SetTransMatrix(Mtx);

// Setup dummy prim to ensure OtPos 0 is initialised (for fast add)
TILE_16	*Prim=GetPrimTILE16();
sOT		*ThisOT=OtPtr+LayerOT;
		Prim->x0=1024;
		Prim->y0=1024;
		AddPrim(ThisOT,Prim);
		ASSERT(ThisOT->FirstPrim);

		for (int i=0; i<CLayerTile::LAYER_TILE_TYPE_MAX; i++)
		{
			if (TileLayers[i]) TileLayers[i]->render();
		}
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CLevel::think(int _frames)
{
		for (int i=0; i<CLayerTile::LAYER_TILE_TYPE_MAX; i++)
		{
			if (TileLayers[i]) TileLayers[i]->think(MapPos);
		}
}



/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
DVECTOR	CLevel::getMapSize()
{
	DVECTOR		size;
	sLayerHdr	*layer;

	layer=(sLayerHdr*)MakePtr(LevelHdr,LevelHdr->ActionLayer);
	size.vx=layer->Width;
	size.vy=layer->Height;

	return size;
}


/*****************************************************************************/
