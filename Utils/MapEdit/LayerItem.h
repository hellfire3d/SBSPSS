/*******************/
/*** Layer Item ***/
/*******************/

#ifndef	__LAYER_ITEM_HEADER__
#define	__LAYER_ITEM_HEADER__

#include	"LayerThing.h"
#include	"Layer.h"
#include	"MapEdit.h"
#include	"Elem.h"

/*****************************************************************************/
class	CLayerItem : public CLayerThing
{
public:
		CLayerItem(sLayerDef &Def);
		CLayerItem(CFile *File,int Version)	{Load(File,Version);}

		void			InitLayer(sLayerDef &Def);
		void			InitSubView(CCore *Core);

		void			GUIInit(CCore *Core);
		void			GUIKill(CCore *Core);
		void			GUIUpdate(CCore *Core);
		void			GUIChanged(CCore *Core);
		bool			GUIReady();

		void			GUIThingDefClear();
		void			GUIThingUpdate(bool OnlySel=false);

protected:

};

/*****************************************************************************/
#endif
