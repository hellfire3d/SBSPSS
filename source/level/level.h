/*******************/
/*** Level Class ***/
/*******************/

#ifndef __LEVEL_LEVEL_H__
#define __LEVEL_LEVEL_H__

#include "system\global.h"
#include "level/layertile.h"
#include "level/layercollision.h"

/*****************************************************************************/
// Woo.. this is getting big now isn't it?
// Nope.. it's shrunk again! :)
struct	sLvlTab
{
	u16						Chapter,Level;
	u8						totalSpatCount;
	FileEquate				LevelFilename,TexFilename;
	int						songId;
};

/*****************************************************************************/
class CLayer;
class CLevel
{
public:
	CLevel();
// Scene Handlers
		void		init(int LevelNo);
		void		shutdown();
		void		render();
		void		think(int _frames);

		void				setCameraCentre(DVECTOR _pos)		{MapPos=_pos;}
static	DVECTOR const		&getCameraPos()						{return MapPos;}
static	DVECTOR const		&getPlayerSpawnPos()				{return s_playerSpawnPos;}

static int					getCurrentChapter();
static int					getCurrentChapterLevel();
static int					getTotalSpatCount();

		int					getActorCount()						{return ActorCount;}
		sThingActor			**getActorList()					{return ActorList;}
		int					getPlatformCount()					{return PlatformCount;}
		sThingPlatform		**getPlatformList()					{return PlatformList;}
		int					getHazardCount()					{return HazardCount;}
		sThingHazard		**getHazardList()					{return HazardList;}

		CLayerCollision		*getCollisionLayer()				{return CollisionLayer;}
		DVECTOR				getMapSize();

		bool				GetNextLevel(int &Lvl);

		void				respawnLevel();

static u8					getIsBossRespawn()					{return m_isBossRespawn;}
static s32					getBossHealth()						{return m_bossHealth;}
static void					setIsBossRespawn( u8 newIsBossRespawn )		{m_isBossRespawn=newIsBossRespawn;}
static void					setBossHealth( s32 newBossHealth )			{m_bossHealth=newBossHealth;}

	
private:
	void					initLayers();
	void					initThings(int _respawningLevel);

	void					DisplayLoadingScreen(sLvlTab *lvlTab);

	sLevelHdr				*LevelHdr;

	static DVECTOR	MapPos;
	static DVECTOR	s_playerSpawnPos;

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

	static u8		m_isBossRespawn;
	static s32		m_bossHealth;

};

/*****************************************************************************/

#endif