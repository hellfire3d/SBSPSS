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
		CExport(char *Filename,int LayerCount);
		~CExport();

		void	ExportLayerTile(CCore *Core,char *LayerName,int SubType,CMap &Map);
		void	ExportLayerCollision(CCore *Core,char *LayerName,int SubType,CMap &Map);

		void	ExportTiles(CCore *Core);
		void	ExportStrList(CCore *Core);

protected:
		void	ExportTile(CCore *Core,sExpTile	&ThisElem);
		void	ExportTile3d(CCore *Core,CTile &ThisTile,sExpTile &OutTile);
		void	ExportTile2d(CCore *Core,CTile &ThisTile,sExpTile &OutTile);

		sExpFileHdr			FileHdr;
		
		int					LayerCount;
		CList<int>			LayerOfs;

		CList<sExpTri>		TriList;

		CList<GString>		SetNames;
		CList<GString>		TexNames;

		CList<sExpTile>		UsedTileList;
		FILE				*File;

};

/*****************************************************************************/
#endif

