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

#ifndef __PICKUPS_PICKUP_H__
#include "pickups\pickup.h"
#endif

#ifndef	__PICKUPS_PSPATULA_H__
#include "pickups\pspatula.h"
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

#include	"fx\fx.h"
#include	"fx\fxfallingtile.h"


/*****************************************************************************/
 sLvlTab	LvlTable[]=
{
// C1
	{1,1,	21,	LEVELS_CHAPTER01_LEVEL01_LVL,LEVELS_CHAPTER01_LEVEL01_TEX,	CSoundMediator::SONG_CHAPTER1_LEVEL1},
	{1,2,	63,	LEVELS_CHAPTER01_LEVEL02_LVL,LEVELS_CHAPTER01_LEVEL02_TEX,	CSoundMediator::SONG_CHAPTER1_LEVEL2},
	{1,3,	21,	LEVELS_CHAPTER01_LEVEL03_LVL,LEVELS_CHAPTER01_LEVEL03_TEX,	CSoundMediator::SONG_CHAPTER1_LEVEL3},
	{1,4,	20,	LEVELS_CHAPTER01_LEVEL04_LVL,LEVELS_CHAPTER01_LEVEL04_TEX,	CSoundMediator::SONG_CHAPTER1_LEVEL4},

// C2
	{2,1,	20,	LEVELS_CHAPTER02_LEVEL01_LVL,LEVELS_CHAPTER02_LEVEL01_TEX,	CSoundMediator::SONG_CHAPTER2_LEVEL1},
	{2,2,	20,	LEVELS_CHAPTER02_LEVEL02_LVL,LEVELS_CHAPTER02_LEVEL02_TEX,	CSoundMediator::SONG_CHAPTER2_LEVEL2},
	{2,3,	20,	LEVELS_CHAPTER02_LEVEL03_LVL,LEVELS_CHAPTER02_LEVEL03_TEX,	CSoundMediator::SONG_CHAPTER2_LEVEL3},
	{2,4,	20,	LEVELS_CHAPTER02_LEVEL04_LVL,LEVELS_CHAPTER02_LEVEL04_TEX,	CSoundMediator::SONG_CHAPTER2_LEVEL4},
// C3
	{3,1,	20,	LEVELS_CHAPTER03_LEVEL01_LVL,LEVELS_CHAPTER03_LEVEL01_TEX,	CSoundMediator::SONG_CHAPTER3_LEVEL1},
	{3,2,	20,	LEVELS_CHAPTER03_LEVEL02_LVL,LEVELS_CHAPTER03_LEVEL02_TEX,	CSoundMediator::SONG_CHAPTER3_LEVEL2},
	{3,3,	20,	LEVELS_CHAPTER03_LEVEL03_LVL,LEVELS_CHAPTER03_LEVEL03_TEX,	CSoundMediator::SONG_CHAPTER3_LEVEL3},
	{3,4,	20,	LEVELS_CHAPTER03_LEVEL04_LVL,LEVELS_CHAPTER03_LEVEL04_TEX,	CSoundMediator::SONG_CHAPTER3_LEVEL4},
// C4
	{4,1,	20,	LEVELS_CHAPTER04_LEVEL01_LVL,LEVELS_CHAPTER04_LEVEL01_TEX,	CSoundMediator::SONG_CHAPTER4_LEVEL1},
	{4,2,	20,	LEVELS_CHAPTER04_LEVEL02_LVL,LEVELS_CHAPTER04_LEVEL02_TEX,	CSoundMediator::SONG_CHAPTER4_LEVEL2},
	{4,3,	20,	LEVELS_CHAPTER04_LEVEL03_LVL,LEVELS_CHAPTER04_LEVEL03_TEX,	CSoundMediator::SONG_CHAPTER4_LEVEL3},
	{4,4,	20,	LEVELS_CHAPTER04_LEVEL04_LVL,LEVELS_CHAPTER04_LEVEL04_TEX,	CSoundMediator::SONG_CHAPTER4_LEVEL4},
// C5
	{5,1,	20,	LEVELS_CHAPTER05_LEVEL01_LVL,LEVELS_CHAPTER05_LEVEL01_TEX,	CSoundMediator::SONG_CHAPTER5_LEVEL1},
	{5,2,	20,	LEVELS_CHAPTER05_LEVEL02_LVL,LEVELS_CHAPTER05_LEVEL02_TEX,	CSoundMediator::SONG_CHAPTER5_LEVEL2},
	{5,3,	20,	LEVELS_CHAPTER05_LEVEL03_LVL,LEVELS_CHAPTER05_LEVEL03_TEX,	CSoundMediator::SONG_CHAPTER5_LEVEL3},
	{5,4,	20,	LEVELS_CHAPTER05_LEVEL04_LVL,LEVELS_CHAPTER05_LEVEL04_TEX,	CSoundMediator::SONG_CHAPTER5_LEVEL4},
// C6
	{6,1,	20,	LEVELS_CHAPTER06_LEVEL01_LVL,LEVELS_CHAPTER06_LEVEL01_TEX,	CSoundMediator::SONG_CHAPTER6_ROLLERCOASTER},
	{6,2,	20,	LEVELS_CHAPTER06_LEVEL02_LVL,LEVELS_CHAPTER06_LEVEL02_TEX,	CSoundMediator::SONG_CHAPTER6_SNAILRACE},
	{6,3,	20,	LEVELS_CHAPTER06_LEVEL03_LVL,LEVELS_CHAPTER06_LEVEL03_TEX,	CSoundMediator::SONG_CHAPTER6_BIGWHEEL},
	{6,4,	20,	LEVELS_CHAPTER06_LEVEL04_LVL,LEVELS_CHAPTER06_LEVEL04_TEX,	CSoundMediator::SONG_CHAPTER6_TUNNELOFLOVE},
	{6,5,	20,	LEVELS_CHAPTER06_LEVEL05_LVL,LEVELS_CHAPTER06_LEVEL05_TEX,	CSoundMediator::SONG_CHAPTER6_TUNNELOFLOVE},
// FMA
	{6,99,	0,	LEVELS_FMA_SHADYSHOALS_LVL,LEVELS_FMA_SHADYSHOALS_TEX,		CSoundMediator::SONG_TITLE},

};
const	int		LvlTableSize=sizeof(LvlTable)/sizeof(sLvlTab);

const FileEquate	loadingScreens[6]=
{
	LOADINGSCREENS_PINEAPPLE_GFX,
	LOADINGSCREENS_CULTURE_GFX,
	LOADINGSCREENS_PICKLES_GFX,
	LOADINGSCREENS_MONITOR_GFX,
	LOADINGSCREENS_KARATE_GFX,
	LOADINGSCREENS_PIZZA_GFX,
};

/*****************************************************************************/
DVECTOR		CLevel::MapPos;
DVECTOR		CLevel::s_playerSpawnPos;
sLevelHdr	*CLevel::LevelHdr;

u8 CLevel::m_isBossRespawn;
s32 CLevel::m_bossHealth;

/*****************************************************************************/
CLevel::CLevel()
{
}

/*****************************************************************************/
void 	CLevel::init(int LevelNo)
{
// Load it
sLvlTab *lvlTab=&LvlTable[LevelNo];

		CActorPool::Reset();
		CSoundMediator::setSong((CSoundMediator::SONGID)lvlTab->songId);

		DisplayLoadingScreen(lvlTab);

		LevelHdr=(sLevelHdr*)CFileIO::loadFile(lvlTab->LevelFilename,"Level");
		LevelHdr->TileBank2d=(sTile2d*)	MakePtr(LevelHdr,(int)LevelHdr->TileBank2d);
		LevelHdr->TileBank3d=(sTile3d*)	MakePtr(LevelHdr,(int)LevelHdr->TileBank3d);
		LevelHdr->TriList=(sTri*)		MakePtr(LevelHdr,(int)LevelHdr->TriList);
		LevelHdr->QuadList=(sQuad*)		MakePtr(LevelHdr,(int)LevelHdr->QuadList);
		LevelHdr->VtxList=(sVtx*)		MakePtr(LevelHdr,(int)LevelHdr->VtxList);
		LevelHdr->ModelList=(sModel*)	MakePtr(LevelHdr,(int)LevelHdr->ModelList);

		CModelGfx::SetData(LevelHdr->ModelList,LevelHdr->TriList,LevelHdr->QuadList,LevelHdr->VtxList);
		m_levelTPage=TPLoadTex(lvlTab->TexFilename);

		s_playerSpawnPos.vx=LevelHdr->PlayerStartX*16;
		s_playerSpawnPos.vy=LevelHdr->PlayerStartY*16+16;

		MapPos.vx=MapPos.vy=0;
		initLayers();

		m_isBossRespawn = false;
		m_bossHealth = 0;
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
// End TMP
/*** E3 Bodge ***/
#if !defined(__USER_sbart__)
		if (Lvl>2) Lvl=0;
#endif
/****************/
		return(Finished);
}

/*****************************************************************************/
void	CLevel::DisplayLoadingScreen(sLvlTab *lvlTab)
{
FontBank	font;
char		buf[128];
u8			*s_image;
int			i;

			font.initialise(&standardFont);
			font.setJustification(FontBank::JUST_CENTRE);
			sprintf(buf,"LOADING CHAPTER %d LEVEL %d",lvlTab->Chapter,lvlTab->Level);
			s_image=CFileIO::loadFile(loadingScreens[lvlTab->Chapter-1]);
			ASSERT(s_image);
			SetScreenImage(s_image);
			for(i=0;i<2;i++)
			{
				font.setColour(255,255,255);
				font.print(256  ,120  ,buf);
				font.setColour(0,0,0);
				font.print(256-1,120-1,buf);
				font.print(256-1,120  ,buf);
				font.print(256-1,120+1,buf);
				font.print(256  ,120-1,buf);
				font.print(256  ,120+1,buf);
				font.print(256+1,120-1,buf);
				font.print(256+1,120  ,buf);
				font.print(256+1,120+1,buf);
				PrimDisplay();
				VSync(0);
				VidSwapDraw();
			}
			ClearScreenImage();
			MemFree(s_image);
			VSync(20);
			font.dump();
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
			CLayerTile *NewLayer=new ("Action Layer") CLayerTile3d(LevelHdr,Layer);
			NewLayer->init(MapPos,0);
			TileLayers[CLayerTile::LAYER_TILE_TYPE_ACTION]=NewLayer;
		}
// Collision

		if (LevelHdr->CollisionLayer) 
		{
			sLayerHdr	*Layer=(sLayerHdr*)MakePtr(LevelHdr,LevelHdr->CollisionLayer);
			CollisionLayer=new ("Collision Layer") CLayerCollision(Layer);
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
	// Triggers
	// Yeah yeah - I'll put this crap into a seperate file when the numbers of trigger types get a bit bigger! (pkg)
	if (LevelHdr->TriggerList)
	{
		sThingHdr	*Hdr=(sThingHdr*)MakePtr(LevelHdr,LevelHdr->TriggerList);
		TriggerCount=Hdr->Count;
		TriggerList=(sThingTrigger*)MakePtr(Hdr,sizeof(sThingHdr));

		for(int i=0;i<TriggerCount;i++)
		{
			CTriggerThing	*trigger=NULL;		// I hate having to do this just to keep the compiler quiet :/ (pkg)
			if(TriggerList->Type==3)
			{
				// Camera lock trigger
				trigger=(CCameraLockTrigger*)new ("CameraLockTrigger") CCameraLockTrigger();
				trigger->init();
				trigger->setPositionAndSize(TriggerList->Pos.X<<4,TriggerList->Pos.Y<<4,
											TriggerList->Width<<4,TriggerList->Height<<4);
				trigger->setTargetBox(TriggerList->TargetPos.X<<4,TriggerList->TargetPos.Y<<4,TriggerList->TargetSize.X<<4,TriggerList->TargetSize.Y<<4);
			}
			TriggerList++;
		}
	}

	if (LevelHdr->TriggerList)
	{
		sThingHdr	*Hdr=(sThingHdr*)MakePtr(LevelHdr,LevelHdr->TriggerList);
		TriggerCount=Hdr->Count;
		TriggerList=(sThingTrigger*)MakePtr(Hdr,sizeof(sThingHdr));

		for(int i=0;i<TriggerCount;i++)
		{
			CTriggerThing	*trigger=NULL;		// I hate having to do this just to keep the compiler quiet :/ (pkg)
			switch(TriggerList->Type)
			{
				// Exit trigger
				case 0:
					trigger=(CTriggerThing*)new ("LevelExitTrigger") CLevelExitTrigger();
					break;

				// Level respawn trigger
				case 1:
					trigger=(CRestartPointTrigger*)new ("RestartTrigger") CRestartPointTrigger();
					break;

				// Teleport trigger
				case 2:
					trigger=(CTeleportTrigger*)new ("TeleportTrigger") CTeleportTrigger();
					break;

				// Camera lock trigger
				case 3:
					trigger=NULL;
					break;

				// In/Out of water triggers
				case 4:
					trigger=(CInWaterTrigger*)new ("InWaterTrigger") CInWaterTrigger();
					break;
				case 5:
					trigger=(COutOfWaterTrigger*)new ("OutOfWaterTrigger") COutOfWaterTrigger();
					break;
			}
			if(trigger)
			{
				trigger->init();
				trigger->setPositionAndSize(TriggerList->Pos.X<<4,TriggerList->Pos.Y<<4,
											TriggerList->Width<<4,TriggerList->Height<<4);
				trigger->setTargetBox(TriggerList->TargetPos.X<<4,TriggerList->TargetPos.Y<<4,TriggerList->TargetSize.X<<4,TriggerList->TargetSize.Y<<4);
			}
			TriggerList++;
		}
	}

// Items
		if (LevelHdr->ItemList)
		{
			sThingHdr	*Hdr=(sThingHdr*)MakePtr(LevelHdr,LevelHdr->ItemList);
			ItemCount=Hdr->Count;
			ItemList=(sThingItem*)MakePtr(Hdr,sizeof(sThingHdr));
			DVECTOR	pos;
			int		spatNumber=0;
			for(int i=0;i<ItemCount;i++)
			{
				int			createThisPickup;
				int			isSpat;
				CBasePickup	*newPickup;
				
				createThisPickup=true;

				isSpat=(PICKUP_TYPE)ItemList->Type==PICKUP__SPATULA;
				if(isSpat&&CGameSlotManager::getSlotData()->isSpatulaUncollected(GameScene.getChapterNumber(),GameScene.getLevelNumber(),spatNumber)==false)
				{
					createThisPickup=false;
				}

				if((PICKUP_TYPE)ItemList->Type==PICKUP__NET&&_respawningLevel)
				{
					createThisPickup=false;
				}

				if(createThisPickup)
				{
					pos.vx=ItemList->Pos.X<<4;
					pos.vy=ItemList->Pos.Y<<4;
					newPickup=createPickup((PICKUP_TYPE)ItemList->Type,&pos);
					if(isSpat)
					{
						((CSpatulaPickup*)newPickup)->setSpatulaNumber(spatNumber);
					}
				}
				if(isSpat)
				{
					spatNumber++;
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
						enemy->setLayerCollision( getCollisionLayer() );
						enemy->setupWaypoints( ThisActor );
						enemy->postInit();

						if ( ThisActor->Speed )
						{
							enemy->setSpeed( ThisActor->Speed );
						}
					}
					break;

				case CGameScene::ACTOR_FRIEND_NPC:
					{
						CNpcFriend *friendNpc;
						friendNpc=CNpcFriend::Create(ThisActor);
						friendNpc->setLayerCollision( getCollisionLayer() );
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
			platform->setLayerCollision( getCollisionLayer() );
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
			hazard->setLayerCollision( getCollisionLayer() );
			if ( ThisHazard->Respawn )
			{
				hazard->setRespawnRate( ThisHazard->Respawn );
			}
		}
	}

	// initialise jellyfish generator

	CJellyfishGenerator::init();
	CThingManager::initAllThings();
}



/*****************************************************************************/
void	CLevel::respawnLevel()
{
	m_isBossRespawn = false;
	m_bossHealth = 0;

	CThingManager::killAllThingsForRespawn();
	initThings(true);
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

		if (CollisionLayer)
		{
			CollisionLayer->shutdown();	
			delete CollisionLayer;
		}

		CSoundMediator::stopAllSound();
		VSync(0);CSoundMediator::think(1);		// This is needed to let xmplay kill off the sounds properly (pkg)

		if (ActorList) MemFree(ActorList);
		if (PlatformList) MemFree(PlatformList);
		if (HazardList) MemFree(HazardList);

		MemFree(LevelHdr);
		CActorPool::Reset();
		TPFree(m_levelTPage);
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
/*****************************************************************************/
/*****************************************************************************/
void	CLevel::destroyMapArea(DVECTOR const &Pos)
{
DVECTOR		TL,BR;
const int			ColT=COLLISION_TYPE_STICKY;


			TL.vx=Pos.vx&-16;;
			TL.vy=Pos.vy&-16;;
			BR=TL;

			if (CollisionLayer->getCollisionBlock(TL.vx,TL.vy)>>COLLISION_TYPE_FLAG_SHIFT==ColT)printf ("!!");

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

					if (*ColElem>>COLLISION_TYPE_FLAG_SHIFT==COLLISION_TYPE_STICKY)
					{
						sTileMapElem	*MapElem=TileLayers[CLayerTile::LAYER_TILE_TYPE_ACTION]->getMapPtr(DP.vx,DP.vy);
						CFXFallingTile	*FX=(CFXFallingTile*)CFX::Create(CFX::FX_TYPE_FALLINGTILE,DP);
						FX->SetTile(MapElem->Tile);
						MapElem->Tile=0;
						*ColElem=0;

					}

				}
			}
}

/*****************************************************************************/
void	CLevel::destroyMapTile(DVECTOR const &Pos)
{
u8				*ColElem=CollisionLayer->getMapPtr(Pos.vx,Pos.vy);
sTileMapElem	*MapElem=TileLayers[CLayerTile::LAYER_TILE_TYPE_ACTION]->getMapPtr(Pos.vx,Pos.vy);

//			*ColElem=0;

			if (MapElem->Tile)
			{
				CFXFallingTile	*FX=(CFXFallingTile*)CFX::Create(CFX::FX_TYPE_FALLINGTILE,Pos);
				FX->SetTile(MapElem->Tile);
				MapElem->Tile=0;

			}

}

