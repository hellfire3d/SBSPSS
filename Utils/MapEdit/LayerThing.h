/*******************/
/*** Layer Thing ***/
/*******************/

#ifndef	__LAYER_THING_HEADER__
#define	__LAYER_THING_HEADER__

#include	"Layer.h"
#include	"MapEdit.h"
#include	"GUIToolbar.h"
#include	"Elem.h"

/*****************************************************************************/
struct	sLayerThing
{
	std::vector<CPoint>		XY;
	int						Type;
	int						SubType;
	int						Spare[8];

bool		operator==(sLayerThing const &v1)					
{
	if (XY[0]!=v1.XY[0]) return(false);
	return(true);
}

};

/*****************************************************************************/
class	CLayerThing : public CLayer
{
public:
		enum	MouseMode
		{
			MouseModeNormal=0,
			MouseModePoints,
		};

		CLayerThing(){};
		CLayerThing(int SubType,int Width,int Height);					// New Layer
		CLayerThing(CFile *File,int Version);							// Load Layer
		~CLayerThing();

virtual	int				GetType()			{return(LAYER_TYPE_ITEM);}
virtual	void			InitSubView(CCore *Core);

virtual	void			Render(CCore *Core,Vector3 &CamPos,bool Is3d);
		void			RenderCursor(CCore *Core,Vector3 &CamPos,bool Is3d);
//		void			FindCursorPos(CCore *Core,Vector3 &CamPos,CPoint &MousePos);

virtual	void			GUIInit(CCore *Core);
virtual	void			GUIKill(CCore *Core);
virtual	void			GUIUpdate(CCore *Core);
virtual	void			GUIChanged(CCore *Core);

		int				GetWidth()						{return(Width);}
		int				GetHeight()						{return(Height);}

virtual	void			Load(CFile *File,int Version);
virtual	void			Save(CFile *File);

virtual	void			Export(CCore *Core,CExport &Exp);

// Functions
virtual	bool			LButtonControl(CCore *Core,UINT nFlags, CPoint &point,bool DownFlag);
virtual	bool			RButtonControl(CCore *Core,UINT nFlags, CPoint &point,bool DownFlag);
virtual	bool			MouseMove(CCore *Core,UINT nFlags, CPoint &point);
virtual	bool			Command(int CmdMsg,CCore *Core,int Param0=0,int Param1=0);

protected:
		void			RenderThing(CCore *Core,Vector3 &CamPos,sLayerThing	&ThisThing,bool Render3d,bool Selected);
		int				CheckThing(CPoint &Pos);
		void			AddThing(CPoint &Pos);
		void			SelectThing(CPoint &Pos);
		int				CheckThingPoint(CPoint &Pos);
		void			AddThingPoint(CPoint &Pos);
		void			SelectThingPoint(CPoint &Pos);

		void			UpdatePos(CPoint &Pos,int Thing,int PosNo,bool Recurs=false);


		int					Width,Height;
		CElemBank			*ThingBank;
		CList<sLayerThing>	ThingList;
		int					CurrentThing,CurrentPoint;
		MouseMode			Mode;


		CGUIToolBar			GUIToolBar;

};

/*****************************************************************************/
#endif
