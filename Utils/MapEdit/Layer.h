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
class	CMapEditView;
class	CLayer
{
public:
		CLayer(){};
virtual	~CLayer(){};

virtual	char			*GetName();
virtual	void			SetName(char *_Name);

virtual	float			GetLayerZPosDiv()		{return(ZPosDiv);}

virtual	void			Render(CCore *Core,Vec &CamPos,BOOL Is3d)=0;
virtual	void			RenderGrid(CCore *Core,Vec &CamPos)=0;

virtual	void			FindCursorPos(CCore *Core,CMapEditView *View,Vec &CamPos,CPoint &MousePos)=0;

virtual	void			InitGUI(CCore *Core)=0;
virtual	void			UpdateGUI(CCore *Core)=0;

// Functions
virtual	BOOL			SetMode(int NewMode)=0;
virtual	BOOL			InitMode()=0;
virtual	BOOL			ExitMode()=0;
virtual	BOOL			LButtonControl(CCore *Core,CMapEditView *View,UINT nFlags, CPoint &CursorPos,BOOL DownFlag)=0;
virtual	BOOL			RButtonControl(CCore *Core,CMapEditView *View,UINT nFlags, CPoint &CursorPos,BOOL DownFlag)=0;
virtual	BOOL			MouseMove(CCore *Core,CMapEditView *View,UINT nFlags, CPoint &CursorPos)=0;

protected:

		char			Name[256];
		BOOL			Render3dFlag;
		float			ZPosDiv;
};


/*****************************************************************************/
#endif
