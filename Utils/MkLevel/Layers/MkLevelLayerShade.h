/*******************/
/*** Layer Shade ***/
/*******************/

#ifndef	__MKLEVEL_LAYER_SHADE_HEADER__
#define	__MKLEVEL_LAYER_SHADE_HEADER__

#include	"MkLevelLayer.h"
#include	<List2d.h>

/*****************************************************************************/
struct	SMkLayerShadeRGB
{
		int	Pos;
		u8	RGB[3];
};

/*****************************************************************************/
struct  sBackGfxList
{
		GString		Name;
		int			TexID;

bool	operator ==(sBackGfxList const &v1)		{return(Name==v1.Name);}
};

/*****************************************************************************/
class	CMkLevelLayerShade : public CMkLevelLayer
{
public:
		CMkLevelLayerShade(sExpLayerHdr *LayerHdr);

		void	PreProcess(CMkLevel *Core);
		void	Process(CMkLevel *Core);
		int		Write(FILE *File,const char *LayerName,const char *MapName);
		int		AddBackGfx(CMkLevel *Core,const char *Filename);

protected:
			int		Count;
			vector<SMkLayerShadeRGB>	List;
			GString						BackGfx[2];
			int							Flags[2];
			int							Trans[2];

			CList<sBackGfxList>			BackGfxList;
			int							TexID[2];
			sLayerShadeHdr				Data;
};

/*****************************************************************************/
#endif
