/******************/
/*** Layer Elem ***/
/******************/

#ifndef	__LAYER_ELEM_HEADER__
#define	__LAYER_ELEM_HEADER__

#include	"Layer.h"
#include	"MapEdit.h"
#include	"GUIToolbar.h"
#include	"Elem.h"

/*****************************************************************************/
struct	sLayerElem
{
	std::vector<CPoint>	XY;
	int					Type;
	int					SubType;
	
};

/*****************************************************************************/
class	CLayerElem : public CElemBank
{
public:
		enum	MouseMode
		{
			MouseModePaint=0,
			MouseModeSelect,
		};

		CLayerElem(){};
		CLayerElem(int SubType,int Width,int Height);					// New Layer
		CLayerElem(CFile *File,int Version);							// Load Layer
		~CLayerElem();

virtual	int				GetType()			{return(LAYER_TYPE_ITEM);}
virtual	void			InitSubView(CCore *Core);

virtual	void			Render(CCore *Core,Vector3 &CamPos,bool Is3d);
		void			RenderCursor(CCore *Core,Vector3 &CamPos,bool Is3d);

virtual	void			GUIInit(CCore *Core);
virtual	void			GUIKill(CCore *Core);
virtual	void			GUIUpdate(CCore *Core);
virtual	void			GUIChanged(CCore *Core);

virtual	void			Load(CFile *File,int Version);
virtual	void			Save(CFile *File);

virtual	void			Export(CCore *Core,CExport &Exp);

// Functions
virtual	bool			LButtonControl(CCore *Core,UINT nFlags, CPoint &point,bool DownFlag);
virtual	bool			RButtonControl(CCore *Core,UINT nFlags, CPoint &point,bool DownFlag);
virtual	bool			MouseMove(CCore *Core,UINT nFlags, CPoint &point);
virtual	bool			Command(int CmdMsg,CCore *Core,int Param0=0,int Param1=0);

// Local
//		CElemBank		*GetElemBank()		{return(ElemBank);}

protected:
		void			RenderElem(CCore *Core,Vector3 &CamPos,CPoint &Pos,int Elem,bool Render3d,float Alpha=1.0f);

		bool			Paint(CMap &Blk,CPoint &CursorPos);

		CElemBank			*ElemBank;
		CList<sLayerElem>	ElemList;
		MouseMode			Mode;

		CGUIToolBar			GUIToolBar;

};

/*****************************************************************************/
#endif
