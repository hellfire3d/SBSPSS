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

	void		setCameraCentre(DVECTOR _pos)		{MapPos=_pos;}

private:
	void		initLayers();

	sLvlHdr		*LevelHdr;
	DVECTOR		MapPos;


// Tile Layers
	CLayerTile	*TileLayers[CLayerTile::LAYER_TILE_TYPE_MAX];
};

/*****************************************************************************/

#endif