/*******************/
/*** Level Class ***/
/*******************/

#ifndef __LEVEL_LEVEL_H__
#define __LEVEL_LEVEL_H__

#include "system\global.h"
#include "level/layertile.h"
#include "level/layercollision.h"

/*****************************************************************************/
class CLayer;
class CLevel
{
public:
// Scene Handlers
	void		init();
	void		shutdown();
	void		render();
	void		think(int _frames);

	void					setCameraCentre(DVECTOR _pos)		{MapPos=_pos;}
	static DVECTOR			getCameraPos()						{return MapPos;}
	static DVECTOR			getPlayerSpawnPos()					{return s_playerSpawnPos;}
	CLayerCollision			*getCollisionLayer()				{return CollisionLayer;}
	DVECTOR					getMapSize();

private:
	void		initLayers();

	sLvlHdr			*LevelHdr;
	sTile			*TileBank;
	static DVECTOR	MapPos;
	static DVECTOR	s_playerSpawnPos;

	TPAGE_DESC		m_levelTPage;


// Tile Layers
	CLayerTile		*TileLayers[CLayerTile::LAYER_TILE_TYPE_MAX];
// Collision
	CLayerCollision	*CollisionLayer;
};

/*****************************************************************************/

#endif