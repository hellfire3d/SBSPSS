/**************/
/*** Export ***/
/**************/

#include	"stdafx.h"
#include	<Vector>

#include	"Core.h"
#include	"TileSet.h"
#include	"Map.h"

#include	"Export.h"
#include	"utils.h"

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
CExport::CExport(char *_Filename)
{
		Filename=_Filename;
}

/*****************************************************************************/
CExport::~CExport()
{
		fclose(File);
}

/*****************************************************************************/
void	CExport::BuildColTable(CTile &ThisTile)
{
u8		*RGB=ThisTile.GetGBTexRGB();
int		Width=ThisTile.GetGBTexW();
int		Height=ThisTile.GetGBTexH();

int		X,Y;
		ColTable.resize(Width);

		for (X=0; X<Width; X++)
		{
			for (Y=Height; Y ; Y--)
			{
				int	Ofs=X+((Y-1)*Width);
				u8	R=RGB[(Ofs*3)+0];
				u8	G=RGB[(Ofs*3)+1];
				u8	B=RGB[(Ofs*3)+2];
				if (R==255 && G==255 && B==255) break;
			}
			ColTable[X]=Y-1;
		}

}