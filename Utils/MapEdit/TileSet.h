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
DefTileBrowserWidth=8,
};

/*****************************************************************************/
class	CCore;
class	CTile;

/*****************************************************************************/
class	CTileSet;
class	CTileBank
{
public:
		CTileBank();
		~CTileBank();

		enum	BrushEnum
		{
			LBrush=0,
			RBrush,
			MaxBrush
		};

		int			FindTileSet(char *Filename);
		void		AddTileSet(char *Filename);
		int			NeedLoad()							{return(LoadFlag);}
		void		Delete();
		void		Reload();
		void		LoadTileSets(CCore *Core);
		CTile		&GetTile(int Bank,int Tile);
		
		void		SetCurrent(int Set)					{CurrentSet=Set;}

		CMap		&GetLBrush()						{return(Brush[LBrush]);}
		CMap		&GetRBrush()						{return(Brush[RBrush]);}
		CMap		&GetBrush(int i)					{return(Brush[i]);}
		CMap		&GetActiveBrush()					{return(GetBrush(ActiveBrush));}

		void		RenderSet(CCore *Core,Vec &CamPos,BOOL Is3d);
		void		FindCursorPos(CCore *Core,CMapEditView *View,Vec &CamPos,CPoint &MousePos);
		void		RenderCursor(CCore *Core,Vec &CamPos,BOOL Is3d);

		void		UpdateGUI(CCore *Core,BOOL IsTileView);

		BOOL		IsTileValid(int Set,int Tile);
		BOOL		IsTileValidGB(int Set,int Tile);

// Functions
		BOOL		SelectL(BOOL DownFlag)				{return(Select(LBrush,DownFlag));}
		BOOL		SelectR(BOOL DownFlag)				{return(Select(RBrush,DownFlag));}
		BOOL		SelectCancel();

		void		SetActiveBrushL()					{ActiveBrush=LBrush;}
		void		SetActiveBrushR()					{ActiveBrush=RBrush;}

		void		Load(CFile *File,float Version);
		void		Save(CFile *File);


private:
		BOOL		Select(int BrushID,BOOL DownFlag);
		void		SetBrush(CMap &ThisBrush);

		std::vector<CTileSet>	TileSet;
		int						CurrentSet;
		CMap					Brush[2];
		int						ActiveBrush;
		int						SelStart,SelEnd;

		BOOL					LoadFlag;
		int						LastCursorPos,CursorPos;
};

/*****************************************************************************/
class	CTileSet
{
public:
		CTileSet(char *_Filename,int Idx);
		~CTileSet();
		
		int		IsLoaded()			{return(Loaded);}
		int		GetTileCount()		{return(Tile.size());}

		void	Load(CCore *Core);
		void	Load2d(CCore *Core);
		void	Load3d(CCore *Core);

		char	*GetPath()			{return(Path);}
		char	*GetName()			{return(Name);}
		char	*GetExt()			{return(Ext);}

		CTile	&GetTile(int No)	{return(Tile[No]);}
		void	Purge();
		int		FindCursorPos(CCore *Core,CMapEditView *View,Vec &CamPos,CPoint &MousePos);
		void	Render(Vec &CamPos,CMap &LBrush,CMap &RBrush,BOOL Render3d);
		void	RenderCursor(Vec &CamPos,int Pos,int Width, int Height);
		void	RenderBrush(Vec &CamPos,CMap &LBrush,CMap &RBrush);
		void	RenderGrid(Vec &CamPos);
		int		GetTileBrowserWidth()		{return(TileBrowserWidth);}
		BOOL	IsTileValid(int No)			{return(Tile[No].IsValid());}
		BOOL	IsTileValidGB(int No)		{return(Tile[No].IsValidGB());}


private:
		BOOL	Create16x16Tile(sRGBData &Src,u8 *Dst,int XOfs,int YOfs);
		CPoint	GetTilePos(int ID);

		char				Drive[_MAX_DRIVE],Path[_MAX_DIR],Name[_MAX_FNAME],Ext[_MAX_EXT];
		int					SetNumber;
		std::vector<CTile>	Tile;
		BOOL				Loaded;
		int					TileBrowserWidth;
};

/*****************************************************************************/
#endif