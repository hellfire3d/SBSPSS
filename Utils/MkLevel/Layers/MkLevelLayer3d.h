/****************/
/*** Layer 3d ***/
/****************/

#ifndef	__MKLEVEL_LAYER_3D__HEADER__
#define	__MKLEVEL_LAYER_3D__HEADER__

#include	"MkLevelLayerTile.h"
#include	<List2d.h>

/*****************************************************************************/
class	CMkLevelLayer3d : public CMkLevelLayerTile
{
public:
		CMkLevelLayer3d(sExpLayerHdr *LayerHdr) : CMkLevelLayerTile(LayerHdr){};

		void	PreProcess(CMkLevel *Core);
protected:

};

/*****************************************************************************/
#endif
