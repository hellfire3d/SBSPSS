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


/*****************************************************************************/
class	CMapEditView;
class	CCore
{
public:
		CCore();
		~CCore();

		void					Init();
		void					NewMap();
		void					OpenMap();
		void					Render(CMapEditView *View,BOOL ForceRender=FALSE);
		void					RenderLayers(CMapEditView *View);
		void					RenderTileView(CMapEditView *View);

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
		CTile					GetTile(int Bank,int TileNo)	{return(TileBank.GetTile(Bank,TileNo));}
		void					TileBankLoad(char *Filename);
		void					TileBankReload();
		void					TileBankSet();

// Param Bar
		void					UpdateParamBar(CMapEditView *View,BOOL ViewFlag);

// Layers
		void					SetActiveLayer(int Layer);
		int						GetActiveLayer()				{return(ActiveLayer);}
		CLayer					*GetLayer(int i)				{return(Layers[i]);}

// Grid
		void					UpdateGrid(CMapEditView *View,BOOL Toggle=FALSE);
		BOOL					IsGridOn()						{return(GridFlag);}

// Tex Cache
		CTexCache				&GetTexCache()					{return(TexCache);}

// Misc
		void					UpdateAll(CMapEditView *View);
		void					UpdateView(CMapEditView *View,Vec Ofs=Vec(0,0,0));

		Vec						&GetCam();
		void					SetCursorPos(CPoint &Pos)		{CursorPos=Pos;}
		CPoint					&GetCursorPos()					{return(CursorPos);}

private:
		CPoint					CurrentMousePos,LastMousePos;
		CPoint					CursorPos,LastCursorPos;
		Vec						MapCam,TileCam;

		CLayer					*Layers[LAYER_TYPE_MAX];
		int						ActiveLayer;

		CTileBank				TileBank;

		CTexCache				TexCache;

		BOOL					TileViewFlag;
		BOOL					GridFlag;
		BOOL					Is3dFlag;
		

};

/*****************************************************************************/
#endif
