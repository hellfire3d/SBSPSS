/*******************/
/*** Layer Actor ***/
/*******************/

#ifndef	__LAYER_ACTOR_HEADER__
#define	__LAYER_ACTOR_HEADER__

#include	"LayerThing.h"
#include	"Layer.h"
#include	"MapEdit.h"
#include	"GUILayerActor.h"
#include	"Elem.h"

/*****************************************************************************/
class	CLayerActor : public CLayerThing
{
public:
		CLayerActor(sLayerDef &Def);
		CLayerActor(CFile *File,int Version)	{Load(File,Version);}

		void			InitLayer(sLayerDef &Def);
		void			InitSubView(CCore *Core);

		void			GUIInit(CCore *Core);
		void			GUIKill(CCore *Core);
		void			GUIUpdate(CCore *Core);
		void			GUIChanged(CCore *Core);
		bool			GUIReady();

		void			GUIThingDefClear();
		void			GUIThingUpdate(bool OnlySel=false);
		void			GUIThingPointUpdate(bool OnlySel=false);

		void			ExportThing(CExport &Exp,sLayerThing &ThisThing);


protected:
		CGUILayerActor		GUI;

};

/*****************************************************************************/
#endif
