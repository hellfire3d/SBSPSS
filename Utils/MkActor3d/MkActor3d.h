/**********************************/
/*** SpongeBob 3d Actor Creator ***/
/**********************************/

#include <Vector3.h>
#include <List.h>
#include <FaceStore.h>

#include "..\mapedit\ExportHdr.h"

//***************************************************************************
struct	sGinSkel
{
	sBone		Bone;
	CFaceStore	FaceList;
};

//***************************************************************************
struct	sBoneAnim
{
		vector<int>			Idx;
};

//***************************************************************************
struct	sAnim
{
		GString				Name;
		int					FrameCount;
		vector<sBoneAnim>	BoneAnim;
		vector<s32>			Move;
		int					AnimOfs;
		int					MoveOfs;
};

//***************************************************************************
class	CMkActor3d
{
public:
		void				SetInName(GString const &In);
		void				SetTPData(int TPBase,int TPW,int TPH);
		
		void				AddAnim(const char *Name);
		void				AddTex(const char *Name);
		
		void				ProcessActor();
		void				ProcessAnim();
private:
		void				ActorLoad();
		void				ActorProcess();
		void				ActorWrite();

		void				ProcessSkel(int Idx,int Parent);
		void				WriteSkel();

		void				BuildSkelOut();
		void				BuildBoneOut(sBone &OutBone,CNode const &InNode,int ParentBoneIdx);
		int					WriteTexInfoList();


		GString				InFilename,InPath,Name,OutFile,IncFile;
		CScene				Scene;

		vector<sGinSkel>	Skel;
		CFaceStore			FaceList;

		sActorHdr			FileHdr;
		FILE				*File;

		std::vector<GString>	InTexList;
		std::vector<GString>	InAnimList;

		int					TPageBase;
		int					TPageWidth,TPageHeight;

/*Anim*/
		void				AnimLoad();
		void				AnimWrite();
		void				AnimWriteInclude();
		int					AnimProcessSkelMove(CScene &Scene,sAnim &ThisAnim,int Idx);
		void				AnimProcessSkelAnim(CScene &Scene,sAnim &ThisAnim,int Idx);
		int					AnimWriteMove(sAnim const &ThisAnim);
		int					AnimWriteAnim(sAnim const &ThisAnim);
		int					AnimWriteQuatTable();

		int					BoneCount;
		vector<sAnim>		AnimList;
		CList<sQuat>		QuatList;


};

//***************************************************************************
