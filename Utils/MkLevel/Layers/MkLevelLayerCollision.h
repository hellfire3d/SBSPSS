/***********************/
/*** Layer Collision ***/
/***********************/

#ifndef	__MKLEVEL_LAYER_COLLISION_HEADER__
#define	__MKLEVEL_LAYER_COLLISION_HEADER__

#include	"MkLevelLayer.h"
#include	<List2d.h>

/*****************************************************************************/
class	CMkLevelLayerCollision: public CMkLevelLayer
{
public:
		CMkLevelLayerCollision(sExpLayerHdr *LayerHdr);

virtual	void	PreProcess(CMkLevel *Core);
virtual	void	Process(CMkLevel *Core);
virtual	int		Write(FILE *File,const char *LayerName,const char *MapName);

protected:

		CList2d<sExpColTile>	Map;
};

/*****************************************************************************/
#endif
