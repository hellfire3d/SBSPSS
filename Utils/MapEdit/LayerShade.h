/*******************/
/*** Layer Shade ***/
/*******************/

#ifndef	__LAYER_SHADE_HEADER__
#define	__LAYER_SHADE_HEADER__

#include	"Layer.h"
#include	"MapEdit.h"
#include	"LayerShadeGui.h"

/*****************************************************************************/
class	CCore;
class	CLayerShade : public CLayer
{

public:
		enum
		{
			LAYER_SHADE_RGB_MAX=4,
		};
		CLayerShade(){};
		CLayerShade(int SubType,int Width,int Height);					// New Layer
		CLayerShade(CFile *File,int Version);							// Load Layer
		~CLayerShade();

		int				GetType()			{return(LAYER_TYPE_SHADE);}
		int				GetSubType()		{return(SubType);}

		void			Render(CCore *Core,Vector3 &CamPos,bool Is3d);
		void			RenderGrid(CCore *Core,Vector3 &CamPos,bool Active){};
		void			RenderSelection(CCore *Core,Vector3 &ThisCam){};

		void			FindCursorPos(CCore *Core,Vector3 &CamPos,CPoint &MousePos){};
		void			RenderCursor(CCore *Core,Vector3 &CamPos,bool Is3d){};

		void			GUIInit(CCore *Core);
		void			GUIKill(CCore *Core);
		void			GUIUpdate(CCore *Core);
		void			GUIChanged(CCore *Core);

		int				GetWidth()			{return(Width);}
		int				GetHeight()			{return(Height);}
		bool			Resize(int _Width,int _Height);

		void			Load(CFile *File,int Version);
		void			Save(CFile *File);

		void			Export(CCore *Core,CExport &Exp);

// Functions
//		bool			LButtonControl(CCore *Core,UINT nFlags, CPoint &CursorPos,bool DownFlag)	{return(false);}
//		bool			RButtonControl(CCore *Core,UINT nFlags, CPoint &CursorPos,bool DownFlag)	{return(false);}
//		bool			MouseMove(CCore *Core,UINT nFlags, CPoint &CursorPos)					{return(false);}
//		bool			Command(int CmdMsg,CCore *Core,int Param0=0,int Param1=0){};

protected:
		void			Render(CCore *Core,Vector3 &CamPos,CMap &ThisMap,bool Render3d,float Alpha=1.0f,Vector3 *Ofs=0);

		int				Width,Height;
		int				SubType;
		
		CLayerShadeGUI	ShadeDlg;

		int				Count;
		int				Pos[LAYER_SHADE_RGB_MAX];
		RGBQUAD			RGB[LAYER_SHADE_RGB_MAX];
};

/*****************************************************************************/
#endif
