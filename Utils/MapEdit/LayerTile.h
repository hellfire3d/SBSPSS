/******************/
/*** Layer Tile ***/
/******************/

#ifndef	__LAYER_TILE_HEADER__
#define	__LAYER_TILE_HEADER__

#include	"Layer.h"

/*****************************************************************************/

/*****************************************************************************/
enum	TileLayerEnum
{
	TileLayerDefaultWidth=30,
	TileLayerDefaultHeight=20,
};

/*****************************************************************************/
class	CCore;
class	CMapEditView;
class	CLayerTile : public CLayer
{

public:
		enum	MouseMode
		{
			MouseModePaint=0,
			MouseModeSelect,
			MouseModePicker,
		};
		enum	MouseFlag
		{
			MouseFlagMirrorX=1<<0,
			MouseFlagMirrorY=1<<1,
		};

		CLayerTile(char *_Name,int Width,int Height,float MapDiv,float ZDiv,BOOL Is3d,BOOL Resizable);		// New Layer
		CLayerTile(CFile *File,int Version);																// Load Layer
		~CLayerTile();

		int				GetType()			{return(LAYER_TYPE_TILE);}

		void			Render(CCore *Core,Vec &CamPos,BOOL Is3d);
		void			RenderGrid(CCore *Core,Vec &CamPos);

		void			FindCursorPos(CCore *Core,CMapEditView *View,Vec &CamPos,CPoint &MousePos);
		void			RenderCursor(CCore *Core,Vec &CamPos,BOOL Is3d);

		void			InitGUI(CCore *Core);
		void			UpdateGUI(CCore *Core);

		int				GetWidth()			{return(Map.GetWidth());}
		int				GetHeight()			{return(Map.GetHeight());}
		void			Resize(int Width,int Height);

		void			Load(CFile *File,float Version);
		void			Save(CFile *File);

// Functions
		BOOL			SetMode(int NewMode);
		BOOL			InitMode();
		BOOL			ExitMode();

		BOOL			LButtonControl(CCore *Core,CMapEditView *View,UINT nFlags, CPoint &point,BOOL DownFlag);
		BOOL			RButtonControl(CCore *Core,CMapEditView *View,UINT nFlags, CPoint &point,BOOL DownFlag);
		BOOL			MouseMove(CCore *Core,CMapEditView *View,UINT nFlags, CPoint &point);

		BOOL			MirrorX(CCore *Core);
		BOOL			MirrorY(CCore *Core);

protected:
		void			Render(CCore *Core,Vec &CamPos,CMap &ThisMap,BOOL Render3d,float Alpha=1.0f);
		void			RenderCursorPaint(CCore *Core,Vec &CamPos,BOOL Is3d);

		BOOL			Paint(CMap &Blk,CPoint &CursorPos);

		CMap			Map;

		MouseMode		Mode;
};

/*****************************************************************************/
#endif
