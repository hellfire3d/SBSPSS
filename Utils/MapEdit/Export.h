/**************/
/*** Export ***/
/**************/

#ifndef	__EXPORT_HEADER__
#define	__EXPORT_HEADER__

#include	"stdafx.h"
#include	<Vector>
#include	"mapedit.h"

#include	"Quantize.h"
#include	<list.h>


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

virtual		void	ExportLayerTile(CCore *Core,char *LayerName,int SubType,CMap &Map)=0;
virtual		void	ExportTiles(CCore *Core)=0;

protected:

		char		Drive[_MAX_DRIVE],Path[_MAX_DIR],Name[_MAX_FNAME],Ext[_MAX_EXT];
		FILE		*File;
		int			Count;

		CList<sMapElem>			UsedTileList;


};

/*****************************************************************************/
#endif

