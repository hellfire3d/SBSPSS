/**************/
/*** Export ***/
/**************/

#ifndef	__EXPORT_HEADER__
#define	__EXPORT_HEADER__

#include	"stdafx.h"
#include	<Vector>

#include	"Quantize.h"


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

		void	ExportLayerTile(char *LayerName,CMap &Map);
		void	ExportAll(CCore *Core);

		void	PrintTileList();

protected:
		int		FindTileInList(sMapElem &Tile);
		int		AddTileToList(sMapElem &Tile);

		void	ExportTiles(CCore *Core);
		void	ExportPalette(CCore *Core);
		
virtual	void	ExportLayerTileStart(char *LayerName,int Width,int Height)=0;
virtual	void	ExportLayerTile(sMapElem &Elem,int NewIdx)=0;
virtual	void	ExportLayerTileEnd(char *LayerName)=0;
virtual	int		GetMinLayerTileWidth()=0;
virtual	int		GetMinLayerTileHeight()=0;

virtual	void	ExportTileStart(int TileCount)=0;
virtual	void	ParseTile(CTile &ThisTile)=0;
virtual	void	CreateTilePalette()=0;
virtual	void	ExportTile(CTile &ThisTile)=0;
virtual	void	ExportTileEnd()=0;

virtual	void	ExportPaletteStart()=0;
virtual	void	ExportPalette()=0;
virtual	void	ExportPaletteEnd()=0;


		char		Drive[_MAX_DRIVE],Path[_MAX_DIR],Name[_MAX_FNAME],Ext[_MAX_EXT];
		FILE		*File;
		int			Count;
		std::vector<sMapElem>	UsedTileList;

};

/*****************************************************************************/
#endif