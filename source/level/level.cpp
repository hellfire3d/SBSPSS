/*************/
/*** Level ***/
/*************/

#include 	"system\global.h"
#include 	"fileio\fileio.h"
#include 	"utils\utils.h"
#include	"utils\pak.h"
#include	"gfx\tpage.h"
#include	"gfx\prim.h"
#include	<DStructs.h>
#include	"game\game.h"

#include	"level\level.h"
#include	"level\layertile.h"
#include	"level\layerback.h"
#include	"level\layertile3d.h"
#include	"level\layercollision.h"

#include "gfx\font.h"

#include	"triggers\trigger.h"
/*
#ifndef	__TRIGGERS_TLEVEXIT_H__
#include "triggers\tlevexit.h"
#endif

#ifndef	__TRIGGERS_TRESTART_H__
#include "triggers\trestart.h"
#endif

#ifndef	__TRIGGERS_TTELEPRT_H__
#include "triggers\tteleprt.h"
#endif

#ifndef	__TRIGGERS_TCAMLOCK_H__
#include "triggers\tcamlock.h"
#endif

#ifndef	__TRIGGERS_TWATER_H__
#include "triggers\twater.h"
#endif
*/

#ifndef __PICKUPS_PICKUP_H__
#include "pickups\pickup.h"
#endif

#ifndef	__PICKUPS_PSPATULA_H__
#include "pickups\pspatula.h"
#endif

#ifndef	__PICKUPS_PKELP_H__
#include "pickups\pkelp.h"
#endif

#ifndef	__SOUND_SOUND_H__
#include "sound\sound.h"
#endif

#ifndef __VID_HEADER_
#include "system\vid.h"
#endif

#include	"gfx\actor.h"

#ifndef	__ENEMY_NPC_H__
#include "enemy\npc.h"
#endif

#ifndef __FRIEND_FRIEND_H__
#include "friend\friend.h"
#endif

#ifndef __PLATFORM_PLATFORM_H__
#include "platform\platform.h"
#endif

#ifndef __HAZARD_HAZARD_H__
#include "hazard\hazard.h"
#endif

#ifndef __PROJECTL_PROJECTL_H__
#include "projectl\projectl.h"
#endif

#ifndef __JELLFISH_JELLFISH_H__
#include "jellfish\jellfish.h"
#endif

#ifndef	__GAME_GAMESLOT_H__
#include "game\gameslot.h"
#endif

#ifndef __LOCALE_TEXTDBASE_H__
#include "locale\textdbase.h"
#endif

#ifndef _ANIMTEX_HEADER_
#include "gfx\animtex.h"
#endif

#include	"fx\fx.h"
#include	"fx\fxfallingtile.h"


// Level data include files:
#include <CHAPTER01_LEVEL01_INF.h>
#include <CHAPTER01_LEVEL02_INF.h>
#include <CHAPTER01_LEVEL03_INF.h>
#include <CHAPTER01_LEVEL04_INF.h>
#include <CHAPTER02_LEVEL01_INF.h>
#include <CHAPTER02_LEVEL02_INF.h>
#include <CHAPTER02_LEVEL03_INF.h>
#include <CHAPTER02_LEVEL04_INF.h>
#include <CHAPTER03_LEVEL01_INF.h>
#include <CHAPTER03_LEVEL02_INF.h>
#include <CHAPTER03_LEVEL03_INF.h>
#include <CHAPTER03_LEVEL04_INF.h>
#include <CHAPTER04_LEVEL01_INF.h>
#include <CHAPTER04_LEVEL02_INF.h>
#include <CHAPTER04_LEVEL03_INF.h>
#include <CHAPTER04_LEVEL04_INF.h>
#include <CHAPTER05_LEVEL01_INF.h>
#include <CHAPTER05_LEVEL02_INF.h>
#include <CHAPTER05_LEVEL03_INF.h>
#include <CHAPTER05_LEVEL04_INF.h>
#include <CHAPTER06_LEVEL01_INF.h>
#include <CHAPTER06_LEVEL02_INF.h>
#include <CHAPTER06_LEVEL03_INF.h>
#include <CHAPTER06_LEVEL04_INF.h>
#include <CHAPTER06_LEVEL05_INF.h>

#ifdef __VERSION_DEBUG__
int	PretendToBeAPS2=false;
#endif

/*****************************************************************************/
 sLvlTab	LvlTable[]=
{
// C1
	{1,1,	-1,	CHAPTER01_LEVEL01_INF_TOTAL_ITEM_GOLDEN_SPATULA,	LEVELS_CHAPTER01_LEVEL01_LVL,LEVELS_CHAPTER01_LEVEL01_TEX,	CSoundMediator::SONG_CHAPTER1_LEVEL1,			STR__CHAPTER_1,STR__CHAPTER_1_LEVEL_1,		CFmaScene::FMA_SCRIPT__NONE			},
	{1,2,	-1,	CHAPTER01_LEVEL02_INF_TOTAL_ITEM_GOLDEN_SPATULA,	LEVELS_CHAPTER01_LEVEL02_LVL,LEVELS_CHAPTER01_LEVEL02_TEX,	CSoundMediator::SONG_CHAPTER1_LEVEL2,			STR__CHAPTER_1,STR__CHAPTER_1_LEVEL_2,		CFmaScene::FMA_SCRIPT__NONE			},
	{1,3,	-1,	CHAPTER01_LEVEL03_INF_TOTAL_ITEM_GOLDEN_SPATULA,	LEVELS_CHAPTER01_LEVEL03_LVL,LEVELS_CHAPTER01_LEVEL03_TEX,	CSoundMediator::SONG_CHAPTER1_LEVEL3,			STR__CHAPTER_1,STR__CHAPTER_1_LEVEL_3,		CFmaScene::FMA_SCRIPT__NONE			},
	{1,4,	-1,	CHAPTER01_LEVEL04_INF_TOTAL_ITEM_GOLDEN_SPATULA,	LEVELS_CHAPTER01_LEVEL04_LVL,LEVELS_CHAPTER01_LEVEL04_TEX,	CSoundMediator::SONG_CHAPTER1_LEVEL4,			STR__CHAPTER_1,STR__CHAPTER_1_LEVEL_4,		CFmaScene::FMA_SCRIPT__CH1FINISHED	},
	{1,5,	-1,	CHAPTER06_LEVEL01_INF_TOTAL_ITEM_KELP_TOKEN,		LEVELS_CHAPTER06_LEVEL01_LVL,LEVELS_CHAPTER06_LEVEL01_TEX,	CSoundMediator::SONG_CHAPTER6_BIGWHEEL,			STR__KELP_WORLD,STR__CHAPTER_6_LEVEL_1,		CFmaScene::FMA_SCRIPT__NONE			},
// C2
	{2,1,	+1,	CHAPTER02_LEVEL01_INF_TOTAL_ITEM_GOLDEN_SPATULA,	LEVELS_CHAPTER02_LEVEL01_LVL,LEVELS_CHAPTER02_LEVEL01_TEX,	CSoundMediator::SONG_CHAPTER2_LEVEL1,			STR__CHAPTER_2,STR__CHAPTER_2_LEVEL_1,		CFmaScene::FMA_SCRIPT__NONE			},
	{2,2,	+1,	CHAPTER02_LEVEL02_INF_TOTAL_ITEM_GOLDEN_SPATULA,	LEVELS_CHAPTER02_LEVEL02_LVL,LEVELS_CHAPTER02_LEVEL02_TEX,	CSoundMediator::SONG_CHAPTER2_LEVEL2,			STR__CHAPTER_2,STR__CHAPTER_2_LEVEL_2,		CFmaScene::FMA_SCRIPT__NONE			},
	{2,3,	-1,	CHAPTER02_LEVEL03_INF_TOTAL_ITEM_GOLDEN_SPATULA,	LEVELS_CHAPTER02_LEVEL03_LVL,LEVELS_CHAPTER02_LEVEL03_TEX,	CSoundMediator::SONG_CHAPTER2_LEVEL3,			STR__CHAPTER_2,STR__CHAPTER_2_LEVEL_3,		CFmaScene::FMA_SCRIPT__NONE			},
	{2,4,	-1,	CHAPTER02_LEVEL04_INF_TOTAL_ITEM_GOLDEN_SPATULA,	LEVELS_CHAPTER02_LEVEL04_LVL,LEVELS_CHAPTER02_LEVEL04_TEX,	CSoundMediator::SONG_CHAPTER2_LEVEL4,			STR__CHAPTER_2,STR__CHAPTER_2_LEVEL_4,		CFmaScene::FMA_SCRIPT__CH2FINISHED	},
	{2,5,	-1,	CHAPTER06_LEVEL02_INF_TOTAL_ITEM_KELP_TOKEN,		LEVELS_CHAPTER06_LEVEL02_LVL,LEVELS_CHAPTER06_LEVEL02_TEX,	CSoundMediator::SONG_CHAPTER6_ROLLERCOASTER,		STR__KELP_WORLD,STR__CHAPTER_6_LEVEL_2,		CFmaScene::FMA_SCRIPT__NONE			},
// C3																																																										
	{3,1,	+1,	CHAPTER03_LEVEL01_INF_TOTAL_ITEM_GOLDEN_SPATULA,	LEVELS_CHAPTER03_LEVEL01_LVL,LEVELS_CHAPTER03_LEVEL01_TEX,	CSoundMediator::SONG_CHAPTER3_LEVEL1,			STR__CHAPTER_3,STR__CHAPTER_3_LEVEL_1,		CFmaScene::FMA_SCRIPT__NONE			},
	{3,2,	-1,	CHAPTER03_LEVEL02_INF_TOTAL_ITEM_GOLDEN_SPATULA,	LEVELS_CHAPTER03_LEVEL02_LVL,LEVELS_CHAPTER03_LEVEL02_TEX,	CSoundMediator::SONG_CHAPTER3_LEVEL2,			STR__CHAPTER_3,STR__CHAPTER_3_LEVEL_2,		CFmaScene::FMA_SCRIPT__NONE			},
	{3,3,	-1,	CHAPTER03_LEVEL03_INF_TOTAL_ITEM_GOLDEN_SPATULA,	LEVELS_CHAPTER03_LEVEL03_LVL,LEVELS_CHAPTER03_LEVEL03_TEX,	CSoundMediator::SONG_CHAPTER3_LEVEL3,			STR__CHAPTER_3,STR__CHAPTER_3_LEVEL_3,		CFmaScene::FMA_SCRIPT__NONE			},
	{3,4,	-1,	CHAPTER03_LEVEL04_INF_TOTAL_ITEM_GOLDEN_SPATULA,	LEVELS_CHAPTER03_LEVEL04_LVL,LEVELS_CHAPTER03_LEVEL04_TEX,	CSoundMediator::SONG_CHAPTER3_LEVEL4,			STR__CHAPTER_3,STR__CHAPTER_3_LEVEL_4,		CFmaScene::FMA_SCRIPT__CH3FINISHED	},
	{3,5,	-1,	CHAPTER06_LEVEL03_INF_TOTAL_ITEM_KELP_TOKEN,		LEVELS_CHAPTER06_LEVEL03_LVL,LEVELS_CHAPTER06_LEVEL03_TEX,	CSoundMediator::SONG_CHAPTER6_TUNNELOFLOVE,			STR__KELP_WORLD,STR__CHAPTER_6_LEVEL_3,		CFmaScene::FMA_SCRIPT__NONE			},
// C4																																																										
	{4,1,	-1,	CHAPTER04_LEVEL01_INF_TOTAL_ITEM_GOLDEN_SPATULA,	LEVELS_CHAPTER04_LEVEL01_LVL,LEVELS_CHAPTER04_LEVEL01_TEX,	CSoundMediator::SONG_CHAPTER4_LEVEL1,			STR__CHAPTER_4,STR__CHAPTER_4_LEVEL_1,		CFmaScene::FMA_SCRIPT__NONE			},
	{4,2,	-1,	CHAPTER04_LEVEL02_INF_TOTAL_ITEM_GOLDEN_SPATULA,	LEVELS_CHAPTER04_LEVEL02_LVL,LEVELS_CHAPTER04_LEVEL02_TEX,	CSoundMediator::SONG_CHAPTER4_LEVEL2,			STR__CHAPTER_4,STR__CHAPTER_4_LEVEL_2,		CFmaScene::FMA_SCRIPT__NONE			},
	{4,3,	-1,	CHAPTER04_LEVEL03_INF_TOTAL_ITEM_GOLDEN_SPATULA,	LEVELS_CHAPTER04_LEVEL03_LVL,LEVELS_CHAPTER04_LEVEL03_TEX,	CSoundMediator::SONG_CHAPTER4_LEVEL3,			STR__CHAPTER_4,STR__CHAPTER_4_LEVEL_3,		CFmaScene::FMA_SCRIPT__NONE			},
	{4,4,	-1,	CHAPTER04_LEVEL04_INF_TOTAL_ITEM_GOLDEN_SPATULA,	LEVELS_CHAPTER04_LEVEL04_LVL,LEVELS_CHAPTER04_LEVEL04_TEX,	CSoundMediator::SONG_CHAPTER4_LEVEL4,			STR__CHAPTER_4,STR__CHAPTER_4_LEVEL_4,		CFmaScene::FMA_SCRIPT__CH4FINISHED	},
	{4,5,	-1,	CHAPTER06_LEVEL04_INF_TOTAL_ITEM_KELP_TOKEN,		LEVELS_CHAPTER06_LEVEL04_LVL,LEVELS_CHAPTER06_LEVEL04_TEX,	CSoundMediator::SONG_CHAPTER6_GHOSTTRAIN,		STR__KELP_WORLD,STR__CHAPTER_6_LEVEL_4,		CFmaScene::FMA_SCRIPT__NONE			},
// C5																																																										
	{5,1,	-1,	CHAPTER05_LEVEL01_INF_TOTAL_ITEM_GOLDEN_SPATULA,	LEVELS_CHAPTER05_LEVEL01_LVL,LEVELS_CHAPTER05_LEVEL01_TEX,	CSoundMediator::SONG_CHAPTER5_LEVEL1,			STR__CHAPTER_5,STR__CHAPTER_5_LEVEL_1,		CFmaScene::FMA_SCRIPT__NONE			},
	{5,2,	-1,	CHAPTER05_LEVEL02_INF_TOTAL_ITEM_GOLDEN_SPATULA,	LEVELS_CHAPTER05_LEVEL02_LVL,LEVELS_CHAPTER05_LEVEL02_TEX,	CSoundMediator::SONG_CHAPTER5_LEVEL2,			STR__CHAPTER_5,STR__CHAPTER_5_LEVEL_2,		CFmaScene::FMA_SCRIPT__NONE			},
	{5,3,	-1,	CHAPTER05_LEVEL03_INF_TOTAL_ITEM_GOLDEN_SPATULA,	LEVELS_CHAPTER05_LEVEL03_LVL,LEVELS_CHAPTER05_LEVEL03_TEX,	CSoundMediator::SONG_CHAPTER5_LEVEL3,			STR__CHAPTER_5,STR__CHAPTER_5_LEVEL_3,		CFmaScene::FMA_SCRIPT__NONE			},
	{5,4,	-1,	CHAPTER05_LEVEL04_INF_TOTAL_ITEM_GOLDEN_SPATULA,	LEVELS_CHAPTER05_LEVEL04_LVL,LEVELS_CHAPTER05_LEVEL04_TEX,	CSoundMediator::SONG_CHAPTER5_LEVEL4,			STR__CHAPTER_5,STR__CHAPTER_5_LEVEL_4,		CFmaScene::FMA_SCRIPT__CH5FINISHED	},
	{5,5,	-1,	CHAPTER06_LEVEL05_INF_TOTAL_ITEM_KELP_TOKEN,		LEVELS_CHAPTER06_LEVEL05_LVL,LEVELS_CHAPTER06_LEVEL05_TEX,	CSoundMediator::SONG_CHAPTER6_SNAILRACE,		STR__KELP_WORLD,STR__CHAPTER_6_LEVEL_5,		CFmaScene::FMA_SCRIPT__NONE			},
// FMA																																																										
	{6,0,	-1,	0,													LEVELS_FMA_SHADYSHOALS_LVL,LEVELS_FMA_SHADYSHOALS_TEX,		CSoundMediator::SONG_FMA,						STR__NOW_LOADING,STR__BLANK,				CFmaScene::FMA_SCRIPT__NONE			},
	{6,0,	-1,	0,													LEVELS_FMA_CONTROLROOM_LVL,LEVELS_FMA_CONTROLROOM_TEX,		CSoundMediator::SONG_FMA,						STR__NOW_LOADING,STR__BLANK,				CFmaScene::FMA_SCRIPT__NONE			},

};
const	int		LvlTableSize=sizeof(LvlTable)/sizeof(sLvlTab);

const FileEquate	loadingScreens[6]=
{
	LOADINGSCREENS_PINEAPPLE_GFX,	// Chapter 1
	LOADINGSCREENS_CULTURE_GFX,		// Chapter 2
	LOADINGSCREENS_PICKLES_GFX,		// Chapter 3
	LOADINGSCREENS_MONITOR_GFX,		// Chapter 4
	LOADINGSCREENS_KARATE_GFX,		// Chapter 5
};

/*****************************************************************************/
DVECTOR		CLevel::MapPos;
DVECTOR		CLevel::s_playerSpawnPos;
int			CLevel::s_playerFacing;
sLevelHdr	*CLevel::LevelHdr;

u8 CLevel::m_isBossRespawn;
s32 CLevel::m_bossHealth;
CNpcCheckpointHazard	*CLevel::m_checkpoint;

/*****************************************************************************/
CLevel::CLevel()
{
}

/*****************************************************************************/
void 	CLevel::init(int LevelNo)
{
// Load it
sLvlTab *lvlTab=&LvlTable[LevelNo];

		CSoundMediator::setSong((CSoundMediator::SONGID)lvlTab->songId);

		LevelHdr=(sLevelHdr*)CFileIO::loadFile(lvlTab->LevelFilename,"Level");
		LevelHdr->ElemBank2d=(sElem2d*)	MakePtr(LevelHdr,(int)LevelHdr->ElemBank2d);
		LevelHdr->ElemBank3d=(sElem3d*)	MakePtr(LevelHdr,(int)LevelHdr->ElemBank3d);
		LevelHdr->TriList=(sTri*)		MakePtr(LevelHdr,(int)LevelHdr->TriList);
		LevelHdr->QuadList=(sQuad*)		MakePtr(LevelHdr,(int)LevelHdr->QuadList);
		LevelHdr->VtxList=(sVtx*)		MakePtr(LevelHdr,(int)LevelHdr->VtxList);
		LevelHdr->VtxIdxList=(u16*)		MakePtr(LevelHdr,(int)LevelHdr->VtxIdxList);
		LevelHdr->ModelList=(sModel*)	MakePtr(LevelHdr,(int)LevelHdr->ModelList);

// Deal with RGB Tables (and create if none)
		if (LevelHdr->RGBLayer)
		{
			sLayerRGBHdr	*RGBHdr=(sLayerRGBHdr*)	MakePtr(LevelHdr,(int)LevelHdr->RGBLayer+sizeof(sLayerHdr));
			m_RGBMap=(u8*)			MakePtr(LevelHdr,(int)RGBHdr->RGBMap);
			m_RGBTable=(u8*)		MakePtr(LevelHdr,(int)RGBHdr->RGBTable);
		}
		else
		{ // Make blank RGB data
			sLayerHdr	*LayerLayer=(sLayerHdr*)MakePtr(LevelHdr,LevelHdr->ActionLayer);
				
			int	LvlSize=LayerLayer->Width*LayerLayer->Height;

			m_RGBMap=(u8*)MemAlloc(LvlSize,"BlankRGBMap"); ASSERT(m_RGBMap);
			memset(m_RGBMap,0,LvlSize);
			m_RGBTable=(u8*)MemAlloc(16*4,"BlankRGBTable");	ASSERT(m_RGBTable);
			u8 RGB=127;
			for (int c=0; c<16; c++)
			{
				m_RGBTable[(c*4)+0]=RGB;
				m_RGBTable[(c*4)+1]=RGB;
				m_RGBTable[(c*4)+2]=RGB;
				m_RGBTable[(c*4)+3]=0;
				RGB+=6;
			}
		}

		CModelGfx::SetData(LevelHdr);
		m_levelTPage=TPLoadTex(lvlTab->TexFilename);

		s_playerSpawnPos.vx=LevelHdr->PlayerStartX*16;
		s_playerSpawnPos.vy=LevelHdr->PlayerStartY*16+16;
		s_playerFacing=lvlTab->SBFacing;

		MapPos.vx=MapPos.vy=0;
		initLayers();

		m_isBossRespawn = false;
		m_checkpoint = NULL;
		m_bossHealth = 0;

		int level=GameScene.getLevelNumber();
		int chapter=GameScene.getChapterNumber();

		if ( chapter == 5 && level == 5 ||			// snail race
				chapter == 5 && level == 4 )		// tuna canning
		{
			CAnimTex::SetSpeed( -4 );
		}
}

/*****************************************************************************/
bool	CLevel::GetNextLevel(int &Lvl)
{
bool	Finished=false;
		Lvl++;

		if (Lvl>=LvlTableSize)
		{
			Lvl=0;
			Finished=true;
		}

		return(Finished);
}

/*****************************************************************************/
void	CLevel::DisplayLoadingScreen(int LevelNo)
{
ScalableFontBank	font;
char				buf[256];
u8					*s_image;
int					i;

			if (LevelNo==-1)
			{
				// default loading screen
				s_image=LoadPakScreen(LOADINGSCREENS_TEENAGE_GFX);
				sprintf(buf,"%s",TranslationDatabase::getString(STR__NOW_LOADING));
			}
			else if (LevelNo==-2)
			{
				// fma laoding screen..
				s_image=LoadPakScreen(	LOADINGSCREENS_PIZZA_GFX);
				sprintf(buf,"%s",TranslationDatabase::getString(STR__NOW_LOADING));
			}
			else
			{
				s_image=LoadPakScreen(loadingScreens[LvlTable[LevelNo].Chapter-1]);
				sprintf(buf,"%s\n\n%s",TranslationDatabase::getString(LvlTable[LevelNo].ChapterLoadingText),TranslationDatabase::getString(LvlTable[LevelNo].LevelLoadingText));
			}
			
			ASSERT(s_image);
			SetScreenImage(s_image);

			font.initialise(&standardFont);
			font.setJustification(FontBank::JUST_CENTRE);
			font.setScale(370);

			for(i=0;i<2;i++)
			{
				font.setColour(255,255,255);
				font.print(256  ,90  ,buf);
				font.setColour(0,0,0);
				font.print(256-1,90-1,buf);
				font.print(256-1,90  ,buf);
				font.print(256-1,90+1,buf);
				font.print(256  ,90-1,buf);
				font.print(256  ,90+1,buf);
				font.print(256+1,90-1,buf);
				font.print(256+1,90  ,buf);
				font.print(256+1,90+1,buf);

				PrimDisplay();
				VidSwapDraw();
			}
			font.dump();

			ClearScreenImage();
			MemFree(s_image);
			StartLoad();
			VSync(0);
}

/*****************************************************************************/
void	CLevel::initLayers()
{
// Init Layers
		for (int i=0; i<CLayerTile::LAYER_TILE_TYPE_MAX; i++)
		{
			TileLayers[i]=0;
		}
		CollisionLayer=0;
		ActorList=0;
		ItemList=0;
		PlatformList=0;
		FXList=0;
		HazardList=0;

// Back
		if (LevelHdr->BackLayer)
		{
			sLayerHdr	*Layer=(sLayerHdr*)MakePtr(LevelHdr,LevelHdr->BackLayer);
			CLayerTile	*NewLayer=new ("Back Layer") CLayerBack(LevelHdr,Layer);
			NewLayer->init(MapPos,3);
			TileLayers[CLayerTile::LAYER_TILE_TYPE_BACK]=NewLayer;
		}

// Mid
		if (LevelHdr->MidLayer)
		{
			sLayerHdr	*Layer=(sLayerHdr*)MakePtr(LevelHdr,LevelHdr->MidLayer);
			CLayerTile *NewLayer=new ("Mid Layer") CLayerTile(LevelHdr,Layer);
			NewLayer->init(MapPos,2);
			TileLayers[CLayerTile::LAYER_TILE_TYPE_MID]=NewLayer;
		}

// Action
		if (LevelHdr->ActionLayer)
		{
			sLayerHdr	*Layer=(sLayerHdr*)MakePtr(LevelHdr,LevelHdr->ActionLayer);
			CLayerTile *NewLayer=new ("Action Layer") CLayerTile3d(LevelHdr,Layer,m_RGBMap,m_RGBTable);
			NewLayer->init(MapPos,0);
			TileLayers[CLayerTile::LAYER_TILE_TYPE_ACTION]=NewLayer;
			MapSize.vx=Layer->Width;
			MapSize.vy=Layer->Height;
			MapSize16.vx=Layer->Width*16;
			MapSize16.vy=Layer->Height*16;
			CThingManager::setMapWH(MapSize);
		}
// Collision

		if (LevelHdr->CollisionLayer)
		{
			sLayerHdr	*Layer=(sLayerHdr*)MakePtr(LevelHdr,LevelHdr->CollisionLayer);
			CollisionLayer=new ("Collision Layer") CLayerCollision(Layer);
			CGameScene::setCollision(CollisionLayer);
			CreateTileStore();
		}
		else
		{
			ASSERT(!"Where is the collision, moron!");
		}
		
// Actors
		if (LevelHdr->ActorList)
		{
			sThingHdr	*Hdr=(sThingHdr*)MakePtr(LevelHdr,LevelHdr->ActorList);
			ActorCount=Hdr->Count;
			ActorList=(sThingActor**)MemAlloc(ActorCount*sizeof(sThingActor**),"Actor List");
			u8	*ThingPtr=(u8*)MakePtr(Hdr,sizeof(sThingHdr));
			for (int i=0; i<ActorCount; i++)
			{
				ActorList[i]=(sThingActor*)ThingPtr;
				ThingPtr+=sizeof(sThingActor);
				ThingPtr+=ActorList[i]->PointCount*sizeof(u16)*2;
			}
		}
// Platforms
		if (LevelHdr->PlatformList)
		{

			sThingHdr	*Hdr=(sThingHdr*)MakePtr(LevelHdr,LevelHdr->PlatformList);
			PlatformCount=Hdr->Count;
			PlatformList=(sThingPlatform**)MemAlloc(PlatformCount*sizeof(sThingPlatform**),"Platform List");
			u8	*ThingPtr=(u8*)MakePtr(Hdr,sizeof(sThingHdr));
			for (int i=0; i<PlatformCount; i++)
			{
				PlatformList[i]=(sThingPlatform*)ThingPtr;
				ThingPtr+=sizeof(sThingPlatform);
				ThingPtr+=PlatformList[i]->PointCount*sizeof(u16)*2;
			}
		}

// FX
		if (LevelHdr->FXList)
		{
			sThingHdr	*Hdr=(sThingHdr*)MakePtr(LevelHdr,LevelHdr->FXList);
			FXCount=Hdr->Count;
			FXList=(sThingFX*)MakePtr(Hdr,sizeof(sThingHdr));
		}

// Hazards
		if (LevelHdr->HazardList)
		{
			sThingHdr	*Hdr=(sThingHdr*)MakePtr(LevelHdr,LevelHdr->HazardList);
			HazardCount=Hdr->Count;
			HazardList=(sThingHazard**)MemAlloc(HazardCount*sizeof(sThingHazard**),"Hazard List");
			u8	*ThingPtr=(u8*)MakePtr(Hdr,sizeof(sThingHdr));
			for (int i=0; i<HazardCount; i++)
			{
				HazardList[i]=(sThingHazard*)ThingPtr;
				ThingPtr+=sizeof(sThingHazard);
				ThingPtr+=HazardList[i]->PointCount*sizeof(u16)*2;
			}
		}

		initThings(false);
}



/*****************************************************************************/
void	CLevel::initThings(int _respawningLevel)
{
	if (LevelHdr->TriggerList)
	{
		sThingHdr	*Hdr=(sThingHdr*)MakePtr(LevelHdr,LevelHdr->TriggerList);
		TriggerCount=Hdr->Count;
		TriggerList=(sThingTrigger*)MakePtr(Hdr,sizeof(sThingHdr));

// Cam Locks first
		for(int i=0;i<TriggerCount;i++)
		{
			if (TriggerList[i].Type==CTrigger::TRIGGER_CAMLOCK)
			{
				CTrigger::Create(&TriggerList[i]);
			}
		}
// the rest
		TriggerList=(sThingTrigger*)MakePtr(Hdr,sizeof(sThingHdr));
		for(int i=0;i<TriggerCount;i++)
		{
			if (TriggerList[i].Type!=CTrigger::TRIGGER_CAMLOCK)
			{
				CTrigger::Create(&TriggerList[i]);
			}
		}
	}

// Items
		if (LevelHdr->ItemList)
		{
			DVECTOR		pos;
			int			itemNumber;
			sThingHdr	*Hdr=(sThingHdr*)MakePtr(LevelHdr,LevelHdr->ItemList);
			ItemCount=Hdr->Count;
			ItemList=(sThingItem*)MakePtr(Hdr,sizeof(sThingHdr));
			itemNumber=0;
			for(int i=0;i<ItemCount;i++)
			{
				int			createThisPickup;
				int			isNumberedItem;
				CBasePickup	*newPickup;

				createThisPickup=true;
				isNumberedItem=false;
				if((PICKUP_TYPE)ItemList->Type==PICKUP__KELP_TOKEN)
				{
					isNumberedItem=true;
					if(CGameSlotManager::getSlotData()->isKelpTokenUncollected(GameScene.getChapterNumber()-1,GameScene.getLevelNumber()-1,itemNumber)==false)
					{
						createThisPickup=false;
					}
				}

				if((PICKUP_TYPE)ItemList->Type==PICKUP__HELMET&&
				   _respawningLevel)
				{
					createThisPickup=false;
				}

				if(createThisPickup)
				{
					pos.vx=ItemList->Pos.X<<4;
					pos.vy=ItemList->Pos.Y<<4;
					newPickup=createPickup((PICKUP_TYPE)ItemList->Type,&pos);
					if(isNumberedItem)
					{
						if((PICKUP_TYPE)ItemList->Type==PICKUP__KELP_TOKEN)
						{
							((CKelpTokenPickup*)newPickup)->setTokenNumber(itemNumber);
						}
						else
						{
							ASSERT(0);
						}
					}
				}
				if(isNumberedItem)
				{
					itemNumber++;
				}

				ItemList++;
			}
		}

// Init actors (needs moving and tidying
	int actorNum;
	int platformNum;
	int hazardNum;

	sThingActor **actorList = getActorList();
	if (actorList)
	{
		for ( actorNum = 0 ; actorNum < getActorCount() ; actorNum++ )
		{
			sThingActor	*ThisActor=actorList[actorNum];
			CGameScene::ACTOR_TYPE actorType = CGameScene::getActorType( actorList[actorNum]->Type );
			switch ( actorType )
			{
				case CGameScene::ACTOR_ENEMY_NPC:
					{
						CNpcEnemy *enemy;
						enemy=CNpcEnemy::Create(ThisActor);

						if ( enemy )
						{
							// if boss is killed and user dies, boss will not initialise and will return NULL

							enemy->setupWaypoints( ThisActor );
							enemy->postInit();

							if ( ThisActor->Speed )
							{
								enemy->setSpeed( ThisActor->Speed );
							}
						}
					}
					break;

				case CGameScene::ACTOR_FRIEND_NPC:
					{
						CNpcFriend *friendNpc;
						friendNpc=CNpcFriend::Create(ThisActor);
						friendNpc->setupWaypoints( ThisActor );
						friendNpc->postInit();
					}
					break;

				default:
					break;
			}
		}
	}

	sThingPlatform **platformList = getPlatformList();
	if (platformList)
	{
		for ( platformNum = 0 ; platformNum < getPlatformCount() ; platformNum++ )
		{
			sThingPlatform *ThisPlatform = platformList[platformNum];
			CNpcPlatform *platform;
			platform = CNpcPlatform::Create( ThisPlatform );
			platform->postInit();

			if ( ThisPlatform->Speed )
			{
				platform->setSpeed( ThisPlatform->Speed );
			}
		}
	}

	sThingHazard **hazardList = getHazardList();
	if (hazardList)
	{
		for ( hazardNum = 0 ; hazardNum < getHazardCount() ; hazardNum++ )
		{
			sThingHazard *ThisHazard = hazardList[hazardNum];
			CNpcHazard *hazard;
			hazard = CNpcHazard::Create( ThisHazard );
			if ( ThisHazard->Respawn )
			{
				hazard->setRespawnRate( ThisHazard->Respawn );
			}
		}
	}

	// initialise jellyfish generator

	CJellyfishGenerator::init();
	CThingManager::initAllThings();
	CThingManager::matchWheelsAndWeights();
	CThingManager::matchPressureSwitches();
	CThingManager::matchGaryTriggers();
}

/*****************************************************************************/
void	CLevel::respawnLevel()
{
	//m_isBossRespawn = false;
	//m_bossHealth = 0;

	CThingManager::killAllThingsForRespawn();
	initThings(true);
	ApplyTileStore();	// Put dem tiles back
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
		if (!LevelHdr->RGBLayer)
		{
			MemFree(m_RGBMap);
			MemFree(m_RGBTable);
		}

		if (CollisionLayer)
		{
			CollisionLayer->shutdown();
			delete CollisionLayer;
		}

		CSoundMediator::stopAllSound();
		for(int i=0;i<5;i++)
		{
			VSync(0);CSoundMediator::think(1);		// This is needed to let xmplay kill off the sounds properly (pkg)
		}

		if (ActorList) MemFree(ActorList);
		if (PlatformList) MemFree(PlatformList);
		if (HazardList) MemFree(HazardList);
		if (m_TileStore) MemFree(m_TileStore);

		MemFree(LevelHdr);
		CActorPool::Reset();
		TPFree(m_levelTPage);
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void 	CLevel::render()
{
#ifdef __VERSION_DEBUG__
// Clever code to make code not so confusing!!
// Emulate a PS2 by forcing it to run in 50/60 fps...by not drawing much!!
		VidSetClearScreen(PretendToBeAPS2);
		if(!PretendToBeAPS2)
#endif
// Render Layers
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
/*
DVECTOR	CLevel::getMapSize()
{
DVECTOR		size;
sLayerHdr	*layer;

		layer=(sLayerHdr*)MakePtr(LevelHdr,LevelHdr->ActionLayer);
		size.vx=layer->Width;
		size.vy=layer->Height;

		return size;
}
*/

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CLevel::destroyMapArea(DVECTOR const &Pos,int ColT)
{
DVECTOR		TL,BR;
//const int			ColT=COLLISION_TYPE_DESTRUCTABLE_WALL;


			TL.vx=Pos.vx&-16;;
			TL.vy=Pos.vy&-16;;
			BR=TL;

// Left
			while (CollisionLayer->getCollisionBlock(TL.vx-16,TL.vy)>>COLLISION_TYPE_FLAG_SHIFT==ColT) TL.vx-=16;
// Top
			while (CollisionLayer->getCollisionBlock(TL.vx,TL.vy-16)>>COLLISION_TYPE_FLAG_SHIFT==ColT) TL.vy-=16;
// Right
			while (CollisionLayer->getCollisionBlock(BR.vx+16,BR.vy)>>COLLISION_TYPE_FLAG_SHIFT==ColT) BR.vx+=16;
// Bottom
			while (CollisionLayer->getCollisionBlock(BR.vx,BR.vy+16)>>COLLISION_TYPE_FLAG_SHIFT==ColT) BR.vy+=16;

DVECTOR		DP;

			for (DP.vy=TL.vy; DP.vy<=BR.vy; DP.vy+=16)
			{
				for (DP.vx=TL.vx; DP.vx<=BR.vx; DP.vx+=16)
				{
					u8	*ColElem=CollisionLayer->getMapPtr(DP.vx,DP.vy);

					if (*ColElem>>COLLISION_TYPE_FLAG_SHIFT==ColT)
					{
						sTileMapElem	*MapElem=TileLayers[CLayerTile::LAYER_TILE_TYPE_ACTION]->getMapPtr(DP.vx,DP.vy);
						if (MapElem->Tile)
						{
							CFXFallingTile	*FX=(CFXFallingTile*)CFX::Create(CFX::FX_TYPE_FALLINGTILE,DP);
							FX->SetTile(MapElem->Tile);
							MapElem->Tile=0;
						}
						*ColElem=0;

					}

				}
			}
}
/*****************************************************************************/
// fixed now, so takes out whole vertical section, wherever it is hit 
// Note: relies on tiles not being top or bottom of map tho!!
void	CLevel::destroyMapTile(DVECTOR const &Pos,int ColT)
{
int				Width=getMapWidth();
DVECTOR			MP=Pos;
//const int		ColT=COLLISION_TYPE_DESTRUCTABLE_FLOOR;

// Goto Top
			while (*CollisionLayer->getMapPtr(MP.vx,MP.vy-1)>>COLLISION_TYPE_FLAG_SHIFT==ColT)
			{
				MP.vy-=16;
			}

u8				*ColElem=CollisionLayer->getMapPtr(MP.vx,MP.vy);
sTileMapElem	*MapElem=TileLayers[CLayerTile::LAYER_TILE_TYPE_ACTION]->getMapPtr(MP.vx,MP.vy);

// Thrash em down
			while (*ColElem>>COLLISION_TYPE_FLAG_SHIFT==ColT)
			{
				if (MapElem->Tile)
				{
					CFXFallingTile	*FX=(CFXFallingTile*)CFX::Create(CFX::FX_TYPE_FALLINGTILE,MP);
					FX->SetTile(MapElem->Tile);
					MapElem->Tile=0;
				}
				*ColElem=0;

				MapElem+=Width;
				ColElem+=Width;
				MP.vy+=16;
			}

}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CLevel::reverseMapConveyor()
{
DVECTOR		size = getMapSize();
int			ColT;

DVECTOR		DP;

			for (DP.vy=0; DP.vy<=(size.vy<<4); DP.vy+=16)
			{
				for (DP.vx=0; DP.vx<=(size.vx<<4); DP.vx+=16)
				{
					u8	*ColElem=CollisionLayer->getMapPtr(DP.vx,DP.vy);

					if ( (*ColElem)>>COLLISION_TYPE_FLAG_SHIFT == COLLISION_TYPE_MOVE_LEFT )
					{
						*ColElem &= ~COLLISION_TYPE_FLAG_MOVE_LEFT;
						*ColElem |= COLLISION_TYPE_FLAG_MOVE_RIGHT;
					}
					else if ( (*ColElem)>>COLLISION_TYPE_FLAG_SHIFT == COLLISION_TYPE_MOVE_RIGHT )
					{
						*ColElem &= ~COLLISION_TYPE_FLAG_MOVE_RIGHT;
						*ColElem |= COLLISION_TYPE_FLAG_MOVE_LEFT;
					}
				}
			}
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
// note: slippery used as special type in 4:4 boss arena
void	CLevel::CreateTileStore()
{
int		MapW=getMapWidth();
int		MapH=getMapHeight();
int		X,Y;
	
		m_TileStoreCount=0;
		m_TileStore=0;
// CountEm
		for (Y=0; Y<MapH; Y++)
		{
			for (X=0; X<MapW; X++)
			{
				u8	*ColElem=CollisionLayer->getMapPtr(X*16,Y*16);
				int	ColT = (*ColElem) & COLLISION_TYPE_MASK;
				if ( ColT==COLLISION_TYPE_FLAG_DESTRUCTABLE_FLOOR || ColT==COLLISION_TYPE_FLAG_DESTRUCTABLE_WALL || ColT==COLLISION_TYPE_FLAG_SLIPPERY)
				{
					m_TileStoreCount++;
				}
			}
		}
// StoreEm
		if (m_TileStoreCount)
		{
			m_TileStore=(sTileStore*)MemAlloc(m_TileStoreCount*sizeof(sTileStore),"TileStoreList");

			sTileStore	*Ptr=m_TileStore;
			for (Y=0; Y<MapH; Y++)
			{
				for (X=0; X<MapW; X++)
				{
					u8	*ColElem=CollisionLayer->getMapPtr(X*16,Y*16);
					int	ColT = (*ColElem) & COLLISION_TYPE_MASK;
					if ( ColT==COLLISION_TYPE_FLAG_DESTRUCTABLE_FLOOR || ColT==COLLISION_TYPE_FLAG_DESTRUCTABLE_WALL || ColT==COLLISION_TYPE_FLAG_SLIPPERY)
					{
						Ptr->X=X;
						Ptr->Y=Y;
						Ptr->Col=*ColElem;
						Ptr->Tile=*TileLayers[CLayerTile::LAYER_TILE_TYPE_ACTION]->getMapPtr(X*16,Y*16);
						Ptr++;
					}
				}
			}
		}
}

/*****************************************************************************/
void	CLevel::ApplyTileStore()
{
sTileStore	*Ptr=m_TileStore;

		for (int i=0; i<m_TileStoreCount; i++)
		{
			u8				*ColElem=CollisionLayer->getMapPtr(Ptr->X*16,Ptr->Y*16);
			sTileMapElem	*MapElem=TileLayers[CLayerTile::LAYER_TILE_TYPE_ACTION]->getMapPtr(Ptr->X*16,Ptr->Y*16);

			*ColElem=Ptr->Col;
			*MapElem=Ptr->Tile;
			Ptr++;
		}
}
