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
//		void	Process(CMkLevel *Core);
//		int		Write(FILE *File,const char *LayerName,const char *MapName);
protected:
//		void	ProcessVtxList(vector<sVtx> const &In,vector<sVtx> &Out);
};

/*****************************************************************************/
#endif
