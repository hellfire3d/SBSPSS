/*******************/
/*** Layer Actor ***/
/*******************/

#ifndef	__MKLEVEL_LAYER_ACTOR_HEADER__
#define	__MKLEVEL_LAYER_ACTOR_HEADER__

#include	"MkLevelLayerThing.h"
#include	<List2d.h>

/*****************************************************************************/
class	CMkLevelLayerActor : public CMkLevelLayerThing
{
public:
		CMkLevelLayerActor(sExpLayerHdr *LayerHdr) : CMkLevelLayerThing(LayerHdr){};
const	char	*GetTypeName()		{return("ACTOR");}

		void	PreProcess(CMkLevel *Core);
		void	Process(CMkLevel *Core);
		int		Write(CMkLevel *Core,FILE *File,const char *LayerName);

};

/*****************************************************************************/
#endif
