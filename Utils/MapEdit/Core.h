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
		void					Render(CMapEditView *View);

// Control
		void					LButtonControl(CMapEditView *View,UINT nFlags, CPoint &point,BOOL DownFlag);
		void					MButtonControl(CMapEditView *View,UINT nFlags, CPoint &point,BOOL DownFlag);
		void					RButtonControl(CMapEditView *View,UINT nFlags, CPoint &point,BOOL DownFlag);
		void					MouseWheel(CMapEditView *View,UINT nFlags, short zDelta, CPoint &pt);
		void					MouseMove(CMapEditView *View,UINT nFlags, CPoint &point);

// TileBank
		void					UpdateTileView(CMapEditView *View,BOOL ViewFlag);
		void					ToggleTileView(CMapEditView *View);
		BOOL					GetTileView()					{return(TileViewFlag);}

		CTile					GetTile(int Bank,int TileNo)	{return(TileBank.GetTile(Bank,TileNo));}

// Param Bar
		void					UpdateParamBar(CMapEditView *View,BOOL ViewFlag);
		void					ToggleParamView(CMapEditView *View);
		BOOL					GetParamViewFlag()				{return(ParamViewFlag);}
// Layers
		void					SetActiveLayer(int Layer);
		int						GetActiveLayer()				{return(ActiveLayer);}
		CLayer					*GetLayer(int i)				{return(Layers[i]);}

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

		BOOL					RenderFlag;
		BOOL					TileViewFlag;
		BOOL					ParamViewFlag;

};

/*****************************************************************************/
#endif
