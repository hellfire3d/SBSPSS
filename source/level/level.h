/*******************/
/*** Level Class ***/
/*******************/

#ifndef __LEVEL_LEVEL_H__
#define __LEVEL_LEVEL_H__

#ifndef __FMA_FMA_H__
#include "fma\fma.h"
#endif

#include "system\global.h"
#include "level/layertile.h"
#include "level/layercollision.h"

#ifndef __HAZARD_HCHECK_H__
#include "hazard\hcheck.h"
#endif

/*****************************************************************************/
struct	sLvlTab
{
	u16								Chapter,Level;
	s8								SBFacing;
	u8								totalSpatCount;
	FileEquate						LevelFilename,TexFilename;
	int								songId;
	u16								ChapterLoadingText,LevelLoadingText;
	CFmaScene::FMA_SCRIPT_NUMBER	FMAToFollow;
};

/*****************************************************************************/
extern int		s_globalLevelSelectThing;
extern sLvlTab	LvlTable[];

/*****************************************************************************/
struct	sTileStore
{
	u16				X,Y;
	sTileMapElem	Tile;
	u8				Col;
};

/*****************************************************************************/
class CLayer;
class CLevel
{
public:
	CLevel();
// Scene Handlers
		void			init(int LevelNo);
		void			shutdown();
		void			render();
		void			think(int _frames);

		void			setCameraCentre(DVECTOR const &_pos)	{MapPos=_pos;}
static	DVECTOR const	&getCameraPos()							{return MapPos;}
static	DVECTOR const	&getPlayerSpawnPos()					{return s_playerSpawnPos;}
static	int				getPlayerStartFacing()					{return s_playerFacing;}

static int							getCurrentChapter()						{return LvlTable[s_globalLevelSelectThing].Chapter;}
static int							getCurrentChapterLevel()				{return LvlTable[s_globalLevelSelectThing].Level;}
static int							getTotalSpatCount()						{return LvlTable[s_globalLevelSelectThing].totalSpatCount;}
static int							getChapterLoadingText()					{return LvlTable[s_globalLevelSelectThing].ChapterLoadingText;}
static int							getLevelLoadingText()					{return LvlTable[s_globalLevelSelectThing].LevelLoadingText;}
static CFmaScene::FMA_SCRIPT_NUMBER getFMAToFollow()						{return LvlTable[s_globalLevelSelectThing].FMAToFollow;}

		void			destroyMapArea(DVECTOR const &Pos,int ColT=COLLISION_TYPE_DESTRUCTABLE_WALL);
		void			destroyMapTile(DVECTOR const &Pos,int ColT=COLLISION_TYPE_DESTRUCTABLE_FLOOR);

		void			reverseMapConveyor();

		int				getActorCount()							{return ActorCount;}
		sThingActor		**getActorList()						{return ActorList;}
		int				getPlatformCount()						{return PlatformCount;}
		sThingPlatform	**getPlatformList()						{return PlatformList;}
		int				getHazardCount()						{return HazardCount;}
		sThingHazard	**getHazardList()						{return HazardList;}

		CLayerCollision	*getCollisionLayer()					{return CollisionLayer;}

		DVECTOR	 const	&getMapSize()							{return(MapSize);}
		int				getMapWidth()							{return(MapSize.vx);}
		int				getMapHeight()							{return(MapSize.vy);}

		DVECTOR	 const	&getMapSize16()							{return(MapSize16);}
		int				getMapWidth16()							{return(MapSize16.vx);}
		int				getMapHeight16()						{return(MapSize16.vy);}

		bool			GetNextLevel(int &Lvl);

		void			respawnLevel();
static	sLevelHdr		*getLevelHdr()							{return(LevelHdr);}

static u8				getIsBossRespawn()						{return m_isBossRespawn;}
static s32				getBossHealth()							{return m_bossHealth;}
static void				setIsBossRespawn( u8 newIsBossRespawn)	{m_isBossRespawn=newIsBossRespawn;}
static void				setBossHealth( s32 newBossHealth )		{m_bossHealth=newBossHealth;}

static CNpcCheckpointHazard		*getCurrentCheckpoint()											{return( m_checkpoint );}
static void						setCurrentCheckpoint( CNpcCheckpointHazard *newCheckpoint )		{m_checkpoint = newCheckpoint;}

static	void			DisplayLoadingScreen(int LevelNo=-1);

private:
		void			initLayers();
		void			initThings(int _respawningLevel);

static	sLevelHdr		*LevelHdr;

static	DVECTOR			MapPos;
static	DVECTOR			s_playerSpawnPos;
static	int				s_playerFacing;
		TPAGE_DESC		m_levelTPage;

// Tile Layers
		CLayerTile		*TileLayers[CLayerTile::LAYER_TILE_TYPE_MAX];

// Collision
		CLayerCollision	*CollisionLayer;

// Things
		int				ActorCount;
		sThingActor		**ActorList;
		int				ItemCount;
		sThingItem		*ItemList;
		int				PlatformCount;
		sThingPlatform	**PlatformList;
		int				TriggerCount;
		sThingTrigger	*TriggerList;
		int				FXCount;
		sThingFX		*FXList;
		int				HazardCount;
		sThingHazard	**HazardList;

		u8				*m_RGBMap;
		u8				*m_RGBTable;

static	u8				m_isBossRespawn;
static	s32				m_bossHealth;

		DVECTOR			MapSize,MapSize16;
static	CNpcCheckpointHazard	*m_checkpoint;

// Level Repair stuff
		void			CreateTileStore();
		void			ApplyTileStore();
		int				m_TileStoreCount;
		sTileStore		*m_TileStore;
};

/*****************************************************************************/

#endif