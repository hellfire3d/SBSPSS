/******************/
/*** Layer Tile ***/
/******************/

#include	<DaveLib.h>
#include	<List2d.h>

#include	"MkLevelLayer.h"
#include	"MkLevelLayerTile.h"



/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
CMkLevelLayerTile::CMkLevelLayerTile(sExpLayerHdr *LayerHdr)
{
		Type=LayerHdr->Type;
		SubType=LayerHdr->SubType;
		Width=LayerHdr->Width;
		Height=LayerHdr->Height;

sExpLayerTile	*MapPtr=(sExpLayerTile *)((int)LayerHdr+sizeof(sExpLayerHdr));

		InMap.SetSize(Width,Height);
		
		for (int Y=0; Y<Height; Y++)
		{
			for (int X=0; X<Width; X++)
			{
				sExpLayerTile	ThisTile;
				
				ThisTile.Tile=MapPtr->Tile;
				ThisTile.Flags=MapPtr->Flags;
				InMap.Set(X,Y,ThisTile);
				MapPtr++;
			}
		}
}

/*****************************************************************************/
/*****************************************************************************/
/*** Pre-Process *************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CMkLevelLayerTile::PreProcess(CMkLevel *Core)
{
int		Width=InMap.GetWidth();
int		Height=InMap.GetHeight();

		OutMap.SetSize(Width,Height);

		for (int Y=0; Y<Height; Y++)
		{
			for (int X=0; X<Width; X++)
			{
				sExpLayerTile	&InElem=InMap.Get(X,Y);
				sMkLevelElem	&OutElem=OutMap.Get(X,Y);
				
				OutElem.Elem=0;
				if (InElem.Tile)
				{ // Dont process blanks
					OutElem.Elem=Core->AddTile2d(InElem);
				}
			}
		}
}

/*****************************************************************************/
/*****************************************************************************/
/*** Process *****************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CMkLevelLayerTile::Process(CMkLevel *Core)
{
}

/*****************************************************************************/
/*****************************************************************************/
/** Write ********************************************************************/
/*****************************************************************************/
/*****************************************************************************/
int		CMkLevelLayerTile::Write(FILE *File,const char *LayerName,const char *MapName)
{
sLayerHdr	Hdr;
int			ThisPos=ftell(File);
int			Width=OutMap.GetWidth();
int			Height=OutMap.GetHeight();

		Hdr.Type=Type;
		Hdr.SubType=SubType;
		Hdr.Width=Width;
		Hdr.Height=Height;
		fwrite(&Hdr,sizeof(sLayerHdr),1,File);

		printf("%s (%i,%i)= %i\n",LayerName,Width,Height,Width*Height*sizeof(sTileMapElem));

		for (int Y=0; Y<Height; Y++)
		{
			for (int X=0; X<Width; X++)
			{
				sMkLevelElem	&ThisElem=OutMap.Get(X,Y);	
				sTileMapElem	OutElem;
				OutElem.Tile=ThisElem.Elem;
//				OutElem.LightIdx=0;
			
				fwrite(&OutElem,sizeof(sTileMapElem),1,File);
			}
		}
		PadFile(File);

		return(ThisPos);
}

/*****************************************************************************/
