/******************/
/*** Layer Tile ***/
/******************/

#ifndef	__LAYER_TILE_HEADER__
#define	__LAYER_TILE_HEADER__

#include	"Layer.h"
#include	"MapEdit.h"
#include	"GUIToolbar.h"

/*****************************************************************************/
class	CCore;
class	CTileBank;
class	CLayerTile : public CLayer
{

public:
		enum	MouseMode
		{
			MouseModePaint=0,
			MouseModeSelect,
		};

		CLayerTile(){};
//		CLayerTile(int Type,int SubType,int Width,int Height);
		CLayerTile(sLayerDef &Def);
		CLayerTile(CFile *File,int Version)	{Load(File,Version);}
		~CLayerTile();

		void			InitLayer(sLayerDef &Def);
		void			InitSubView(CCore *Core);

virtual	void			Render(CCore *Core,Vector3 &CamPos,bool Is3d);
virtual	void			RenderSelection(CCore *Core,Vector3 &ThisCam);
		void			RenderCursor(CCore *Core,Vector3 &CamPos,bool Is3d);
		void			Render4TGA(const char *Filename);
		void			WriteTile2Buffer(sMapElem &Elem,u8 *Buffer,int X,int Y,int PixW,int PixH);

virtual	void			GUIInit(CCore *Core);
virtual	void			GUIKill(CCore *Core);
virtual	void			GUIUpdate(CCore *Core);
virtual	void			GUIChanged(CCore *Core);

		int				GetWidth()			{return(Map.GetWidth());}
		int				GetHeight()			{return(Map.GetHeight());}
		bool			Resize(int Width,int Height);

		void			Load(CFile *File,int Version);
		void			Save(CFile *File);
		void			CheckLayerSize(int Width,int Height);
		void			Validate(CCore *Core);

		void			Export(CCore *Core,CExport &Exp);
		void			LoadGfx(CCore *Core);
// Functions
virtual	bool			LButtonControl(CCore *Core,UINT nFlags, CPoint &point,bool DownFlag);
virtual	bool			RButtonControl(CCore *Core,UINT nFlags, CPoint &point,bool DownFlag);
virtual	bool			MouseMove(CCore *Core,UINT nFlags, CPoint &point);
virtual	bool			Command(int CmdMsg,CCore *Core,int Param0=0,int Param1=0);

		sMapElem		GetMapElem(int X,int Y)	{return(Map.Get(X,Y));}
		bool			MirrorX(CCore *Core);
		bool			MirrorY(CCore *Core);
		bool			SetColFlags(CCore *Core,int Flags);

		bool			CopySelection(CCore *Core);
		bool			PasteSelection(CCore *Core);

// Local
		CTileBank		*GetTileBank()		{return(TileBank);}
		void			RemoveSet(int Set);
		void			RemapSet(int OrigSet,int NewSet);

protected:
		void			Render(CCore *Core,Vector3 &CamPos,CMap &ThisMap,bool Render3d,float Alpha=1.0f,Vector3 *Ofs=0);
		void			RenderCursorPaint(CCore *Core,Vector3 &CamPos,bool Is3d);

		bool			Paint(CMap &Blk,CPoint &CursorPos);

		CMap			Map;
		MouseMode		Mode;

		CTileBank		*TileBank;

		CGUIToolBar		GUIToolBar;

};

/*****************************************************************************/
#endif
