/****************/
/*** Layer FX ***/
/***************/

#ifndef	__MKLEVEL_LAYER_FX_HEADER__
#define	__MKLEVEL_LAYER_FX_HEADER__

#include	"MkLevelLayerThing.h"
#include	<List2d.h>

/*****************************************************************************/
class	CMkLevelLayerFX : public CMkLevelLayerThing
{
public:
		CMkLevelLayerFX(sExpLayerHdr *LayerHdr) : CMkLevelLayerThing(LayerHdr){};
		const	char	*GetTypeName()		{return("FX");}

		void	PreProcess(CMkLevel *Core);
		void	Process(CMkLevel *Core);
		int		Write(CMkLevel *Core,FILE *File,const char *LayerName);

};

/*****************************************************************************/
#endif
