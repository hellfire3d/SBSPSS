/**********************/
/*** Layer Platform ***/
/**********************/

#ifndef	__MKLEVEL_LAYER_PLATFORM_HEADER__
#define	__MKLEVEL_LAYER_PLATFORM_HEADER__

#include	"MkLevelLayerThing.h"
#include	<List2d.h>

/*****************************************************************************/
class	CMkLevelLayerPlatform : public CMkLevelLayerThing
{
public:
		CMkLevelLayerPlatform(sExpLayerHdr *LayerHdr) : CMkLevelLayerThing(LayerHdr){};
const	char	*GetTypeName()		{return("PLATFORM");}

		void	PreProcess(CMkLevel *Core);
		void	Process(CMkLevel *Core);
		int		Write(CMkLevel *Core,FILE *File,const char *LayerName);

		CList<int>	RemapTable;

};

/*****************************************************************************/
#endif
