/***********************/
/*** Layer Collision ***/
/***********************/

#ifndef	__LAYER_COLLISION_HEADER__
#define	__LAYER_COLLISION_HEADER__

#include	"Layer.h"
#include	"MapEdit.h"
#include	"GUILayerCollision.h"

/*****************************************************************************/
class	CCore;
class	CLayerCollision : public CLayerTile
{

public:
//		CLayerCollision(int Type,int SubType,int Width,int Height);
		CLayerCollision(sLayerDef &Def);
		CLayerCollision(CFile *File,int Version) {Load(File,Version);}
		~CLayerCollision();

		int				GetType()			{return(LAYER_TYPE_COLLISION);}
		void			InitSubView(CCore *Core);

		void			Render(CCore *Core,Vector3 &CamPos,bool Is3d);

		void			GUIInit(CCore *Core);
		void			GUIKill(CCore *Core);
		void			GUIUpdate(CCore *Core);
		void			GUIChanged(CCore *Core);

		void			Load(CFile *File,int Version);
		void			Save(CFile *File);

		void			Export(CCore *Core,CExport &Exp);
		bool			Command(int CmdMsg,CCore *Core,int Param0,int Param1);

// Functions
		void			DeleteSet(int Set);
		void			RemapSet(int OrigSet,int NewSet);
protected:
		void			InitTileBank();

		GUILayerCollision	GUI;

};

/*****************************************************************************/
#endif
