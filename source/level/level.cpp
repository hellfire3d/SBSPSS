/*************/
/*** Level ***/
/*************/

#include 	"system\global.h"
#include 	"fileio\fileio.h"
#include 	"utils\utils.h"
#include	"gfx\tpage.h"
#include	<DStructs.h>

#include	"level\level.h"
#include	"level\layer.h"
#include	"level\layertile.h"

/*****************************************************************************/
CLevel::CLevel()
{
	for (int i=0; i<CLayer::LAYER_TYPE_MAX; i++)
	{
		LayerList[i]=0;
	}
}

/*****************************************************************************/
CLevel::~CLevel()
{
	for (int i=0; i<CLayer::LAYER_TYPE_MAX; i++)
	{
		if (LayerList[i]) delete LayerList[i];
	}
}

/*****************************************************************************/
void 	CLevel::init()
{
		LevelHdr=(sLvlHdr *)CFileIO::loadFile(CHAPTER01_LEVEL01_LVL,"Level Data");
		TPLoadTex(CHAPTER01_LEVEL01_TEX);

		LevelHdr->TriList=(sTri*)MakePtr(LevelHdr,(int)LevelHdr->TriList);
		LevelHdr->QuadList=(sQuad*)MakePtr(LevelHdr,(int)LevelHdr->QuadList);
		LevelHdr->VtxList=(sVtx*)MakePtr(LevelHdr,(int)LevelHdr->VtxList);
		LevelHdr->TileList=(sTile*)MakePtr(LevelHdr,(int)LevelHdr->TileList);
		DAVE_DBGMSG("LayerCount=%i\n",GetLayerCount());
		LoadLayers();

		for (int i=0; i<CLayer::LAYER_TYPE_MAX; i++)
		{
			if (LayerList[i]) LayerList[i]->init();
		}

}

/*****************************************************************************/
void	CLevel::LoadLayers()
{
int			LayerCount=GetLayerCount();
int			*HdrOfs=(int*)MakePtr(LevelHdr,sizeof(sLvlHdr));

		for (int i=0; i<LayerCount; i++)
		{
			sLayerHdr	*Hdr=(sLayerHdr*)MakePtr(LevelHdr,*HdrOfs++);
			DAVE_DBGMSG("T:%i S:%i W:%i H:%i\n",Hdr->Type,Hdr->SubType,Hdr->Width,Hdr->Height);
			int	LayerType=CLayer::GetLayerType(Hdr->Type,Hdr->SubType);
			switch (LayerType)
			{
				case CLayer::LAYER_TYPE_TILE_BACK:
					LayerList[LayerType]=new ("Back Layer") CLayerTile(Hdr,LevelHdr->TileList, LevelHdr->TriList, LevelHdr->QuadList, LevelHdr->VtxList);
					break;
				case CLayer::LAYER_TYPE_TILE_MID:
					LayerList[LayerType]=new ("Mid Layer") CLayerTile(Hdr,LevelHdr->TileList, LevelHdr->TriList, LevelHdr->QuadList, LevelHdr->VtxList);
					break;
				case CLayer::LAYER_TYPE_TILE_ACTION:
					LayerList[LayerType]=new ("Action Layer") CLayerTile(Hdr,LevelHdr->TileList, LevelHdr->TriList, LevelHdr->QuadList, LevelHdr->VtxList);
					break;
				case CLayer::LAYER_TYPE_TILE_FORE:
					LayerList[LayerType]=new ("Fore Layer") CLayerTile(Hdr,LevelHdr->TileList, LevelHdr->TriList, LevelHdr->QuadList, LevelHdr->VtxList);
					break;
				default:
					DAVE_DBGMSG("%i\n",LayerType);
					ASSERT(!"Unknown Layer");
					break;
			}
		}
}

/*****************************************************************************/
void	CLevel::shutdown()
{
		MemFree(LevelHdr);
}

/*****************************************************************************/
void 	CLevel::render()
{
	for (int i=0; i<CLayer::LAYER_TYPE_MAX; i++)
	{
		if (LayerList[i]) LayerList[i]->render();
	}
}

/*****************************************************************************/
void	CLevel::think(int _frames)
{
	for (int i=0; i<CLayer::LAYER_TYPE_MAX; i++)
	{
		if (LayerList[i]) LayerList[i]->think(_frames);
	}
}


/*****************************************************************************/
