/*********************/
/*** Layer Trigger ***/
/*********************/

#ifndef	__MKLEVEL_LAYER_TRIGGER_HEADER__
#define	__MKLEVEL_LAYER_TRIGGER_HEADER__

#include	"MkLevelLayerThing.h"
#include	<List2d.h>

/*****************************************************************************/
class	CMkLevelLayerTrigger : public CMkLevelLayerThing
{
public:
		CMkLevelLayerTrigger(sExpLayerHdr *LayerHdr) : CMkLevelLayerThing(LayerHdr){};
const	char	*GetTypeName()		{return("TRIGGER");}

		void	PreProcess(CMkLevel *Core);
		void	Process(CMkLevel *Core);
		int		Write(FILE *File,const char *LayerName,const char *MapName);

};

/*****************************************************************************/
#endif
