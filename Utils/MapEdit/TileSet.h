/*********************/
/*** TileSet Stuph ***/
/*********************/

#ifndef	__TILESET_HEADER__
#define	__TILESET_HEADER__

#include	"stdafx.h"
#include	"gl3d.h"
#include	<gl\gl.h>
#include	<gl\glu.h>
#include	<gl\glut.h>
#include	<Vector>

#include	"TexCache.h"
#include	"Tile.h"

/*****************************************************************************/
enum	TileSetEnum
{
TileBrowserWidth=8,
};

/*****************************************************************************/
class	CCore;
class	CTile;

/*****************************************************************************/
class	CTileBank
{
public:
		CTileBank();
		~CTileBank();

		void		AddTileSet(char *Filename);
		int			NeedLoad()							{return(LoadFlag);}
		void		Reload();
		void		LoadTileSets(CCore *Core);
		CTile		&GetTile(int Bank,int Tile);
		
		void		SetCurrent(int Set)					{CurrentSet=Set;}

		sMapElem	&GetLTile()							{return(LTile);}
		sMapElem	&GetRTile()							{return(RTile);}

		void		RenderSet(CCore *Core,Vec &CamPos,BOOL Is3d);
		void		FindCursorPos(CCore *Core,CMapEditView *View,Vec &CamPos,CPoint &MousePos);

		void		UpdateGUI(CCore *Core,BOOL IsTileView);

// Functions
		BOOL		TileSelectL()						{return(TileSelect(LTile,RTile));}
		BOOL		TileSelectR()						{return(TileSelect(RTile,LTile));}

private:
		BOOL	TileSelect(sMapElem &ThisTile,sMapElem &OtherTile);

		std::vector<CTileSet>	TileSet;
		int						CurrentSet;
		sMapElem				LTile,RTile;

		BOOL					LoadFlag;
		int						CursorPos;
};

/*****************************************************************************/
class	CTileSet
{
public:
		CTileSet(char *_Filename);
		~CTileSet();
		
		int		IsLoaded()			{return(Loaded);}
		void	Load(CCore *Core);
		char	*GetPath()			{return(Path);}
		char	*GetName()			{return(Name);}
		CTile	&GetTile(int No)	{return(Tile[No]);}
		void	Purge();
		int		FindCursorPos(CCore *Core,CMapEditView *View,Vec &CamPos,CPoint &MousePos);
		void	Render2d(Vec &CamPos,int LTile,int RTile,int CursorPos,BOOL GridFlag);
		void	Render3d(Vec &CamPos,int LTile,int RTile,int CursorPos,BOOL GridFlag);

private:
		void	RenderMisc(BOOL LTileFlag,BOOL RTileFlag,BOOL CursorFlag,BOOL GridFlag);

		char				Path[256],Name[256];
		std::vector<CTile>	Tile;
		BOOL				Loaded;

};

/*****************************************************************************/
#endif