/********************/
/*** Layer Hazard ***/
/********************/

#ifndef	__MKLEVEL_LAYER_HAZARD_HEADER__
#define	__MKLEVEL_LAYER_HAZARD_HEADER__

#include	"MkLevelLayerThing.h"
#include	<List2d.h>

/*****************************************************************************/
class	CMkLevelLayerHazard : public CMkLevelLayerThing
{
public:
		CMkLevelLayerHazard(sExpLayerHdr *LayerHdr) : CMkLevelLayerThing(LayerHdr){};
const	char	*GetTypeName()		{return("HAZARD");}

		void	PreProcess(CMkLevel *Core);
		void	Process(CMkLevel *Core);
		int		Write(CMkLevel *Core,FILE *File,const char *LayerName);

		CList<int>	RemapTable;

};

/*****************************************************************************/
#endif
