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
	LAYER_TYPE_TILE=0,
	LAYER_TYPE_MAX
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

virtual	void			Render(CCore *Core,Vec &CamPos,BOOL Is3d)=0;
virtual	void			RenderGrid(CCore *Core,Vec &CamPos,BOOL Active)=0;

virtual	void			FindCursorPos(CCore *Core,CMapEditView *View,Vec &CamPos,CPoint &MousePos)=0;
virtual	void			RenderCursor(CCore *Core,Vec &CamPos,BOOL Is3d)=0;

virtual	void			InitGUI(CCore *Core)=0;
virtual	void			UpdateGUI(CCore *Core)=0;

virtual	int				GetWidth()=0;
virtual	int				GetHeight()=0;
virtual	void			Resize(int Width,int Height)=0;

virtual	void			Load(CFile *File,float Version)=0;
virtual	void			Save(CFile *File)=0;

virtual	void			Export(CExport &Exp)=0;

// Functions
virtual	BOOL			SetMode(int NewMode)=0;
virtual	BOOL			InitMode()=0;
virtual	BOOL			ExitMode()=0;
virtual	BOOL			LButtonControl(CCore *Core,CMapEditView *View,UINT nFlags, CPoint &CursorPos,BOOL DownFlag)=0;
virtual	BOOL			RButtonControl(CCore *Core,CMapEditView *View,UINT nFlags, CPoint &CursorPos,BOOL DownFlag)=0;
virtual	BOOL			MouseMove(CCore *Core,CMapEditView *View,UINT nFlags, CPoint &CursorPos)=0;

virtual	BOOL			MirrorX(CCore *Core){return(FALSE);};
virtual	BOOL			MirrorY(CCore *Core){return(FALSE);};

protected:
		BOOL			Render3dFlag;
		float			ScaleFactor;
		BOOL			ResizeFlag;
		BOOL			VisibleFlag;

};


/*****************************************************************************/
#endif
