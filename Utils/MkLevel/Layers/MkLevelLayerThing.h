/*******************/
/*** Layer Thing ***/
/*******************/

#ifndef	__MKLEVEL_LAYER_THING_HEADER__
#define	__MKLEVEL_LAYER_THING_HEADER__

#include	"MkLevelLayer.h"

struct	sPoint
{
	int	x;
	int	y;
inline	operator=(sPoint const &Src)	{x=Src.x;y=Src.y;}
};

struct	sMkLevelLayerThing 
{
	GString					Name;
	int						Type;
	vector<sPoint>			XY;
	sLayerThingData			Data;
};

/*****************************************************************************/
class	CMkLevelLayerThing : public CMkLevelLayer
{
public:
		CMkLevelLayerThing(sExpLayerHdr *LayerHdr);
virtual	const char	*GetTypeName()=0;

virtual	void	PreProcess(CMkLevel *Core)=0;
virtual	void	Process(CMkLevel *Core)=0;
virtual	int		Write(FILE *File,const char *LayerName,const char *MapName)=0;

protected:
		u8		*LoadThing(sMkLevelLayerThing &ThisThing,u8 *Ptr);
		void	LoadThingNames(char *Ptr);
		void	ProcessList();

		CList<sMkLevelLayerThing>	ThingList;
		
};

/*****************************************************************************/
#endif
