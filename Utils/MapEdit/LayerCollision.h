/***********************/
/*** Layer Collision ***/
/***********************/

#ifndef	__LAYER_COLLISION_HEADER__
#define	__LAYER_COLLISION_HEADER__

#include	"Layer.h"
#include	"MapEdit.h"
#include	"LayerCollisionGUI.h"

/*****************************************************************************/
class	CCore;
class	CLayerCollision : public CLayerTile
{

public:
		CLayerCollision(int SubType,int Width,int Height);					// New Layer
		CLayerCollision(CFile *File,int Version);							// Load Layer
		~CLayerCollision();

		int				GetType()			{return(LAYER_TYPE_COLLISION);}

		void			Render(CCore *Core,Vector3 &CamPos,BOOL Is3d);
		BOOL			SetColFlag(CCore *Core,int Flag);

		void			GUIInit(CCore *Core);
		void			GUIKill(CCore *Core);
		void			GUIUpdate(CCore *Core);
		void			GUIChanged(CCore *Core);

		void			Load(CFile *File,int Version);
		void			Save(CFile *File);

		void			Export(CCore *Core,CExport &Exp);

// Functions
		void			DeleteSet(int Set);
		void			RemapSet(int OrigSet,int NewSet);
protected:


		CLayerCollisionGUI	CollisionGUI;

};

/*****************************************************************************/
#endif
