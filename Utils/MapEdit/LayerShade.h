/*******************/
/*** Layer Shade ***/
/*******************/

#ifndef	__LAYER_SHADE_HEADER__
#define	__LAYER_SHADE_HEADER__

#include	"Layer.h"
#include	"MapEdit.h"
#include	"GUILayerShade.h"
#include	"Elem.h"
#include	"ExportHdr.h"


/*****************************************************************************/
struct	sBackList
{
		GString	Name;
		int		ElemID;
};


/*****************************************************************************/
class	CCore;
class	CLayerShade : public CLayer
{

public:
		enum
		{
			LAYER_SHADE_RGB_MAX=4,
		};

		CLayerShade(sLayerDef &Def);
		CLayerShade(CFile *File,int Version)	{Load(File,Version);}
		~CLayerShade();

		void			InitLayer(sLayerDef &Def);

		void			Render(CCore *Core,Vector3 &CamPos,bool Is3d);
		void			RenderGrid(CCore *Core,Vector3 &CamPos,bool Active){};
		void			RenderSelection(CCore *Core,Vector3 &ThisCam){};

		void			RenderCursor(CCore *Core,Vector3 &CamPos,bool Is3d);

		void			GUIInit(CCore *Core);
		void			GUIKill(CCore *Core);
		void			GUIUpdate(CCore *Core);
		void			GUIChanged(CCore *Core);

//		int				GetWidth()						{return(LayerDef.Width);}
//		int				GetHeight()						{return(LayerDef.Height);}
		void			CheckLayerSize(int Width,int Height);
		bool			Resize(int Width,int Height);

		void			Load(CFile *File,int Version);
		void			Save(CFile *File);

		void			Export(CCore *Core,CExport &Exp);
		virtual	void			LoadGfx(CCore *Core)		{if (GfxBank->NeedLoad()) GfxBank->LoadAllSets(Core);}

// Functions
		bool			LButtonControl(CCore *Core,UINT nFlags, CPoint &CursorPos,bool DownFlag);
		bool			RButtonControl(CCore *Core,UINT nFlags, CPoint &CursorPos,bool DownFlag);
		bool			MouseMove(CCore *Core,UINT nFlags, CPoint &CursorPos);
		bool			Command(int CmdMsg,CCore *Core,int Param0=0,int Param1=0);

protected:
		void			LoadGfx();
		void			Render(CCore *Core,Vector3 &CamPos,CMap &ThisMap,bool Render3d,float Alpha=1.0f,Vector3 *Ofs=0);
		void			RenderBackGfx(CCore *Core,Vector3 &ThisCam,sLayerShadeGfx &ThisGfx);
		void			AddGfx(CCore *Core);
		void			GotoGfx(CCore *Core);
		void			DeleteGfx(CCore *Core);

		CGUILayerShade		GUIShade;
		CIni				Script;
		CList<sBackList>	BankList;
		CElemBank			*GfxBank;
		sLayerShadeGfx			Cursor;

		int					ShadeCount;
		sRGBCol				ShadeRGB[LAYER_SHADE_RGB_MAX];

		CList<sLayerShadeGfx>		GfxList;
		int					CurrentGfx;

};

/*****************************************************************************/
#endif
