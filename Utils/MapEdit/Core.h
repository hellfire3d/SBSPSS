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

const	s32		FileVersion=2;

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
		void					Render(CMapEditView *View,BOOL ForceRender=FALSE);
		void					RenderLayers(CMapEditView *View);
		void					RenderTileView(CMapEditView *View);
		void					Export(char *Filename);


// Control
		void					SetMode(int NewMode);
		void					LButtonControl(CMapEditView *View,UINT nFlags, CPoint &point,BOOL DownFlag);
		void					MButtonControl(CMapEditView *View,UINT nFlags, CPoint &point,BOOL DownFlag);
		void					RButtonControl(CMapEditView *View,UINT nFlags, CPoint &point,BOOL DownFlag);
		void					MouseWheel(CMapEditView *View,UINT nFlags, short zDelta, CPoint &pt);
		void					MouseMove(CMapEditView *View,UINT nFlags, CPoint &point);
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
		void					TileBankGUIUpdate()				{TileBank.GUIUpdate(this);}

// GUI 
		void					UpdateParamBar();
		void					GUIAdd(CDialog &Dlg,int ID,bool Visible=true,bool Lock=false);
		void					GUIRemove(CDialog &Dlg,int ID,bool Force=false);
		void					GUIRemoveAll(bool Force=false);
		void					GUIUpdate();
		void					GUIChanged();

// Layers
		void					AddLayer(int Type, int SubType, int Width, int Height);
		CLayer					*AddLayer(CLayer *Layer);
		void					SetLayer(int Layer);
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
		int						GetMapWidth()					{return(Layer[FindActionLayer()]->GetWidth());}
		int						GetMapHeight()					{return(Layer[FindActionLayer()]->GetHeight());}

		void					Toggle2d3d();
		int						FindLayer(int Type,int SubType=-1);
		int						FindActionLayer();

		void					SetScale();
		Vector3					&GetScaleVector()			{return(ScaleVector);}
		float					GetZoomW();
		float					GetZoomH();
		
		void					CopySelection();
		void					PasteSelection();

		GString					GetCurrentPath();

private:
		CPoint					CurrentMousePos,LastMousePos;
		CPoint					CursorPos,LastCursorPos;
		Vector3					MapCam,TileCam;
		Vector3					MapCamOfs,TileCamOfs;
		Vector3					ScaleVector;

		std::vector<CLayer*>	Layer;
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
