/****************/
/*** Layer 3d ***/
/****************/

#include	<Davelib.h>
#include	<List2d.h>

#include	"..\MkLevel.h"
#include	"MkLevelLayer3d.h"

/*****************************************************************************/
/*****************************************************************************/
/*** Pre-Process *************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CMkLevelLayer3d::PreProcess(CMkLevel *Core)
{
int			Width=InMap.GetWidth();
int			Height=InMap.GetHeight();

		OutMap.SetSize(Width,Height);

		for (int Y=0; Y<Height; Y++)
		{
			for (int X=0; X<Width; X++)
			{
				sExpLayerTile	&InElem=InMap.Get(X,Y);
				sMkLevelElem	&OutElem=OutMap.Get(X,Y);

				OutElem.Elem=0;
				if (InElem.Tile)
				{ // Not Blank Tile
					OutElem.Elem=Core->AddTile3d(InElem);
				}
			}
		}
}

