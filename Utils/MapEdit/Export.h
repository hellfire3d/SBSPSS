/**************/
/*** Export ***/
/**************/

#ifndef	__EXPORT_HEADER__
#define	__EXPORT_HEADER__

#include	"stdafx.h"
#include	"mapedit.h"
#include	<Vector>

#include	<List.h>
#include	"ExportHdr.h"

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
class	CCore;
class	CMap;
class	CTile;

class	CExport
{
public:
		CExport(char *Filename);
		~CExport();

		void	Write(void *Addr,int Len);
		int		ExportLayerHeader(sLayerDef &LayerDef);//int Type,int SubType,int Width,int Height);
		int		AddTile(sExpTile &Tile)					{return(UsedTileList.Add(Tile));}
		void	PadFile();
		void	ExportTiles(CCore *Core);
		void	ExportStrList(CCore *Core);
		
		void	ExportElem3d(CCore *Core,CElem &ThisElem,int &TriStart,int &TriCount);
protected:
		void	ExportTile(CCore *Core,sExpTile	&ThisElem);
		void	ExportTile3d(CCore *Core,CElem &ThisTile,sExpTile &OutTile);
		void	ExportTile2d(CCore *Core,CElem &ThisTile,sExpTile &OutTile);

		sExpFileHdr			FileHdr;
		
		CList<int>			LayerOfs;

		CList<sExpTri>		TriList;

		CList<GString>		SetNames;
		CList<GString>		TexNames;

		CList<sExpTile>		UsedTileList;
		FILE				*File;

};

/*****************************************************************************/
#endif

