/******************/
/*** Layer Core ***/
/******************/

#ifndef	__LAYER_HEADER__
#define	__LAYER_HEADER__

#include	<Vector>
//#include	"gl3d.h"
#include	<Vector3.h>
#include	"Map.h"
#include	"LayerDef.h"

/*****************************************************************************/
enum	LAYER_ENUMS
{
SELECT_BUFFER_SIZE=16,
};

enum
{
	SCREEN_WIDTH_TILE=30,
	SCREEN_HEIGHT_TILE=20,
};

/*****************************************************************************/
class	CCore;
class	CMapEditView;
class	CExport;
class	CLayer
{
public:
		CLayer(){};
virtual	~CLayer(){};

virtual	char			*GetName()=0;
virtual	void			SetVisible(BOOL f)				{VisibleFlag=f;}
virtual	BOOL			IsVisible()						{return(VisibleFlag);}
virtual	int				GetType()=0;
virtual	int				GetSubType()					{return(-1);}

virtual	float			GetScaleFactor()				{return(ScaleFactor);}

virtual	void			Render(CCore *Core,Vector3 &CamPos,BOOL Is3d)=0;
virtual	void			RenderGrid(CCore *Core,Vector3 &CamPos,BOOL Active)=0;

virtual	void			FindCursorPos(CCore *Core,CMapEditView *View,Vector3 &CamPos,CPoint &MousePos)=0;
virtual	void			RenderCursor(CCore *Core,Vector3 &CamPos,BOOL Is3d)=0;

virtual	void			InitGUI(CCore *Core)=0;
virtual	void			UpdateGUI(CCore *Core)=0;

virtual	int				GetWidth()=0;
virtual	int				GetHeight()=0;
virtual	void			CheckLayerSize(int Width,int Height){};
virtual	BOOL			Resize(int Width,int Height)=0;

virtual	void			Load(CFile *File,float Version)=0;
virtual	void			Save(CFile *File)=0;

virtual	void			Export(CCore *Core,CExport &Exp)=0;

// Functions
virtual	BOOL			SetMode(int NewMode)=0;
virtual	BOOL			InitMode()=0;
virtual	BOOL			ExitMode()=0;
virtual	BOOL			LButtonControl(CCore *Core,CMapEditView *View,UINT nFlags, CPoint &CursorPos,BOOL DownFlag)=0;
virtual	BOOL			RButtonControl(CCore *Core,CMapEditView *View,UINT nFlags, CPoint &CursorPos,BOOL DownFlag)=0;
virtual	BOOL			MouseMove(CCore *Core,CMapEditView *View,UINT nFlags, CPoint &CursorPos)=0;

virtual	BOOL			MirrorX(CCore *Core){return(FALSE);};
virtual	BOOL			MirrorY(CCore *Core){return(FALSE);};


virtual	void			DeleteSet(int Set){};
virtual	void			RemapSet(int OrigSet,int NewSet){};

protected:
		BOOL			Render3dFlag;
		float			ScaleFactor;
		BOOL			ResizeFlag;
		BOOL			VisibleFlag;

};


/*****************************************************************************/
#endif
