/********************/
/*** Layer Hazard ***/
/********************/

#ifndef	__LAYER_HAZARD_HEADER__
#define	__LAYER_HAZARD_HEADER__

#include	"LayerThing.h"
#include	"Layer.h"
#include	"MapEdit.h"
#include	"GUILayerHazard.h"
#include	"Elem.h"

/*****************************************************************************/
class	CLayerHazard : public CLayerThing
{
public:
		CLayerHazard(sLayerDef &Def);
		CLayerHazard(CFile *File,int Version)	{Load(File,Version);}

		void			InitLayer(sLayerDef &Def);
		void			InitSubView(CCore *Core);

		void			LoadDefThing(const char *Name,sLayerThing &ThisDef);
		void			LoadOldThing(CFile *File,sLayerThing &ThisThing);

		void			GUIInit(CCore *Core);
		void			GUIKill(CCore *Core);
		void			GUIUpdate(CCore *Core);
		void			GUIChanged(CCore *Core);
		bool			GUIReady();

		void			GUIThingDefClear();
		void			GUIThingUpdate(bool OnlySel=false);
		void			GUIThingPointUpdate(bool OnlySel=false);

		void			Export(CCore *Core,CExport &Exp);
		void			ExportThingData(CCore *Core,CExport &Exp,sLayerThing &ThisThing,sLayerThingData &OutThing);

virtual	bool			CanMirror(){return(true);}
virtual	bool			CanRotate(){return(true);}

protected:
		CGUILayerHazard		GUIHazard;

};

/*****************************************************************************/
#endif
