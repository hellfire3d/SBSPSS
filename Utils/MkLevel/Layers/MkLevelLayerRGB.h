/*****************/
/*** Layer RGB ***/
/*****************/

#ifndef	__MKLEVEL_LAYER_RGB_HEADER__
#define	__MKLEVEL_LAYER_RGB_HEADER__

#include	"MkLevelLayer.h"
#include	<List2d.h>

struct	sRGBElem
{
	sRGBCol	RGB;
	int		TableIdx;
};

struct	sRGBList
{
	sRGBCol	RGB;
	int		Count;
bool	operator==(sRGBList const &v1)	{return(RGB==v1.RGB);}

};

/*****************************************************************************/
class	CMkLevelLayerRGB : public CMkLevelLayer
{
public:
		CMkLevelLayerRGB(sExpLayerHdr *LayerHdr);

		void	PreProcess(CMkLevel *Core);
		void	Process(CMkLevel *Core);
		int		Write(CMkLevel *Core,FILE *File,const char *LayerName);

protected:
		int		WriteRGBMap(FILE *File);
		int		WriteRGBTable(FILE *File);
		int		FindClosestRGB(sRGBCol const &RGB);

		CList2d<sRGBElem>		RGBMap;
		sRGBCol					ShadeRGB;

		CList2d<u8>				OutMap;
		CList2d<sRGBCol>		OutRGBTable;
};

/*****************************************************************************/
#endif
