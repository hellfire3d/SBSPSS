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
enum MOUSE_MODE
{
	MOUSE_MODE_NONE=0,
	MOUSE_MODE_LMB_EDIT,
	MOUSE_MODE_MMB_EDIT,
	MOUSE_MODE_RMB_EDIT,
	
};


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


private:
		CMapEditView			*ParentWindow;	
		MOUSE_MODE				MouseMode;
		CPoint					CurrentMousePos,LastMousePos;
		Vec						MapCam,TileCam;

		CLayer					*Layers[LAYER_TYPE_MAX];
		int						ActiveLayer;

		std::vector<CTileSet>	TileSet;
		CTexCache				TexCache;

		BOOL					TileViewFlag;
		BOOL					LayerViewFlag;


};

/*****************************************************************************/
#endif
