/******************/
/*** Layer Item ***/
/******************/

#ifndef	__MKLEVEL_LAYER_ITEM_HEADER__
#define	__MKLEVEL_LAYER_ITEM_HEADER__

#include	"MkLevelLayerThing.h"
#include	<List2d.h>

/*****************************************************************************/
class	CMkLevelLayerItem : public CMkLevelLayerThing
{
public:
		CMkLevelLayerItem(sExpLayerHdr *LayerHdr) : CMkLevelLayerThing(LayerHdr){};
const	char	*GetTypeName()		{return("ITEM");}

		void	PreProcess(CMkLevel *Core);
		void	Process(CMkLevel *Core);
		int		Write(CMkLevel *Core,FILE *File,const char *LayerName);

};

/*****************************************************************************/
#endif
