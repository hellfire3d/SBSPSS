/***********************/
/*** Map Editor Core ***/
/***********************/

#ifndef	__CORE_HEADER__
#define	__CORE_HEADER__

#include	<Vector>
#include	"gl3d.h"
#include	"Layer.h"
#include	"LayerBack.h"

#include	"TexCache.h"
#include	"TileSet.h"



/*****************************************************************************/
class	CMapEditView;
class	CCore
{
public:
		CCore();
		~CCore();

		void				Init(CMapEditView *Wnd);
		void				Render();

// Control
		void				LButtonControl(UINT nFlags, CPoint &point,BOOL DownFlag);
		void				MButtonControl(UINT nFlags, CPoint &point,BOOL DownFlag);
		void				RButtonControl(UINT nFlags, CPoint &point,BOOL DownFlag);
		void				MouseWheel(UINT nFlags, short zDelta, CPoint &pt);
		void				MouseMove(UINT nFlags, CPoint &point);

// TileBank
		void				UpdateTileView(BOOL ViewFlag);
		BOOL				GetTileView()					{return(TileViewFlag);}
		void				ToggleTileView()				{UpdateTileView(!TileViewFlag);}

		GLint				GetTile(int Bank,int TileNo);

// Layers
		void				UpdateLayerBar(BOOL ViewFlag);
		BOOL				GetLayerViewFlag()				{return(LayerViewFlag);}
		void				ToggleLayerView()				{UpdateLayerBar(!LayerViewFlag);}

		void				SetActiveLayer(int Layer);
		int					GetActiveLayer()				{return(ActiveLayer);}
		CLayer				*GetLayer(int i)				{return(Layers[i]);}

// Tex Cache
		CTexCache			&GetTexCache()	{return(TexCache);}

// Misc
		void				UpdateAll();
		void				UpdateView(Vec Ofs=Vec(0,0,0));

		Vec					&GetCam();
		void				SetCursorPos(CPoint &Pos)		{CursorPos=Pos;}
		CPoint				&GetCursorPos()					{return(CursorPos);}
		CMapEditView		*GetParentWindow()				{return(ParentWindow);}
		void				Redraw(BOOL f=TRUE);

private:
		CMapEditView			*ParentWindow;	
		CPoint					CurrentMousePos,LastMousePos;
		CPoint					CursorPos,LastCursorPos;
		Vec						MapCam,TileCam;

		CLayer					*Layers[LAYER_TYPE_MAX];
		int						ActiveLayer;

		std::vector<CTileSet>	TileSet;
		CTexCache				TexCache;

		BOOL					RenderFlag;
		BOOL					TileViewFlag;
		BOOL					LayerViewFlag;


};

/*****************************************************************************/
#endif
