/******************/
/*** Layer Core ***/
/******************/

#ifndef	__LAYER_HEADER__
#define	__LAYER_HEADER__

#include	<Vector>
#include	"gl3d.h"
#include	"Map.h"

/*****************************************************************************/
enum	LAYER_ENUMS
{
SELECT_BUFFER_SIZE=16,
};

/*****************************************************************************/
enum LAYER_TYPE
{
	LAYER_TYPE_BACK=0,
	LAYER_TYPE_MID,
	LAYER_TYPE_ACTION,
	LAYER_TYPE_FORE,

	LAYER_TYPE_MAX
};


/*****************************************************************************/
class	CCore;
class	CMap;
class	CMapEditView;
class	CLayer
{
public:
		CLayer();
		~CLayer();

// Virtual
virtual	void			Init()=0;
virtual	char			*GetName()=0;
virtual	void			Render(CCore *Core,Vec &MapPos,BOOL Is3d);
virtual	void			Render2d(CCore *Core,Vec &MapPos);
virtual	void			Render3d(CCore *Core,Vec &MapPos);
virtual	void			RenderGrid(CCore *Core,Vec &MapPos);

virtual	float			GetLayerZPosDiv()=0;
virtual	BOOL			CanRender3d()=0;

virtual	void			FindCursorPos(CCore *Core,CMapEditView *View,Vec &MapPos,CPoint &MousePos);

protected:
		CMap			Map;

};


/*****************************************************************************/
#endif
