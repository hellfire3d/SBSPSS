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
CExport::CExport(char *Filename)
{
		_splitpath(Filename,Drive,Path,Name,Ext);
}

/*****************************************************************************/
CExport::~CExport()
{
		fclose(File);
}

/*****************************************************************************/
void	CExport::ExportAll(CCore *Core)
{
		ExportTiles(Core);
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CExport::ExportLayerTile(char *LayerName,CMap &Map)
{
int			MapWidth=Map.GetWidth();
int			MapHeight=Map.GetHeight();
int			MinWidth=GetMinLayerTileWidth();
int			MinHeight=GetMinLayerTileHeight();

int			ExpWidth=max(MapWidth,MinWidth);
int			ExpHeight=max(MapHeight,MinHeight);

sMapElem	BlankElem={0,0,0};

		ExportLayerTileStart(LayerName,ExpWidth,ExpHeight);
		for (int Y=0; Y<ExpHeight; Y++)
		{
			for (int X=0; X<ExpWidth; X++)
			{
				if (X<MapWidth && Y<MapHeight)
					{
					sMapElem	&ThisElem=Map.Get(X,Y);
					int	Idx=AddTileToList(ThisElem);
					ExportLayerTile(ThisElem,Idx);
				}
				else
				{
					ExportLayerTile(BlankElem,0);
				}
			}

		}
		ExportLayerTileEnd(LayerName);

}

/*****************************************************************************/
void	CExport::PrintTileList()
{
int		ListSize=UsedTileList.size();

		for (int i=0; i<ListSize; i++)
		{
//			TRACE3("%02d: %i %i\n",i,UsedTileList[i].Set,UsedTileList[i].Tile);
		}

}

/*****************************************************************************/
int		CExport::FindTileInList(sMapElem &Tile)
{
int		ListSize=UsedTileList.size();

		for (int Idx=0; Idx<ListSize; Idx++)
		{
			sMapElem	&ListTile=UsedTileList[Idx];

			if (ListTile.Set==Tile.Set && ListTile.Tile==Tile.Tile) return(Idx);
		}

		return(-1);
}

/*****************************************************************************/
int		CExport::AddTileToList(sMapElem &Tile)
{
int		Idx=FindTileInList(Tile);

		if (Idx==-1)
		{ // New tile!!
			Idx=UsedTileList.size();
			UsedTileList.push_back(Tile);
		}

		return(Idx);
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CExport::ExportTiles(CCore *Core)
{
CTileBank	&TileBank=Core->GetTileBank();
int			ListSize=UsedTileList.size(),i;

		PrintTileList();
		for (i=0; i<ListSize; i++)
		{
			sMapElem	&ThisElem=UsedTileList[i];
			CTile		&ThisTile=Core->GetTile(ThisElem.Set,ThisElem.Tile);
			
			ParseTile(ThisTile);
		}
		CreateTilePalette();
		ExportTileStart(ListSize);
		for (i=0; i<ListSize; i++)
		{
			sMapElem	&ThisElem=UsedTileList[i];
			CTile		&ThisTile=Core->GetTile(ThisElem.Set,ThisElem.Tile);

			ExportTile(ThisTile);
		}
		ExportTileEnd();


// Palette
		ExportPaletteStart();
		ExportPalette();
		ExportPaletteEnd();

}
/*****************************************************************************/
