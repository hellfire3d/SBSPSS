/*******************/
/*** Level Class ***/
/*******************/

#ifndef __LEVEL_LEVEL_H__
#define __LEVEL_LEVEL_H__

#include	"level/layer.h"

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

	int			GetLayerCount()		{return(LevelHdr->LayerCount);}

	
private:
	void		LoadLayers();

	sLvlHdr		*LevelHdr;
	CLayer		*LayerList[CLayer::LAYER_TYPE_MAX];
};

/*****************************************************************************/

#endif