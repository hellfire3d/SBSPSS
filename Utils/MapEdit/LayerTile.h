/******************/
/*** Layer Tile ***/
/******************/

#ifndef	__LAYER_TILE_HEADER__
#define	__LAYER_TILE_HEADER__

#include	"Layer.h"

/*****************************************************************************/
enum	TileLayerEnum
{
	TileLayerDefaultWidth=20,
	TileLayerDefaultHeight=12,
};

/*****************************************************************************/
class	CCore;
class	CMapEditView;
class	CLayerTile : public CLayer
{

public:
		CLayerTile(char *_Name,int Width,int Height,float ZDiv,BOOL Is3d);	// New Layer
		CLayerTile(char *_Name);											// Load Layer
		~CLayerTile();

		void			Render(CCore *Core,Vec &CamPos,BOOL Is3d);
		void			RenderGrid(CCore *Core,Vec &CamPos);
		void			FindCursorPos(CCore *Core,CMapEditView *View,Vec &CamPos,CPoint &MousePos);

		void			InitGUI(CCore *Core);
		void			UpdateGUI(CCore *Core);

protected:
		void			Render2d(CCore *Core,Vec &CamPos);
		void			Render3d(CCore *Core,Vec &CamPos);

		CMap			Map;
		

};

/*****************************************************************************/
#endif
