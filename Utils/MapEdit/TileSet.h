/*********************/
/*** TileSet Stuph ***/
/*********************/

#ifndef	__TILESET_HEADER__
#define	__TILESET_HEADER__

#include	"stdafx.h"
#include	<Vector3.h>
#include	<gl\gl.h>
#include	<gl\glu.h>
#include	<Vector>
#include	<gfname.hpp>

#include	"Layer.h"
#include	"TexCache.h"
#include	"Tile.h"

#include	"MapEdit.h"
#include	"LayerTileGui.h"

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
class	CTileBank : public CLayer
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
// Overloads
		int			GetType()							{return(0);}

		void		Render(CCore *Core,Vector3 &CamPos,bool Is3d);
		void		RenderGrid(CCore *Core,Vector3 &CamPos,bool Active);
		void		RenderSelection(CCore *Core,Vector3 &ThisCam){};
		void		RenderCursor(CCore *Core,Vector3 &CamPos,bool Is3d);
		void		FindCursorPos(CCore *Core,Vector3 &CamPos,CPoint &MousePos);

		void		GUIInit(CCore *Core);
		void		GUIKill(CCore *Core);
		void		GUIUpdate(CCore *Core);
		void		GUIChanged(CCore *Core);

		int			GetWidth(){return(0);}
		int			GetHeight(){return(0);}
		void		CheckLayerSize(int Width,int Height){};
		bool		Resize(int Width,int Height){return(false);}

		void		Load(CFile *File,int Version);
		void		Save(CFile *File);

		void		Export(CCore *Core,CExport &Exp){};

		bool		LButtonControl(CCore *Core,UINT nFlags, CPoint &CursorPos,bool DownFlag);
		bool		RButtonControl(CCore *Core,UINT nFlags, CPoint &CursorPos,bool DownFlag);
		bool		MouseMove(CCore *Core,UINT nFlags, CPoint &CursorPos);
		bool		Command(int CmdMsg,CCore *Core,int Param0=0,int Param1=0);

// Local
		void		AddTileSet(const char *Filename);
		int			NeedLoad()							{return(LoadFlag);}
		void		DeleteCurrent();
		void		ReloadAll();
		void		LoadTileSets(CCore *Core);
		CTile		&GetTile(int Bank,int Tile);
		
		int			GetSetCount()						{return(TileSet.size());}
		
		CMap		&GetLBrush()						{return(Brush[LBrush]);}
		CMap		&GetRBrush()						{return(Brush[RBrush]);}
		CMap		&GetBrush(int i)					{return(Brush[i]);}
		CMap		&GetActiveBrush()					{return(GetBrush(ActiveBrush));}

		bool		IsTileValid(int Set,int Tile);
		bool		CanClose()							{return(SelStart==-1);}

		void		SetCollision(bool f);
		CTileSet	&GetSet(int Set)					{return(TileSet[Set]);}

// Functions
		bool		SelectCancel();
		void		LoadSet(CCore *Core);
		void		DeleteSet(CCore *Core);


protected:

		bool		Select(int BrushID,bool DownFlag);
		void		SetBrush(CMap &ThisBrush);

		CList<CTileSet>			TileSet;
		int						CurrentSet,LastSet;
		CMap					Brush[2];
		int						ActiveBrush;
		int						SelStart,SelEnd;

		bool					LoadFlag;
		int						LastCursorPos,CursorPos;

		CLayerTileGUI			TileBankGUI;

};

/*****************************************************************************/
class	CTileSet
{
public:
		CTileSet(const char *_Filename,int Idx);
		~CTileSet();
		
		int			IsLoaded()			{return(Loaded);}
		int			GetTileCount()		{return(Tile.size());}

		void		Load(CCore *Core);
		void		Load2d(CCore *Core);
		void		Load3d(CCore *Core);

const	char		*GetFilename()		{return(Filename);}
const	char		*GetName()			{return(Name);}

		CTile		&GetTile(int No)	{return(Tile[No]);}
		void		Purge();
		int			FindCursorPos(CCore *Core,Vector3 &CamPos,CPoint &MousePos);
		void		Render(CCore *Core,Vector3 &CamPos,CMap &LBrush,CMap &RBrush,bool Render3d);
		void		RenderCursor(Vector3 &CamPos,int Pos,int Width, int Height);
		void		RenderBrush(Vector3 &CamPos,CMap &LBrush,CMap &RBrush);
		void		RenderGrid(CCore *Core,Vector3 &CamPos,bool Active);
		int			GetTileBrowserWidth()		{return(TileBrowserWidth);}
		bool		IsTileValid(int No);

bool			operator==(CTileSet const &v1)		{return (Name==v1.Name);}

//			if (IsStrSame((char*)Name,(char*)v1.Name) return(i);

private:
		bool		Create16x16Tile(sRGBData &Src,u8 *Dst,int XOfs,int YOfs);
		CPoint		GetTilePos(int ID);

		GString			Filename,Name;
		
		int				SetNumber;
		CList<CTile>	Tile;
		bool			Loaded;
		int				TileBrowserWidth;
};

/*****************************************************************************/
#endif