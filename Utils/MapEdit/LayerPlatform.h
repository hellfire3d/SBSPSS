/*******************/
/*** Layer Platform ***/
/*******************/

#ifndef	__LAYER_PLATFORM_HEADER__
#define	__LAYER_PLATFORM_HEADER__

#include	"LayerThing.h"
#include	"Layer.h"
#include	"MapEdit.h"
#include	"GUILayerPlatform.h"
#include	"Elem.h"

/*****************************************************************************/
class	CLayerPlatform : public CLayerThing
{
public:
		enum
		{
				MoveTypeLinear=0,
				MoveTypeCirular

		};

		CLayerPlatform(sLayerDef &Def);
		CLayerPlatform(CFile *File,int Version)	{Load(File,Version);}

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

protected:
		void			SetThingParams(sLayerThing &Thing);

		CGUILayerPlatform		GUIPlatform;


};

/*****************************************************************************/
#endif
