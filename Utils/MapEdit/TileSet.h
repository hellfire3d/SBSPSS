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

		void	AddTileSet(char *Filename);
		int		NeedLoad()					{return(LoadFlag);}
		void	Reload();
		void	LoadTileSets(CCore *Core);
		CTile	&GetTile(int Bank,int Tile);

		void	RenderSet(CCore *Core,Vec &CamPos,BOOL Is3d);
		void	FindCursorPos(CCore *Core,CMapEditView *View,Vec &CamPos,CPoint &MousePos);

		void	LButtonControl(CCore *Core,CMapEditView *View,UINT nFlags, CPoint &point,BOOL DownFlag);
		void	RButtonControl(CCore *Core,CMapEditView *View,UINT nFlags, CPoint &point,BOOL DownFlag);

		void	InitGUI(CCore *Core);
		void	UpdateGUI(CCore *Core);

private:

		std::vector<CTileSet>	TileSet;
		BOOL					LoadFlag;
		int						CursPos;

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
		void	Render2d(CCore *Core,Vec &CamPos,int CursorPos,int LTile,int RTile);
		void	Render3d(CCore *Core,Vec &CamPos,int CursorPos,int LTile,int RTile);
		void	RenderMisc(BOOL LSelFlag,BOOL RSelFlag, BOOL GridFlag,BOOL CursorFlag);
		int		FindCursorPos(CCore *Core,CMapEditView *View,Vec &CamPos,CPoint &MousePos);

private:
		char				Path[256],Name[256];
		std::vector<CTile>	Tile;
		BOOL				Loaded;

};

/*****************************************************************************/
#endif