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
	int				SubView;

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
class	CExport;
class	CLayer
{
public:
		CLayer();
virtual	~CLayer(){};

		void			InitLayer(sLayerDef &LayerDef);
static	sLayerInfoTable	InfoTable[];
static	int				InfoTableSize;

static	int				GetLayerIdx(int Type,int SubType);
static	CLayer			*NewLayer(sLayerDef &Def);
static	CLayer			*LoadLayer(CFile *File,int Version);

		char			*GetName()						{return(InfoTable[TableIdx].Name);}
		bool			CanDelete()						{return(InfoTable[TableIdx].DeleteFlag);}
		float			GetScaleFactor()				{return(InfoTable[TableIdx].ScaleFactor);}
		bool			GetRender3dFlag()				{return(InfoTable[TableIdx].Render3dFlag);}
		bool			GetResizeFlag()					{return(InfoTable[TableIdx].ResizeFlag);}
		bool			CanExport()						{return(InfoTable[TableIdx].ExportFlag);}
		bool			IsUnique()						{return(!(InfoTable[TableIdx].ExportFlag));}
		int				GetSubViewType()				{return((InfoTable[TableIdx].SubView));}

virtual	void			InitSubView(CCore *Core){};
virtual	CLayer			*GetSubView()					{return(SubView);}


virtual	void			SetVisible(bool f)				{LayerDef.VisibleFlag=f;}
virtual	bool			IsVisible()						{return(LayerDef.VisibleFlag);}
virtual	int				GetType()						{return(LayerDef.Type);}
virtual	int				GetSubType()					{return(LayerDef.SubType);}

virtual	void			LoadGfx(CCore *Core)=0;

virtual	void			Render(CCore *Core,Vector3 &CamPos,bool Is3d){};
virtual	void			Render4TGA(const char *Filename){}
virtual	void			RenderGrid(CCore *Core,Vector3 &CamPos,bool Active);
virtual void			RenderSelection(CCore *Core,Vector3 &ThisCam){};
virtual	void			RenderCursor(CCore *Core,Vector3 &CamPos,bool Is3d){};
virtual	void			FindCursorPos(CCore *Core,Vector3 &CamPos,CPoint &MousePos);

virtual	void			GUIInit(CCore *Core){};
virtual	void			GUIKill(CCore *Core){};
virtual	void			GUIUpdate(CCore *Core){};
virtual	void			GUIChanged(CCore *Core){};

virtual	int				GetWidth()						{return(LayerDef.Width);}
virtual	int				GetHeight()						{return(LayerDef.Height);}
virtual	void			CheckLayerSize(int Width,int Height){};
virtual	void			Validate(CCore *Core){};
virtual	bool			Resize(int Width,int Height)	{return(false);}

virtual	void			Load(CFile *File,int Version);
virtual	void			Save(CFile *File);

virtual	void			Export(CCore *Core,CExport &Exp)=0;

virtual	Vector3			&GetCam()						{return(LayerCam);}

// Functions
virtual	bool			LButtonControl(CCore *Core,UINT nFlags, CPoint &CursorPos,bool DownFlag){return(false);}
virtual	bool			RButtonControl(CCore *Core,UINT nFlags, CPoint &CursorPos,bool DownFlag){return(false);}
virtual	bool			MouseMove(CCore *Core,UINT nFlags, CPoint &CursorPos){return(false);}
virtual	bool			Command(int CmdMsg,CCore *Core,int Param0=0,int Param1=0){return(false);};

protected:

// Saved Vars
		sLayerDef		LayerDef;

// Static Quick Vars
		int				TableIdx;
//		float			ScaleFactor;
//		bool			Render3dFlag;
//		bool			ResizeFlag;

		CSelect			Selection;
		CLayer			*SubView;
		Vector3			LayerCam;

};


/*****************************************************************************/
#endif
