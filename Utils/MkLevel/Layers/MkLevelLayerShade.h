/*******************/
/*** Layer Shade ***/
/*******************/

#ifndef	__MKLEVEL_LAYER_SHADE_HEADER__
#define	__MKLEVEL_LAYER_SHADE_HEADER__

#include	"MkLevelLayer.h"
#include	<List2d.h>

/*****************************************************************************/
struct  sBackGfxList
{
		GString					Name;
		int						TexID;
		sLayerShadeBackGfxType	Out;

bool	operator ==(sBackGfxList const &v1)		{return(Name==v1.Name);}
};

/*****************************************************************************/
class	CMkLevelLayerShade : public CMkLevelLayer
{
public:
		CMkLevelLayerShade(sExpLayerHdr *LayerHdr);

		void	PreProcess(CMkLevel *Core);
		void	Process(CMkLevel *Core);
		int		Write(CMkLevel *Core,FILE *File,const char *LayerName);

protected:
		int		WriteTypeList(FILE *File);
		int		WriteGfxList(FILE *File);

		CList<GString>			TypeNameList;
		CList<sLayerShadeGfx>	GfxList;

		CList<sBackGfxList>		OutTypeList;
		sLayerShadeHdr			ShadeHdr;
};

/*****************************************************************************/
#endif
