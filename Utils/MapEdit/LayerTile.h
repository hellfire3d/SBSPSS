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
			MouseModeNone=0,
			MouseModePaint,
			MouseModeSelect,
			MouseModePicker,
//			MouseModeMirrorX,
//			MouseModeMirrorY,
		};
		enum	MouseFlag
		{
			MouseFlagMirrorX=1<<0,
			MouseFlagMirrorY=1<<1,
		};

		CLayerTile(char *_Name,int Width,int Height,float ZDiv,BOOL Is3d);	// New Layer
		CLayerTile(char *_Name);											// Load Layer
		~CLayerTile();

		void			Render(CCore *Core,Vec &CamPos,BOOL Is3d);
		void			RenderGrid(CCore *Core,Vec &CamPos);
		void			FindCursorPos(CCore *Core,CMapEditView *View,Vec &CamPos,CPoint &MousePos);

		void			InitGUI(CCore *Core);
		void			UpdateGUI(CCore *Core);

// Functions
		BOOL			SetMode(int NewMode);
		BOOL			InitMode();
		BOOL			ExitMode();

		BOOL			SetFlag(int Flag);

		BOOL			LButtonControl(CCore *Core,CMapEditView *View,UINT nFlags, CPoint &point,BOOL DownFlag);
		BOOL			RButtonControl(CCore *Core,CMapEditView *View,UINT nFlags, CPoint &point,BOOL DownFlag);
		BOOL			MouseMove(CCore *Core,CMapEditView *View,UINT nFlags, CPoint &point);

		BOOL			MirrorX();
		BOOL			MirrorY();

protected:
		void			Render2d(CCore *Core,Vec &CamPos);
		void			Render3d(CCore *Core,Vec &CamPos);
		BOOL			Paint(sMapElem &Blk,CPoint &CursorPos);

		CMap			Map;

		MouseMode		Mode;
		BOOL			Flag;

};

/*****************************************************************************/
#endif
