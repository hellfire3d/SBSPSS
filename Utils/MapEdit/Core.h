/***********************/
/*** Map Editor Core ***/
/***********************/

#ifndef	__CORE_HEADER__
#define	__CORE_HEADER__

#include	<Vector>
//#include	"gl3d.h"
#include	<Vector3.h>
#include	"Layer.h"
//#include	"LayerTile.h"

#include	"TexCache.h"
#include	"TileSet.h"

const	float	FileVersion=1.01f;


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
		void					Render(CMapEditView *View,BOOL ForceRender=FALSE);
		void					RenderLayers(CMapEditView *View);
		void					RenderTileView(CMapEditView *View);
		void					ExportAGB(char *Filename);
		void					ExportPSX(char *Filename);


// Control
		void					SetMode(int NewMode);
		void					LButtonControl(CMapEditView *View,UINT nFlags, CPoint &point,BOOL DownFlag);
		void					MButtonControl(CMapEditView *View,UINT nFlags, CPoint &point,BOOL DownFlag);
		void					RButtonControl(CMapEditView *View,UINT nFlags, CPoint &point,BOOL DownFlag);
		void					MouseWheel(CMapEditView *View,UINT nFlags, short zDelta, CPoint &pt);
		void					MouseMove(CMapEditView *View,UINT nFlags, CPoint &point);

// TileBank
		void					UpdateTileView(CMapEditView *View,BOOL Toggle=FALSE);

		void					UpdateTileViewGUI()				{TileBank.UpdateGUI(this,TileViewFlag);}

		CTileBank				&GetTileBank()					{return(TileBank);}
		CTile					&GetTile(int Bank,int TileNo)	{return(TileBank.GetTile(Bank,TileNo));}
		void					TileBankLoad(char *Filename);
		void					TileBankDelete();
		void					TileBankReload();
		void					TileBankSet();
		void					MirrorX(CMapEditView *View);
		void					MirrorY(CMapEditView *View);
		void					ActiveBrushLeft(CMapEditView *View);
		void					ActiveBrushRight(CMapEditView *View);
		BOOL					IsTileValid(int Set,int Tile);


// Param Bar
		void					UpdateParamBar();

// Layers
		void					SetLayer(int Layer);
		void					UpdateLayerGUI(CMapEditView *View);

// Grid
		void					UpdateGrid(CMapEditView *View,BOOL Toggle=FALSE);
		BOOL					IsGridOn()						{return(GridFlag);}

// Tex Cache
		CTexCache				&GetTexCache()					{return(TexCache);}

// Misc
		void					UpdateGUI(CMapEditView *View);
		void					UpdateAll(CMapEditView *View);
		void					UpdateView(CMapEditView *View);
		void					UpdateView(CMapEditView *View,Vector3 &Ofs);

		Vector3					&GetCam();
		Vector3					&GetCamOfs();
		Vector3					OffsetCam(Vector3 &Cam,float DivVal);
		void					SetCursorPos(CPoint &Pos)		{CursorPos=Pos;}
		CPoint					&GetCursorPos()					{return(CursorPos);}

		void					SetMapSize(CMapEditView *View,int Width,int Height);
		int						GetMapWidth()					{return(Layer[FindActionLayer()]->GetWidth());}
		int						GetMapHeight()					{return(Layer[FindActionLayer()]->GetHeight());}

		void					Toggle2d3d(CMapEditView *View);
		int						FindLayer(int Type,int SubType=-1);
		int						FindActionLayer();
		
		GString					GetCurrentPath();
private:
		CPoint					CurrentMousePos,LastMousePos;
		CPoint					CursorPos,LastCursorPos;
		Vector3					MapCam,TileCam;
		Vector3					MapCamOfs,TileCamOfs;

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
