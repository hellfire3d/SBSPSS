/***********************/
/*** Map Editor Core ***/
/***********************/

#ifndef	__CORE_HEADER__
#define	__CORE_HEADER__

#include	<Vector>
#include	<Vector3.h>
#include	"Layer.h"

#include	"TexCache.h"
#include	"TileSet.h"

#include	"MapEdit.h"
#include	"GUILayerList.h"
#include	"GUITileBank.h"

#include	"Layer.h"
#include	"LayerTile.h"

const	s32		FileVersion=10;

#define	SCREEN_MAP_WIDTH	30
#define	SCREEN_MAP_HEIGHT	20

const Vector3	DefaultCamPos(0.0f,0.0f,0.9f);

/*****************************************************************************/
class	CMapEditView;

class	CCore
{
public:
		CCore();
		~CCore();

		void					Init();
		bool					New();
		void					Load(CFile *File);
		void					Save(CFile *File);
		void					Validate(int Type);
		bool					Question(char *Txt);
		void					Render(bool ForceRender=FALSE);
		void					RenderLayers(bool OneShot=false);
		void					RenderNumber(int No);
		void					Export(const char *Filename);
		void					RenderToTga(char *Filename);
		void					RenderToTga();

// View Stuff
		void					SetView(CMapEditView *View)			{CurrentView=View;}
		CMapEditView			*GetView()							{return(CurrentView);}
// Control
		void					SetMode(int NewMode);
		void					LButtonControl(UINT nFlags, CPoint &point,bool DownFlag);
		void					MButtonControl(UINT nFlags, CPoint &point,bool DownFlag);
		void					RButtonControl(UINT nFlags, CPoint &point,bool DownFlag);
		void					MouseWheel(UINT nFlags, short zDelta, CPoint &pt);
		void					MouseMove(UINT nFlags, CPoint &point);
		void					Zoom(float Dst);
		void					Command(int CmdMsg,int Param0=0,int Param1=0);

// Subview & TileBank
		CTileBank				*GetTileBank()					{return(ActionLayer->GetTileBank());}
		void					ToggleSubView();
		bool					IsSubView()						{return(CurrentLayer!=Layer[ActiveLayer]);}
// GUI 
		void					UpdateParamBar();
		void					GUIAdd(CDialog &Dlg,int ID,bool Visible=true,bool Lock=false);
		void					GUIRemove(CDialog &Dlg,int ID,bool Force=false);
		void					GUIRemoveAll(bool Force=false);
		void					GUIUpdate();
		void					GUIChanged();

// Layers
		int						AddLayer(int Type, int SubType, int Width, int Height);
		int						AddLayer(CLayer *Layer);
		void					SetLayer(int Layer,bool Force=false);
		void					AddLayer(int Layer);
		void					DeleteLayer(int Layer);
		void					UpdateLayerGUI();
		int						GetLayerCount()					{return(Layer.size());}
		CLayer					*GetLayer(int i)				{return(Layer[i]);}

// Grid
		void					ToggleGrid();
		bool					IsGridOn()						{return(GridFlag);}

// Tex Cache
		CTexCache				&GetTexCache()					{return(TexCache);}

// Misc
		void					UpdateAll();
		void					RedrawView();
		void					UpdateView(Vector3 *Ofs=NULL);

		Vector3					&GetCam();
		Vector3					OffsetCam(Vector3 &Cam,float DivVal);
		void					SetCursorPos(CPoint &Pos)		{CursorPos=Pos;}
		CPoint					&GetCursorPos()					{return(CursorPos);}

		void					SetMapSize(int Width,int Height);
		int						GetMapWidth()					{return(ActionLayer->GetWidth());}
		int						GetMapHeight()					{return(ActionLayer->GetHeight());}

		void					Toggle2d3d();
		int						FindLayer(int Type,int SubType=-1);
		int						SetActionLayer(CLayerTile *Lyr)		{ActionLayer=Lyr;}
		CLayer					*GetActionLayer()					{return(ActionLayer);}

		void					SetScale();
		Vector3					&GetScaleVector()			{return(ScaleVector);}
		float					GetZoomW();
		float					GetZoomH();
		
		void					CopySelection();
		void					PasteSelection();

		void					SetCamPos(Vector3 Pos);
		void					SetCamPos(CPoint &Pos);

		CElemBank				*GetIconBank()					{return(IconBank);}

private:
		CMapEditView			*CurrentView;
		CPoint					CurrentMousePos,LastMousePos;
		CPoint					CursorPos,LastCursorPos;
		Vector3					MapCam;
		Vector3					MapCamOfs,TileCamOfs;
		Vector3					ScaleVector;

		std::vector<CLayer*>	Layer;
		CLayer					*CurrentLayer;
		CLayerTile				*ActionLayer;
		int						ActiveLayer;

		CTexCache				TexCache;

		CGUILayerList			LayerList;

		bool					SpareFlag;
		bool					GridFlag;
		bool					Is3dFlag;

		CElemBank				*IconBank;
		bool					RenderTGAFlag;
		GString					TGAFilename;
	
};

/*****************************************************************************/
#endif
