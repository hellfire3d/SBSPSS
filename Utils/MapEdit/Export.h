/**************/
/*** Export ***/
/**************/

#ifndef	__EXPORT_HEADER__
#define	__EXPORT_HEADER__

#include	"stdafx.h"
#include	<Vector>
#include	<GFName.hpp>
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
virtual		void	ExportLayerCollision(CCore *Core,char *LayerName,int SubType,CMap &Map)=0;

virtual		void	ExportTiles(CCore *Core)=0;

protected:
		void		BuildColTable(CTile &ThisTile);

		GFName				Filename;
		FILE				*File;
		int					Count;

		std::vector<int>	ColTable;


};

/*****************************************************************************/
#endif

