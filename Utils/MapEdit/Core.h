/***********************/
/*** Map Editor Core ***/
/***********************/

#ifndef	__CORE_HEADER__
#define	__CORE_HEADER__

#include	<Vector>
#include	"gl3d.h"
#include	"Layer.h"
//#include	"LayerTile.h"

#include	"TexCache.h"
#include	"TileSet.h"

const	float	FileVersion=1.0f;

//#define	UseLighting

/*****************************************************************************/
class	CMapEditView;
class	CCore
{
public:
	enum
	{
		LAYER_BACK=0,
		LAYER_MID,
		LAYER_ACTION,
		LAYER_FORE,
	};
		CCore();
		~CCore();

		void					Init();
		void					New();
		void					Load(CFile *File);
		void					Save(CFile *File);
		void					Render(CMapEditView *View,BOOL ForceRender=FALSE);
		void					RenderLayers(CMapEditView *View);
		void					RenderTileView(CMapEditView *View);
		void					Export();


// Control
		void					SetMode(int NewMode);
		void					LButtonControl(CMapEditView *View,UINT nFlags, CPoint &point,BOOL DownFlag);
		void					MButtonControl(CMapEditView *View,UINT nFlags, CPoint &point,BOOL DownFlag);
		void					RButtonControl(CMapEditView *View,UINT nFlags, CPoint &point,BOOL DownFlag);
		void					MouseWheel(CMapEditView *View,UINT nFlags, short zDelta, CPoint &pt);
		void					MouseMove(CMapEditView *View,UINT nFlags, CPoint &point);

// TileBank
		void					UpdateTileView(CMapEditView *View,BOOL Toggle=FALSE);

		CTileBank				&GetTileBank()					{return(TileBank);}
		CTile					&GetTile(int Bank,int TileNo)	{return(TileBank.GetTile(Bank,TileNo));}
		void					TileBankLoad(char *Filename);
		void					TileBankReload();
		void					TileBankSet();
		void					MirrorX(CMapEditView *View);
		void					MirrorY(CMapEditView *View);
		void					ActiveBrushLeft(CMapEditView *View);
		void					ActiveBrushRight(CMapEditView *View);
// Param Bar
		void					UpdateParamBar();

// Layers
		void					UpdateLayerGUI(CMapEditView *View);
//		void					SetActiveLayer(int Layer);
//		int						GetActiveLayer()				{return(ActiveLayer);}
//		CLayer					*GetLayer(int i)				{return(Layer[i]);}

// Grid
		void					UpdateGrid(CMapEditView *View,BOOL Toggle=FALSE);
		BOOL					IsGridOn()						{return(GridFlag);}

// Tex Cache
		CTexCache				&GetTexCache()					{return(TexCache);}

// Misc
		void					UpdateGUI(CMapEditView *View);
		void					UpdateAll(CMapEditView *View);
		void					UpdateView(CMapEditView *View,Vec Ofs=Vec(0,0,0));

		Vec						&GetCam();
		void					SetCursorPos(CPoint &Pos)		{CursorPos=Pos;}
		CPoint					&GetCursorPos()					{return(CursorPos);}

		void					SetMapSize(CMapEditView *View,int Width,int Height);
		int						GetMapWidth()					{return(Layer[LAYER_ACTION]->GetWidth());}
		int						GetMapHeight()					{return(Layer[LAYER_ACTION]->GetHeight());}

		void					Toggle2d3d(CMapEditView *View);


private:
		CPoint					CurrentMousePos,LastMousePos;
		CPoint					CursorPos,LastCursorPos;
		Vec						MapCam,TileCam;

		std::vector<CLayer*>	Layer;
		int						ActiveLayer;

		CTileBank				TileBank;

		CTexCache				TexCache;

		BOOL					TileViewFlag;
		BOOL					GridFlag;
		BOOL					Is3dFlag;
		

};

/*****************************************************************************/
#endif
