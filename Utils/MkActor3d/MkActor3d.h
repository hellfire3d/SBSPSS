/**********************************/
/*** SpongeBob 3d Actor Creator ***/
/**********************************/

#include <Vector3.h>
#include <List.h>
#include <FaceStore.h>

#include "..\mapedit\ExportPSXHdr.h"

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
		void			WriteBone(int Idx);
		void			BuildBoneOut(sBone &OutBone,CNode const &InNode);
		void			BuildWeightOut(sWeight &OutWeight,sGinWeight const &InWeight);
		int				WriteWeightList();

		GString			InFilename,InPath,Name,OutFile,OutDir;

		CScene			Scene;

		CFaceStore		FaceList;
		vector<sWeight> WeightList;
		CList<sTri>		OutTriList;
		CList<sVtx>		OutVtxList;

		sActor3dHdr		FileHdr;
		FILE			*File;

		int				TPageBase;
		int				TPageWidth,TPageHeight;

};

//***************************************************************************
