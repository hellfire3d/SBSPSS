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

	void					setCameraCentre(DVECTOR _pos)		{MapPos=_pos;}
	static DVECTOR			getCameraPos()						{return MapPos;}
	static DVECTOR			getPlayerSpawnPos()					{return s_playerSpawnPos;}
	int						getActorCount()						{return ActorCount;}
	sThingActor				**getActorList()					{return ActorList;}
	int						getPlatformCount()					{return PlatformCount;}
	sThingPlatform			**getPlatformList()					{return PlatformList;}
	CLayerCollision			*getCollisionLayer()				{return CollisionLayer;}
	DVECTOR					getMapSize();

	int						GetNextLevel(int Lvl);
private:
	void					initLayers();
	void					DisplayLoadingScreen(sLvlTab *lvlTab);

	sLvlHdr			*LevelHdr;

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

};

/*****************************************************************************/

#endif