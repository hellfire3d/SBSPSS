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

		void			Render(CCore *Core,Vec &MapPos,BOOL Is3d);
		void			RenderGrid(CCore *Core,Vec &MapPos);
		void			FindCursorPos(CCore *Core,CMapEditView *View,Vec &MapPos,CPoint &MousePos);

protected:
		void			Render2d(CCore *Core,Vec &MapPos);
		void			Render3d(CCore *Core,Vec &MapPos);

		CMap			Map;
		

};

/*****************************************************************************/
#endif
