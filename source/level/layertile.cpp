/************************/
/*** Tile Layer Class ***/
/************************/

#include 	"system\global.h"
#include	<DStructs.h>

#include	"Layer.h"
#include	"LayerTile.h"

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
int		CLayerTile::GetLayerType(int SubType)
{
	switch(SubType)
	{
		case CLayerTile::LAYER_TILE_SUBTYPE_BACK:	return(LAYER_TYPE_TILE_BACK);
		case CLayerTile::LAYER_TILE_SUBTYPE_MID:	return(LAYER_TYPE_TILE_MID);
		case CLayerTile::LAYER_TILE_SUBTYPE_ACTION: return(LAYER_TYPE_TILE_ACTION);
		case CLayerTile::LAYER_TILE_SUBTYPE_FORE:	return(LAYER_TYPE_TILE_FORE);
	}
	return(-1);
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CLayerTile::init()
{
}

/*****************************************************************************/
void	CLayerTile::shutdown()
{
}

/*****************************************************************************/
void	CLayerTile::render()
{
}

/*****************************************************************************/
void	CLayerTile::think(int _frames)
{
}

