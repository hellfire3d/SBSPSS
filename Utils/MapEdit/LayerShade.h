/*******************/
/*** Layer Shade ***/
/*******************/

#ifndef	__LAYER_SHADE_HEADER__
#define	__LAYER_SHADE_HEADER__

#include	"Layer.h"
#include	"MapEdit.h"
#include	"GUILayerShade.h"

/*****************************************************************************/
class	CCore;
class	CLayerShade : public CLayer
{

public:
		enum
		{
			LAYER_SHADE_RGB_MAX=4,
		};
//		CLayerShade(){};
		CLayerShade(sLayerDef &Def);
		CLayerShade(CFile *File,int Version)	{Load(File,Version);}
		~CLayerShade();

		void			InitLayer(sLayerDef &Def);

//		int				GetType()			{return(LAYER_TYPE_SHADE);}
//		int				GetSubType()		{return(SubType);}

		void			Render(CCore *Core,Vector3 &CamPos,bool Is3d);
		void			RenderGrid(CCore *Core,Vector3 &CamPos,bool Active){};
		void			RenderSelection(CCore *Core,Vector3 &ThisCam){};

		void			RenderCursor(CCore *Core,Vector3 &CamPos,bool Is3d){};

		void			GUIInit(CCore *Core);
		void			GUIKill(CCore *Core);
		void			GUIUpdate(CCore *Core);
		void			GUIChanged(CCore *Core);

		bool			Resize(int Width,int Height);

		void			Load(CFile *File,int Version);
		void			Save(CFile *File);

		void			Export(CCore *Core,CExport &Exp);

// Functions

protected:
		void			Render(CCore *Core,Vector3 &CamPos,CMap &ThisMap,bool Render3d,float Alpha=1.0f,Vector3 *Ofs=0);

		CGUILayerShade	GUI;

		int				Count;
		int				Pos[LAYER_SHADE_RGB_MAX];
		RGBQUAD			RGB[LAYER_SHADE_RGB_MAX];
};

/*****************************************************************************/
#endif
