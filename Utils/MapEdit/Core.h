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

#include	"MApEdit.h"
#include	"LayerList.h"
#include	"LayerTileGUI.h"

const	s32		FileVersion=3;

#define	SCREEN_MAP_WIDTH	30
#define	SCREEN_MAP_HEIGHT	20

/*****************************************************************************/
class	CMapEditView;
class	CCore
{
public:
		CCore();
		~CCore();

		void					Init();
		BOOL					New();
		void					Load(CFile *File);
		void					Save(CFile *File);
		BOOL					Question(char *Txt);
		void					Render(BOOL ForceRender=FALSE);
		void					RenderLayers();
		void					RenderTileView();
		void					Export(char *Filename);

// View Stuff
		void					SetView(CMapEditView *View)			{CurrentView=View;}
		CMapEditView			*GetView()							{return(CurrentView);}
// Control
		void					SetMode(int NewMode);
		void					LButtonControl(UINT nFlags, CPoint &point,BOOL DownFlag);
		void					MButtonControl(UINT nFlags, CPoint &point,BOOL DownFlag);
		void					RButtonControl(UINT nFlags, CPoint &point,BOOL DownFlag);
		void					MouseWheel(UINT nFlags, short zDelta, CPoint &pt);
		void					MouseMove(UINT nFlags, CPoint &point);
		void					Zoom(float Dst);

// TileBank
		CTileBank				&GetTileBank()					{return(TileBank);}
		void					UpdateTileView(BOOL Toggle=FALSE);

		CTile					&GetTile(int Bank,int TileNo)	{return(TileBank.GetTile(Bank,TileNo));}
		void					TileBankLoad(char *Filename);
		void					TileBankDelete();
		void					TileBankReload();
		void					TileBankSet();
		void					MirrorX();
		void					MirrorY();
		void					ActiveBrushLeft();
		void					ActiveBrushRight();
		BOOL					IsTileValid(int Set,int Tile);
		BOOL					IsTileView()					{return(TileViewFlag);}
		void					TileBankGUIInit()				{TileBank.GUIInit(this);}
		void					TileBankGUIKill()				{TileBank.GUIKill(this);}
		void					TileBankGUIUpdate()				{TileBank.GUIUpdate(this);}

		void					SetColFlag(int Flag);

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

// Grid
		void					UpdateGrid(BOOL Toggle=FALSE);
		BOOL					IsGridOn()						{return(GridFlag);}

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
		int						SetActionLayer(CLayer *Lyr)		{ActionLayer=Lyr;}

		void					SetScale();
		Vector3					&GetScaleVector()			{return(ScaleVector);}
		float					GetZoomW();
		float					GetZoomH();
		
		void					CopySelection();
		void					PasteSelection();

		void					ResetView();

private:
		CMapEditView			*CurrentView;
		CPoint					CurrentMousePos,LastMousePos;
		CPoint					CursorPos,LastCursorPos;
		Vector3					MapCam,TileCam;
		Vector3					MapCamOfs,TileCamOfs;
		Vector3					ScaleVector;

		std::vector<CLayer*>	Layer;
		CLayer					*ActionLayer;
		int						ActiveLayer;

		CTileBank				TileBank;
		CTexCache				TexCache;

		CLayerListGUI			LayerList;

		BOOL					TileViewFlag;
		BOOL					GridFlag;
		BOOL					Is3dFlag;
		
};

/*****************************************************************************/
#endif
