/**********************************/
/*** SpongeBob 3d Actor Creator ***/
/**********************************/


#include <Vector3.h>
//#include "Layer.h"
//#include "LayerTile.h"
#include <List.h>
#include <FaceStore.h>

#include "..\mapedit\ExportPSXHdr.h"
/*
struct	sTex
{
	int		TexId;
	int		XOfs,YOfs;
	int		Flags;

	bool operator==(sTex const &v1)	
	{
		if (TexId!=v1.TexId) return(FALSE);
		if (XOfs!=v1.XOfs) return(FALSE);
		if (YOfs!=v1.YOfs) return(FALSE);
		if (Flags!=v1.Flags) return(FALSE);
		return(TRUE);
	}
};
*/

//***************************************************************************
class	CMkActor3d
{
public:
		CMkActor3d(GString const &In,GString const &Out,int TPBase,int TPW,int TPH);

		void			Load();
		void			Process();
		void			Write();

private:
		void			BuildSkin(int Idx=0);

		GString			InFilename,InPath,Name,OutFile,OutDir;

		CScene			Scene;

		CFaceStore		FaceList;
		CList<sTri>		OutTriList;
		CList<sVtx>		OutVtxList;

		int				TPageBase;
		int				TPageWidth,TPageHeight;

};

//***************************************************************************
