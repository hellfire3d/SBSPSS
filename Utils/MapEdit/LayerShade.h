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

		void			Render(CCore *Core,Vector3 &CamPos,BOOL Is3d);
		void			RenderGrid(CCore *Core,Vector3 &CamPos,BOOL Active){};
		void			RenderSelection(CCore *Core,Vector3 &ThisCam){};

		void			FindCursorPos(CCore *Core,Vector3 &CamPos,CPoint &MousePos){};
		void			RenderCursor(CCore *Core,Vector3 &CamPos,BOOL Is3d){};

		void			GUIInit(CCore *Core);
		void			GUIKill(CCore *Core);
		void			GUIUpdate(CCore *Core);
		void			GUIChanged(CCore *Core);

		int				GetWidth()			{return(Width);}
		int				GetHeight()			{return(Height);}
		BOOL			Resize(int _Width,int _Height);

		void			Load(CFile *File,int Version);
		void			Save(CFile *File);

		void			Export(CCore *Core,CExport &Exp);

// Functions
		BOOL			SetMode(int NewMode)	{return(false);}
		BOOL			InitMode()				{return(false);}
		BOOL			ExitMode()				{return(false);}
		BOOL			LButtonControl(CCore *Core,UINT nFlags, CPoint &CursorPos,BOOL DownFlag)	{return(false);}
		BOOL			RButtonControl(CCore *Core,UINT nFlags, CPoint &CursorPos,BOOL DownFlag)	{return(false);}
		BOOL			MouseMove(CCore *Core,UINT nFlags, CPoint &CursorPos)					{return(false);}

protected:
		void			Render(CCore *Core,Vector3 &CamPos,CMap &ThisMap,BOOL Render3d,float Alpha=1.0f,Vector3 *Ofs=0);

		int				Width,Height;
		int				SubType;
		
		CLayerShadeGUI	ShadeDlg;

		RGBQUAD			RGB[LAYER_SHADE_RGB_MAX];
};

/*****************************************************************************/
#endif
