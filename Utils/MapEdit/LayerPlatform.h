/*******************/
/*** Layer Actor ***/
/*******************/

#ifndef	__LAYER_ACTOR_HEADER__
#define	__LAYER_ACTOR_HEADER__

#include	"LayerThing.h"
#include	"Layer.h"
#include	"MapEdit.h"
#include	"GUIToolbar.h"
#include	"Elem.h"

/*****************************************************************************/
class	CLayerActor : public CLayerThing
{
public:

		CLayerActor(){};
		CLayerActor(int SubType,int Width,int Height);					// New Layer
		CLayerActor(CFile *File,int Version);							// Load Layer
//		~CLayerActor();

		int				GetType()			{return(LAYER_TYPE_ACTOR);}
		void			InitSubView(CCore *Core);

//virtual	void			Render(CCore *Core,Vector3 &CamPos,bool Is3d);
//		void			RenderCursor(CCore *Core,Vector3 &CamPos,bool Is3d);
//		void			FindCursorPos(CCore *Core,Vector3 &CamPos,CPoint &MousePos);

		void			GUIInit(CCore *Core);
		void			GUIKill(CCore *Core);
		void			GUIUpdate(CCore *Core);
		void			GUIChanged(CCore *Core);

		void			Load(CFile *File,int Version);
		void			Save(CFile *File);
		void			LoadThingScript(const char *Filename);

		void			Export(CCore *Core,CExport &Exp);

protected:
		CGUIToolBar			GUIToolBar;

};

/*****************************************************************************/
#endif
