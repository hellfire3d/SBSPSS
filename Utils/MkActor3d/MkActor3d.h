/**********************************/
/*** SpongeBob 3d Actor Creator ***/
/**********************************/

#include <Vector3.h>
#include <List.h>
#include <FaceStore.h>

#include "..\mapedit\ExportPSXHdr.h"

struct	sGinSkel
{
	sBone		Bone;
	CFaceStore	FaceList;
};

//***************************************************************************
class	CMkActor3d
{
public:
		CMkActor3d(GString const &In,GString const &Out,int TPBase,int TPW,int TPH);

		void				Load();
		void				Process();
		void				Write();

private:
		void				ProcessSkel(int Idx,int Parent);
		void				WriteSkel();

		void				BuildSkelOut();
		void				BuildBoneOut(sBone &OutBone,CNode const &InNode,int ParentBoneIdx);
		int					WriteTexInfoList();

		GString				InFilename,InPath,Name,OutFile,OutDir;

		CScene				Scene;

		vector<sGinSkel>	Skel;

		CFaceStore			FaceList;

		CTexGrab			TexGrab;
		sActor3dHdr			FileHdr;
		FILE				*File;

		int					TPageBase;
		int					TPageWidth,TPageHeight;

};

//***************************************************************************
