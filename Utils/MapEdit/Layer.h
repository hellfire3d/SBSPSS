/******************/
/*** Layer Core ***/
/******************/

#ifndef	__LAYER_HEADER__
#define	__LAYER_HEADER__

#include	<Vector>
#include	<Vector3.h>
#include	"Map.h"
#include	"Select.h"
#include	"LayerDef.h"

/*****************************************************************************/
struct	sLayerInfoTable
{
	LAYER_TYPE		Type;
	LAYER_SUBTYPE	SubType;
	char			*Name;

	bool			DeleteFlag;
	float			ScaleFactor;
	bool			Render3dFlag;
	bool			ResizeFlag;
	bool			ExportFlag;

};

/*****************************************************************************/
enum	LAYER_ENUMS
{
SELECT_BUFFER_SIZE=16,
TileLayerMinWidth=32,
TileLayerMinHeight=22,
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

static	sLayerInfoTable	InfoTable[];
static	int				InfoTableSize;

static	int				GetLayerIdx(int Type,int SubType);

		void			SetDefaultParams();		

		char			*GetName()						{return(InfoTable[GetLayerIdx(GetType(),GetSubType())].Name);}
		bool			CanDelete()						{return(InfoTable[GetLayerIdx(GetType(),GetSubType())].DeleteFlag);}
		bool			CanExport()						{return(InfoTable[GetLayerIdx(GetType(),GetSubType())].ExportFlag);}
		bool			IsUnique()						{return(!(InfoTable[GetLayerIdx(GetType(),GetSubType())].ExportFlag));}

virtual	void			SetVisible(BOOL f)				{VisibleFlag=f;}
virtual	BOOL			IsVisible()						{return(VisibleFlag);}
virtual	int				GetType()=0;
virtual	int				GetSubType()					{return(LAYER_SUBTYPE_NONE);}
		float			GetScaleFactor()				{return(ScaleFactor);}


virtual	void			Render(CCore *Core,Vector3 &CamPos,BOOL Is3d)=0;
virtual	void			RenderGrid(CCore *Core,Vector3 &CamPos,BOOL Active)=0;
virtual void			RenderSelection(CCore *Core,Vector3 &ThisCam)=0;


virtual	void			FindCursorPos(CCore *Core,CMapEditView *View,Vector3 &CamPos,CPoint &MousePos)=0;
virtual	void			RenderCursor(CCore *Core,Vector3 &CamPos,BOOL Is3d)=0;

virtual	void			GUIInit(CCore *Core)=0;
virtual	void			GUIKill(CCore *Core)=0;
virtual	void			GUIUpdate(CCore *Core)=0;
virtual	void			GUIChanged(CCore *Core)=0;

virtual	int				GetWidth()=0;
virtual	int				GetHeight()=0;
virtual	void			CheckLayerSize(int Width,int Height){};
virtual	BOOL			Resize(int Width,int Height)=0;

virtual	void			Load(CFile *File,int Version)=0;
virtual	void			Save(CFile *File)=0;

virtual	void			Export(CCore *Core,CExport &Exp)=0;


// Functions
virtual	BOOL			SetMode(int NewMode)=0;
virtual	BOOL			InitMode()=0;
virtual	BOOL			ExitMode()=0;
virtual	BOOL			LButtonControl(CCore *Core,CMapEditView *View,UINT nFlags, CPoint &CursorPos,BOOL DownFlag)=0;
virtual	BOOL			RButtonControl(CCore *Core,CMapEditView *View,UINT nFlags, CPoint &CursorPos,BOOL DownFlag)=0;
virtual	BOOL			MouseMove(CCore *Core,CMapEditView *View,UINT nFlags, CPoint &CursorPos)=0;

virtual	BOOL			MirrorX(CCore *Core){return(false);};
virtual	BOOL			MirrorY(CCore *Core){return(false);};

virtual	BOOL			CopySelection(CCore *Core)		{return(false);}
virtual	BOOL			PasteSelection(CCore *Core)		{return(false);}


virtual	void			DeleteSet(int Set){};
virtual	void			RemapSet(int OrigSet,int NewSet){};

protected:
		float			ScaleFactor;
		bool			Render3dFlag;
		bool			ResizeFlag;

		BOOL			VisibleFlag;
		CSelect			Selection;
};


/*****************************************************************************/
#endif
