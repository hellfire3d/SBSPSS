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

		void	ExportTileMap(char *LayerName,CMap &Map);
		void	ExportAll(CCore *Core);

		void	PrintTileList();

protected:
		int		FindTileInList(sMapElem &Tile);
		int		AddTileToList(sMapElem &Tile);

		void	ExportTiles(CCore *Core);
		void	ExportPalette(CCore *Core);
		
virtual	void	ExportTileMapStart(char *LayerName,int Width,int Height)=0;
virtual	void	ExportTileMap(sMapElem &Elem,int NewIdx)=0;
virtual	void	ExportTileMapEnd(char *LayerName)=0;

virtual	void	ExportTileStart()=0;
virtual	void	ParseTile(CTile &ThisTile)=0;
virtual	void	CreateTilePalette()=0;
virtual	void	ExportTile(CTile &ThisTile)=0;
virtual	void	ExportTileEnd()=0;

virtual	void	ExportPaletteStart()=0;
virtual	void	ExportPalette()=0;
virtual	void	ExportPaletteEnd()=0;


		char		Name[256];
		FILE		*File;
		int			Count;
		std::vector<sMapElem>	UsedTileList;

};

/*****************************************************************************/
#endif