/*******************/
/*** Level Class ***/
/*******************/

#ifndef __LEVEL_LEVEL_H__
#define __LEVEL_LEVEL_H__

#include	"level/layertile.h"

/*****************************************************************************/
class	CLayer;
class CLevel
{
public:
			CLevel();
	virtual ~CLevel();

// Scene Handlers
	void		init();
	void		shutdown();
	void		render();
	void		think(int _frames);

private:
	void		initLayers();

	sLvlHdr		*LevelHdr;

// Tile Layers
	CLayerTile	*TileLayers[CLayerTile::LAYER_TILE_TYPE_MAX];
};

/*****************************************************************************/

#endif