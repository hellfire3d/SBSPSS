/******************/
/*** Layer Tile ***/
/******************/

#ifndef	__MKLEVEL_LAYER_TILE_HEADER__
#define	__MKLEVEL_LAYER_TILE_HEADER__

#include	"MkLevelLayer.h"
#include	<List2d.h>

struct	sMkLevelElem
{
	u16				Elem;
	int				TexID;
};

/*****************************************************************************/
class	CMkLevelLayerTile : public CMkLevelLayer
{
public:
		CMkLevelLayerTile(sExpLayerHdr *LayerHdr);

virtual	void	PreProcess(CMkLevel *Core);
virtual	void	Process(CMkLevel *Core);
virtual	int		Write(FILE *File,const char *LayerName,const char *MapName);

protected:
		CList2d<sExpLayerTile>		InMap;
		CList2d<sMkLevelElem>		OutMap;

};

/*****************************************************************************/
#endif
