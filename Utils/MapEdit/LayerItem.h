/******************/
/*** Layer Item ***/
/******************/

#ifndef	__LAYER_ITEM_HEADER__
#define	__LAYER_ITEM_HEADER__

#include	"Layer.h"
#include	"MapEdit.h"
#include	"GUIToolbar.h"
#include	"ElemStore.h"


/*****************************************************************************/
class	CLayerItem : public CLayer
{
public:
		enum	MouseMode
		{
			MouseModePaint=0,
			MouseModeSelect,
		};

		CLayerItem(){};
		CLayerItem(int SubType,int Width,int Height);					// New Layer
		CLayerItem(CFile *File,int Version);							// Load Layer
		~CLayerItem();

		int				GetType()			{return(LAYER_TYPE_ITEM);}
		void			InitSubView(CCore *Core);

		void			Render(CCore *Core,Vector3 &CamPos,bool Is3d);
		void			RenderCursor(CCore *Core,Vector3 &CamPos,bool Is3d);

		void			GUIInit(CCore *Core);
		void			GUIKill(CCore *Core);
		void			GUIUpdate(CCore *Core);
		void			GUIChanged(CCore *Core);

		void			Load(CFile *File,int Version);
		void			Save(CFile *File);

		void			Export(CCore *Core,CExport &Exp);

// Functions
		bool			LButtonControl(CCore *Core,UINT nFlags, CPoint &point,bool DownFlag);
		bool			RButtonControl(CCore *Core,UINT nFlags, CPoint &point,bool DownFlag);
		bool			MouseMove(CCore *Core,UINT nFlags, CPoint &point);
		bool			Command(int CmdMsg,CCore *Core,int Param0=0,int Param1=0);

// Local
		CElemBank		*GetElemBank()		{return(ElemBank);}

protected:
		void			Render(CCore *Core,Vector3 &CamPos,CMap &ThisMap,bool Render3d,float Alpha=1.0f,Vector3 *Ofs=0);
		void			RenderCursorPaint(CCore *Core,Vector3 &CamPos,bool Is3d);

		bool			Paint(CMap &Blk,CPoint &CursorPos);

		CElemStore		*ElemBank;
		MouseMode		Mode;

		CGUIToolBar		GUIToolBar;

};

/*****************************************************************************/
#endif
