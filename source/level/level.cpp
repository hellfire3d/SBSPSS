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
#include	"level\layertile3d.h"
#include	"level\layercollision.h"

#include	"pad\pads.h"

DVECTOR CLevel::MapPos;

struct	sLvlTab
{
	FileEquate	TileBank,Level,Tex;
};

sLvlTab	LvlTable[]=
{
// C3 L3 - 6 parts
	{LEVELS_CHAPTER03_LEVEL03_TBK,LEVELS_CHAPTER03_LEVEL0301_LVL,LEVELS_CHAPTER03_LEVEL03_TEX},	//	0
	{LEVELS_CHAPTER03_LEVEL03_TBK,LEVELS_CHAPTER03_LEVEL0302_LVL,LEVELS_CHAPTER03_LEVEL03_TEX},	//	1
	{LEVELS_CHAPTER03_LEVEL03_TBK,LEVELS_CHAPTER03_LEVEL0303_LVL,LEVELS_CHAPTER03_LEVEL03_TEX},	//	2
	{LEVELS_CHAPTER03_LEVEL03_TBK,LEVELS_CHAPTER03_LEVEL0304_LVL,LEVELS_CHAPTER03_LEVEL03_TEX},	//	3
	{LEVELS_CHAPTER03_LEVEL03_TBK,LEVELS_CHAPTER03_LEVEL0305_LVL,LEVELS_CHAPTER03_LEVEL03_TEX},	//	4
	{LEVELS_CHAPTER03_LEVEL03_TBK,LEVELS_CHAPTER03_LEVEL0306_LVL,LEVELS_CHAPTER03_LEVEL03_TEX},	//	5
// C3 L4
	{LEVELS_CHAPTER03_LEVEL04_TBK,LEVELS_CHAPTER03_LEVEL0401_LVL,LEVELS_CHAPTER03_LEVEL04_TEX},	//	6
	{LEVELS_CHAPTER03_LEVEL04_TBK,LEVELS_CHAPTER03_LEVEL0402_LVL,LEVELS_CHAPTER03_LEVEL04_TEX},	//	7
	{LEVELS_CHAPTER03_LEVEL04_TBK,LEVELS_CHAPTER03_LEVEL0403_LVL,LEVELS_CHAPTER03_LEVEL04_TEX},	//	8
	{LEVELS_CHAPTER03_LEVEL04_TBK,LEVELS_CHAPTER03_LEVEL0404_LVL,LEVELS_CHAPTER03_LEVEL04_TEX},	//	9
	{LEVELS_CHAPTER03_LEVEL04_TBK,LEVELS_CHAPTER03_LEVEL0404X_LVL,LEVELS_CHAPTER03_LEVEL04_TEX},	//	10
	{LEVELS_CHAPTER03_LEVEL04_TBK,LEVELS_CHAPTER03_LEVEL0405_LVL,LEVELS_CHAPTER03_LEVEL04_TEX},	//	11
	{LEVELS_CHAPTER03_LEVEL04_TBK,LEVELS_CHAPTER03_LEVEL0406_LVL,LEVELS_CHAPTER03_LEVEL04_TEX},	//	12
// C4 L1
	{LEVELS_CHAPTER04_LEVEL01_TBK,LEVELS_CHAPTER04_LEVEL0101_LVL,LEVELS_CHAPTER04_LEVEL01_TEX},	//	13
	{LEVELS_CHAPTER04_LEVEL01_TBK,LEVELS_CHAPTER04_LEVEL0102_LVL,LEVELS_CHAPTER04_LEVEL01_TEX},	//	14
	{LEVELS_CHAPTER04_LEVEL01_TBK,LEVELS_CHAPTER04_LEVEL0103_LVL,LEVELS_CHAPTER04_LEVEL01_TEX},	//	15
	{LEVELS_CHAPTER04_LEVEL01_TBK,LEVELS_CHAPTER04_LEVEL0104_LVL,LEVELS_CHAPTER04_LEVEL01_TEX},	//	16
// C4 L2
	{LEVELS_CHAPTER04_LEVEL02_TBK,LEVELS_CHAPTER04_LEVEL0201_LVL,LEVELS_CHAPTER04_LEVEL02_TEX},	//	17
	{LEVELS_CHAPTER04_LEVEL02_TBK,LEVELS_CHAPTER04_LEVEL0202_LVL,LEVELS_CHAPTER04_LEVEL02_TEX},	//	18
	{LEVELS_CHAPTER04_LEVEL02_TBK,LEVELS_CHAPTER04_LEVEL0203_LVL,LEVELS_CHAPTER04_LEVEL02_TEX},	//	19
	{LEVELS_CHAPTER04_LEVEL02_TBK,LEVELS_CHAPTER04_LEVEL0204_LVL,LEVELS_CHAPTER04_LEVEL02_TEX},	//	20
	{LEVELS_CHAPTER04_LEVEL02_TBK,LEVELS_CHAPTER04_LEVEL0205_LVL,LEVELS_CHAPTER04_LEVEL02_TEX},	//	20
// C4 L3 - 6 parts
	{LEVELS_CHAPTER04_LEVEL03_TBK,LEVELS_CHAPTER04_LEVEL0301_LVL,LEVELS_CHAPTER04_LEVEL03_TEX},	//	21
	{LEVELS_CHAPTER04_LEVEL03_TBK,LEVELS_CHAPTER04_LEVEL0302_LVL,LEVELS_CHAPTER04_LEVEL03_TEX},	//	22
	{LEVELS_CHAPTER04_LEVEL03_TBK,LEVELS_CHAPTER04_LEVEL0303_LVL,LEVELS_CHAPTER04_LEVEL03_TEX},	//	23
	{LEVELS_CHAPTER04_LEVEL03_TBK,LEVELS_CHAPTER04_LEVEL0304_LVL,LEVELS_CHAPTER04_LEVEL03_TEX},	//	24
	{LEVELS_CHAPTER04_LEVEL03_TBK,LEVELS_CHAPTER04_LEVEL0305_LVL,LEVELS_CHAPTER04_LEVEL03_TEX},	//	25
	{LEVELS_CHAPTER04_LEVEL03_TBK,LEVELS_CHAPTER04_LEVEL0306_LVL,LEVELS_CHAPTER04_LEVEL03_TEX},	//	26
// C4 L4 - 7 parts
	{LEVELS_CHAPTER04_LEVEL04_TBK,LEVELS_CHAPTER04_LEVEL0401_LVL,LEVELS_CHAPTER04_LEVEL04_TEX},	//	27
	{LEVELS_CHAPTER04_LEVEL04_TBK,LEVELS_CHAPTER04_LEVEL0402_LVL,LEVELS_CHAPTER04_LEVEL04_TEX},	//	28
	{LEVELS_CHAPTER04_LEVEL04_TBK,LEVELS_CHAPTER04_LEVEL0403_LVL,LEVELS_CHAPTER04_LEVEL04_TEX},	//	29
	{LEVELS_CHAPTER04_LEVEL04_TBK,LEVELS_CHAPTER04_LEVEL0404_LVL,LEVELS_CHAPTER04_LEVEL04_TEX},	//	30
	{LEVELS_CHAPTER04_LEVEL04_TBK,LEVELS_CHAPTER04_LEVEL0405_LVL,LEVELS_CHAPTER04_LEVEL04_TEX},	//	31
	{LEVELS_CHAPTER04_LEVEL04_TBK,LEVELS_CHAPTER04_LEVEL0406_LVL,LEVELS_CHAPTER04_LEVEL04_TEX},	//	32
	{LEVELS_CHAPTER04_LEVEL04_TBK,LEVELS_CHAPTER04_LEVEL0407_LVL,LEVELS_CHAPTER04_LEVEL04_TEX},	//	33

// C5 L2 - 4 parts
//	{LEVELS_CHAPTER05_LEVEL02_TBK,LEVELS_CHAPTER05_LEVEL0201_LVL,LEVELS_CHAPTER05_LEVEL02_TEX},	//	34
	{LEVELS_CHAPTER05_LEVEL02_TBK,LEVELS_CHAPTER05_LEVEL0202_LVL,LEVELS_CHAPTER05_LEVEL02_TEX},	//	35
	{LEVELS_CHAPTER05_LEVEL02_TBK,LEVELS_CHAPTER05_LEVEL0203_LVL,LEVELS_CHAPTER05_LEVEL02_TEX},	//	36
	{LEVELS_CHAPTER05_LEVEL02_TBK,LEVELS_CHAPTER05_LEVEL0204_LVL,LEVELS_CHAPTER05_LEVEL02_TEX},	//	37
	{LEVELS_CHAPTER05_LEVEL02_TBK,LEVELS_CHAPTER05_LEVEL0205_LVL,LEVELS_CHAPTER05_LEVEL02_TEX},	//	38

// C5 L3 - 5 parts
	{LEVELS_CHAPTER05_LEVEL03_TBK,LEVELS_CHAPTER05_LEVEL0301_LVL,LEVELS_CHAPTER05_LEVEL03_TEX},	//	34
	{LEVELS_CHAPTER05_LEVEL03_TBK,LEVELS_CHAPTER05_LEVEL0302_LVL,LEVELS_CHAPTER05_LEVEL03_TEX},	//	35
	{LEVELS_CHAPTER05_LEVEL03_TBK,LEVELS_CHAPTER05_LEVEL0303_LVL,LEVELS_CHAPTER05_LEVEL03_TEX},	//	36
	{LEVELS_CHAPTER05_LEVEL03_TBK,LEVELS_CHAPTER05_LEVEL0304_LVL,LEVELS_CHAPTER05_LEVEL03_TEX},	//	37
	{LEVELS_CHAPTER05_LEVEL03_TBK,LEVELS_CHAPTER05_LEVEL0305_LVL,LEVELS_CHAPTER05_LEVEL03_TEX},	//	38

// C6 L1 - 2 parts
	{LEVELS_CHAPTER06_LEVEL01_TBK,LEVELS_CHAPTER06_LEVEL0101_LVL,LEVELS_CHAPTER06_LEVEL01_TEX},	//	39
	{LEVELS_CHAPTER06_LEVEL01_TBK,LEVELS_CHAPTER06_LEVEL0102_LVL,LEVELS_CHAPTER06_LEVEL01_TEX},	//	40
	{LEVELS_CHAPTER06_LEVEL03_TBK,LEVELS_CHAPTER06_LEVEL0301_LVL,LEVELS_CHAPTER06_LEVEL03_TEX},	//	41

};
const	static int	LvlTableSize=sizeof(LvlTable)/sizeof(sLvlTab);

extern int	s_globalLevelSelectThing;
#if defined(__USER_daveo__)
int	Lvl=-1;
#endif
/*****************************************************************************/
void 	CLevel::init()
{
#if defined(__USER_daveo__)
		if (Lvl>-1) s_globalLevelSelectThing=Lvl;
#endif
		if (s_globalLevelSelectThing>=LvlTableSize) s_globalLevelSelectThing=0;

		for (int i=0; i<CLayerTile::LAYER_TILE_TYPE_MAX; i++)
		{
			TileLayers[i]=0;
		}
		MapPos.vx=0;		
		MapPos.vy=0;
/*
		TileBankHdr=(sTileBankHdr *)CFileIO::loadFile(LEVELS_CHAPTER02_LEVEL04_TBK,"Tile Bank Data");
		LevelHdr=(sLvlHdr *)CFileIO::loadFile(LEVELS_CHAPTER02_LEVEL0401_LVL,"Level Data");
		m_levelTPage=TPLoadTex(LEVELS_CHAPTER02_LEVEL04_TEX);
*/
		TileBankHdr=(sTileBankHdr *)CFileIO::loadFile(LvlTable[s_globalLevelSelectThing].TileBank,"Tile Bank Data");
		LevelHdr=(sLvlHdr *)CFileIO::loadFile(LvlTable[s_globalLevelSelectThing ].Level,"Level Data");
		m_levelTPage=TPLoadTex(LvlTable[s_globalLevelSelectThing ].Tex);

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
			CLayerTile	*NewLayer=new ("Back Layer") CLayerBack(Layer,  TileList, TriList, QuadList, VtxList);
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
#ifdef __SHOW_COLLISION__
		if(CollisionLayer)
		{
			CollisionLayer->render(MapPos);
		}
#endif
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
