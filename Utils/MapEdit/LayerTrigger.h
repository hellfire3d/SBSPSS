/*********************/
/*** Layer Trigger ***/
/*********************/

#ifndef	__LAYER_TRIGGER_HEADER__
#define	__LAYER_TRIGGER_HEADER__

#include	"LayerThing.h"
#include	"Layer.h"
#include	"MapEdit.h"
#include	"GUILayerTrigger.h"
#include	"Elem.h"

/*****************************************************************************/
class	CLayerTrigger : public CLayerThing
{
public:

		CLayerTrigger(sLayerDef &Def);
		CLayerTrigger(CFile *File,int Version)	{Load(File,Version);}

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


protected:
		void			RenderThing(CCore *Core,Vector3 &CamPos,sLayerThing	&ThisThing,bool Render3d,bool Selected);
		void			SetThingParams(sLayerThing &Thing);

		CGUILayerTrigger		GUITrigger;

};

/*****************************************************************************/
#endif
