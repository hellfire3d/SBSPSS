/**********************************/
/*** SpongeBob 3d Actor Creator ***/
/**********************************/

#include "stdio.h"
#include <misc.hpp>
#include <conio.h>
#include <iostream.h>
#include <vector>
#include <DaveLib.h>
#include <FaceStore.h>

#include "MkActor3d.h"

using namespace std;

//***************************************************************************
CMkActor3d	ThisActor;
//std::vector<GString>	ExtraTex;

//***************************************************************************

char * CycleCommands(char *String,int Num)
{
int		TPBase=-1,TPWidth=-1,TPHeight=-1;
char	Text[2048],*TextPtr;
int		Count,i;

	if (String[0]=='-' || String[0]=='/')
		{
		GString		TpStr;
		switch (String[1])
			{
				// Switches 
			case 'o':
				OutStr = CheckFileString(String);
				break;
			case 'i':
				IncludeFile = CheckFileString(String);
				break;
			case 'd':
				DebugOn =true;
				break;
			case 's':
				TpStr= CheckFileString(String);
				Scale=atof(TpStr);
				break;
			case 't':
				TpStr= CheckFileString(String);
				TextPtr=Text;
				strcpy(TextPtr,TpStr);
				Count=ZeroAndCountCommas(TextPtr);
				if (Count!=2) GObject::Error(ERR_FATAL,"Problem with option %s\n",String);
				TPBase=atol(TextPtr);
				TextPtr+=strlen(TextPtr)+1;
				TPWidth=atol(TextPtr);
				TextPtr+=strlen(TextPtr)+1;
				TPHeight=atol(TextPtr);
				ThisActor.SetTPData(TPBase,TPWidth,TPHeight);
				break;
			case 'x':
				TpStr= CheckFileString(String);
				TextPtr=Text;
				strcpy(TextPtr,TpStr);
				Count=ZeroAndCountCommas(TextPtr);
				for (i=0; i<Count+1; i++)
				{
					ThisActor.AddTex(TextPtr);
					TextPtr+=strlen(TextPtr)+1;
				}
				break;
			case 'a':
				TpStr= CheckFileString(String);
				TextPtr=Text;
				strcpy(TextPtr,TpStr);
				Count=ZeroAndCountCommas(TextPtr);
				for (i=0; i<Count+1; i++)
				{
					ThisActor.AddAnim(TextPtr);
					TextPtr+=strlen(TextPtr)+1;
				}
				break;
			case 'q':
				StripLength=4;
				break;
			default:
				GObject::Error(ERR_FATAL,"Unknown switch %s",String);
				break;
			}
		} 
	else
		{
		ThisActor.SetInName(String);
		}

	return(String);
}

//***************************************************************************

void Usage(char *ErrStr)
{
	printf("\nMkActor3d: by Dave\n");
	printf("Usage: MkActor3d <file> [ <file>.. ] [ switches.. ]\n");
	printf("Switches:\n");
	printf("   -o:[FILE]       Set output Dir\n");
	printf("   -s:nn           Set Scaling value\n");
	printf("   -t:p,w,h        Set TPage No,Width,Height\n");
	printf("   -d:             Enable Debug output\n");
	printf("   -x:             Add Texture\n");
	printf("   -d:             Add Anim\n");
	printf("   -q:             Enable Quadding\n");
	GObject::Error(ERR_FATAL,ErrStr);
}

//***************************************************************************
int	main (int argc, char *argv[])
{
		CommandLine(argc,argv,CycleCommands);
		if (OutStr.Empty()) Usage("No Output File Set\n");

		ThisActor.ProcessActor();
		ThisActor.ProcessAnim();

		return 0;
}

//***************************************************************************
//***************************************************************************
//***************************************************************************
void	CMkActor3d::SetInName(GString const &String)
{
GString	UpperName(String);
		UpperName.Upper();
GFName	File=UpperName;

		InFilename=UpperName;
		InPath=File.Drive();
		InPath+=File.Dir();
		Name=File.File();

}

//***************************************************************************
void	CMkActor3d::SetTPData(int TPBase,int TPW,int TPH)
{
		TPageBase=TPBase; 
		TPageWidth=TPW ;
		TPageHeight=TPH;
}

//***************************************************************************
void	CMkActor3d::AddAnim(const char *Name)
{
		InAnimList.push_back(Name);
}

//***************************************************************************
void	CMkActor3d::AddTex(const char *Name)
{
		InTexList.push_back(Name);
}

//***************************************************************************
void	CMkActor3d::ProcessActor()
{
		ThisActor.ActorLoad();
		ThisActor.ActorProcess();
		ThisActor.ActorWrite();
}

//***************************************************************************
void	CMkActor3d::ProcessAnim()
{
		ThisActor.AnimLoad();
		ThisActor.AnimWrite();
}

//***************************************************************************
void	CMkActor3d::ActorLoad()
{
		OutStr.Upper();
		OutStr.Append('\\');
		OutFile=OutStr+Name;
		IncludeFile.Upper();
		IncludeFile.Append('\\');
		IncFile=IncludeFile;
		IncFile+="ACTOR_";
		IncFile+=Name;

		Scene.Load(InFilename);
}

//***************************************************************************
//***************************************************************************
//***************************************************************************
void    CMkActor3d::BuildBoneOut(sBone &OutBone,CNode const &InNode,int ParentBoneIdx)
{
		OutBone.BoneSize.vx =round(InNode.Pos.x*Scale);
		OutBone.BoneSize.vy =round(InNode.Pos.y*Scale);
		OutBone.BoneSize.vz =round(InNode.Pos.z*Scale);
		OutBone.Parent=ParentBoneIdx;
		OutBone.VtxCount=0;
		OutBone.TriCount=0;
		OutBone.TriStart=0;
}

//***************************************************************************
void	CMkActor3d::ProcessSkel(int Idx,int ParentIdx)
{
CNode					&ThisNode=Scene.GetNode(Idx);
CNode					&ParentNode=Scene.GetNode(ThisNode.ParentIdx);
vector<sGinTri> const	&NodeTriList =	ThisNode.GetTris();
vector<Vector3> const	&NodeVtxList =	ThisNode.GetPts();
vector<int>	const		&NodeMatList =	ThisNode.GetTriMaterial();
vector<sUVTri> const	&NodeUVList =	ThisNode.GetUVTris();
vector<GString> const	&SceneTexList=	Scene.GetTexList();
vector<int> const		&SceneUsedMatList=Scene.GetUsedMaterialIdx();

int						TriCount=NodeTriList.size();
int						ThisIdx=Skel.size();
sGinSkel				&ParentBone=Skel[ParentIdx];
vector<Vector3>			VtxList;

		if (!TriCount)
		{ // Its a Bone!!
			sGinSkel	ThisBone;
			BuildBoneOut(ThisBone.Bone,ThisNode,ParentIdx);

int			WeightCount=ThisNode.Weights.size();
			if (WeightCount) 
			{
//				printf("%s %i\n",ThisNode.Name,WeightCount);
				for (int i=0; i<WeightCount; i++)
				{
					sGinWeight	&ThisWeight=ThisNode.Weights[i];
//					printf("%i %f %f %f\t\n",ThisWeight.VertNo,NodeVtxList[ThisWeight.VertNo].x,NodeVtxList[ThisWeight.VertNo].y,NodeVtxList[ThisWeight.VertNo].z);
				}
			}
			Skel.push_back(ThisBone);
		}
		else
		{ // Model, attach to parent bone
// build TX Vtx List

			int	ListSize=NodeVtxList.size();
			VtxList.resize(ListSize);
Matrix4x4	PMtx=ParentNode.Mtx;
			PMtx.Invert();
			for (int i=0; i<ListSize; i++)
			{
				VtxList[i]=PMtx*NodeVtxList[i];
			}

			for (int T=0; T<TriCount; T++)
			{
				int		Mat=SceneUsedMatList[NodeMatList[T]];

				if (Mat>SceneTexList.size()) GObject::Error(ERR_FATAL,"Crap Material ID, wanted %i, only have %i\n",Mat,SceneTexList.size());

				ParentBone.FaceList.AddFace( VtxList, NodeTriList[T], NodeUVList[T], SceneTexList[Mat]);
			}
		}

int		ChildCount=ThisNode.GetPruneChildCount();
		for (int Loop=0;Loop<ChildCount ; Loop++) ProcessSkel(ThisNode.PruneChildList[Loop],ThisIdx);

}

//***************************************************************************
void	CMkActor3d::BuildSkelOut()
{
int		ListSize=Skel.size();

		BoneCount=Skel.size();

		for (int i=0; i<ListSize; i++)
		{
			sGinSkel	&ThisBone=Skel[i];
			int			VtxStart=FaceList.GetVtxCount();
			int			FaceListSize=ThisBone.FaceList.GetFaceCount();

			if (FaceListSize)
			{
				ThisBone.Bone.TriStart=FaceList.GetFaceCount();
				ThisBone.Bone.TriCount=ThisBone.FaceList.GetFaceCount();

				for (int F=0; F<FaceListSize; F++)
				{
					FaceList.AddFace(ThisBone.FaceList[F]);
				}
			ThisBone.Bone.VtxCount=FaceList.GetVtxCount()-VtxStart;
			}
		}
}

//***************************************************************************
void	CMkActor3d::WriteSkel()
{
int		ListSize=Skel.size();

		for (int i=0; i<ListSize; i++)
		{
			sBone	&ThisBone=Skel[i].Bone;

			fwrite(&ThisBone, sizeof(sBone), 1, File);
		}
}


//***************************************************************************
void	CMkActor3d::ActorProcess()
{
GString	IncName=IncFile+".h";

		FaceList.SetTexBasePath(InPath);
		FaceList.SetTexOut(OutFile+".Tex",TPageBase,TPageWidth,TPageHeight);
		FaceList.SetTexDebugOut(OutFile+".Lbm");
		if (!IncludeFile.Empty()) FaceList.SetTexInclude(IncName);

		ProcessSkel(1,-1);
		BuildSkelOut();
		printf("%s has %i bones\n",Name,BoneCount);

int		ListSize=InTexList.size();
		for (int i=0; i<ListSize; i++)
		{
			FaceList.AddTex(InTexList[i]);
		}
		
		FaceList.ProcessTextures();
		FaceList.Process();
}


//***************************************************************************
void	CMkActor3d::ActorWrite()
{
GString	OutName=OutFile+".A3d";

		File=fopen(OutName,"wb");

// Write Dummy Hdr
		fwrite(&FileHdr,1,sizeof(sActorHdr),File);

// Write Skeleton
		FileHdr.BoneCount=Skel.size();
		FileHdr.BoneList=(sBone*)ftell(File);
		WriteSkel();

// Write Tris
		FileHdr.TriCount=FaceList.GetTriFaceCount();
		FileHdr.TriList=(sTri*)FaceList.WriteTriList(File);
		printf("%i Tris\n",FileHdr.TriCount);
// Write Quads
		FileHdr.QuadCount=FaceList.GetQuadFaceCount();
		FileHdr.QuadList=(sQuad*)FaceList.WriteQuadList(File);
		printf("%i Quads\n",FileHdr.QuadCount);
// Write VtxList
		FileHdr.VtxCount=FaceList.GetVtxCount();
		FileHdr.VtxList=(sVtx*)FaceList.WriteVtxList(File);
		printf("%i Vtx\n",FileHdr.VtxCount);

// Write TexList
		FileHdr.TexInfo=(sTexInfo*)WriteTexInfoList();

		printf("Size=%i\n",ftell(File));

// Rewrite Header
		fseek(File, 0, SEEK_SET);
		fwrite(&FileHdr,1,sizeof(sActorHdr),File);
}

//***************************************************************************
void	CalcTPXY(sTexOutInfo const &In,sTexInfo &Out)
{
int		TPage=In.Tpage;
int		X,Y,W,H;
int		PixPerWord;

		X=(u8)In.u;
		Y=(u8)In.v;
		W=(u8)In.w;
		H=(u8)In.h;
		
		switch (((TPage)>>7)&0x003)
			{
			case 0:
				PixPerWord=4;
				break;
			case 1:
				PixPerWord=2;
				break;
			case 2:
				PixPerWord=1;
				break;
			default:
				GObject::Error(ERR_FATAL,"Unknown Pixel Depth");
				break;
			};

		X/=PixPerWord;
		W/=PixPerWord;

		Out.x=(TPage<<6)&0x7c0;
		Out.x+=X;
		Out.y=(TPage<<4)&0x100;
		Out.y+=Y;

		Out.w=W;
		Out.h=H;
}

//***************************************************************************
int		CMkActor3d::WriteTexInfoList()
{
CTexGrab					&TexGrab=FaceList.GetTexGrab();
std::vector<sTexOutInfo>	&TexList=TexGrab.GetTexInfo();
int		ListSize=TexList.size();
int		Pos=ftell(File);

		for (int i=0; i<ListSize; i++)
		{
			sTexInfo	OutTex;
			
			CalcTPXY(TexList[i],OutTex);
			fwrite(&OutTex, sizeof(sTexInfo), 1, File);
		}
//		printf("%i Materials\n",ListSize);

		return (Pos);
}


//***************************************************************************
//*** Anim ******************************************************************
//***************************************************************************
void	CMkActor3d::AnimLoad()
{
int		ListSize=InAnimList.size();

		for (int i=0; i<ListSize; i++)
		{
			CScene	Scene;
			int		ThisBoneCount;
			GFName	Name=InAnimList[i];

			printf("%s\t",Name.File());
			Scene.Load(InAnimList[i]);

// Process Anim
			sAnim	ThisAnim;
			ThisAnim.Name=Name.File();
			ThisAnim.Name.Upper();
			ThisAnim.FrameCount=AnimProcessSkelMove(Scene,ThisAnim,1);
			AnimProcessSkelAnim(Scene,ThisAnim,1);
			AnimList.push_back(ThisAnim);

			ThisBoneCount=ThisAnim.BoneAnim.size();
			printf("\t(%i Bones, %i Frames)\n",ThisBoneCount,ThisAnim.FrameCount);

// Check Skeleton
			if (BoneCount!=ThisBoneCount)
			{
				GObject::Error(ERR_FATAL,"Invalid skeleton\n");
			}
		}

}

//***************************************************************************
int		CMkActor3d::AnimProcessSkelMove(CScene &Scene,sAnim &ThisAnim,int Idx)
{
CNode					&ThisNode=Scene.GetNode(Idx);
vector<sGinAnim> const	&NodeAnim=ThisNode.GetAnim();
int						FrameCount=NodeAnim.size();
vector<s32>				&Move=ThisAnim.Move;

		Move.resize(FrameCount);
		for (int i=0; i<FrameCount; i++)
		{
			Move[i]=-round(NodeAnim[i].Pos.y*Scale);
		}

		return(FrameCount);	
}

//***************************************************************************
void	CMkActor3d::AnimProcessSkelAnim(CScene &Scene,sAnim &ThisAnim,int Idx)
{
CNode					&ThisNode=Scene.GetNode(Idx);
vector<sGinAnim> const	&NodeAnim=ThisNode.GetAnim();
int						FrameCount=NodeAnim.size();

//			if (!ThisNode.Pts.size())		// Dont export Skin as bone
			if (!ThisNode.GetTris().size())		// Dont export Skin as bone
			{
				sBoneAnim	FrameList;
				FrameList.Idx.resize(FrameCount);
			
				for (int i=0; i<FrameCount; i++)
				{
					sGinAnim const	&InFrame=NodeAnim[i];
					sQuat			ThisFrame;
					Quaternion	const	&ThisQuat=InFrame.Ang;

					ThisFrame.vx=round(ThisQuat.x*4096);
					ThisFrame.vy=round(ThisQuat.y*4096);
					ThisFrame.vz=round(ThisQuat.z*4096);
					ThisFrame.vw=round(ThisQuat.w*4096);
					FrameList.Idx[i]=QuatList.Add(ThisFrame);
				}
				ThisAnim.BoneAnim.push_back(FrameList);
			}

int			ChildCount=ThisNode.GetPruneChildCount();

			for (int Loop=0;Loop<ChildCount;Loop++)	AnimProcessSkelAnim(Scene,ThisAnim,ThisNode.PruneChildList[Loop]);
}

//***************************************************************************
void	CMkActor3d::AnimWrite()
{
GString			OutName=OutFile+".Abk";
int				Anim,AnimCount=AnimList.size();
sAnimFileHdr	FileHdr;
sAnimHdr		Hdr;

		File=fopen(OutName,"wb");

// Write Dummy FileHdr		
		FileHdr.AnimCount=AnimCount;
		FileHdr.BoneCount=BoneCount;
		fwrite(&FileHdr,1,sizeof(sAnimFileHdr),File);

// Write Dummy AnimHdrs

		for (Anim=0; Anim<AnimCount; Anim++)
		{
			fwrite(&Hdr,1,sizeof(sAnimHdr),File);
		}

// Write QuatTable
		FileHdr.QuatTable=(sQuat*)AnimWriteQuatTable();

// Write Movements
		for (Anim=0; Anim<AnimCount; Anim++)
		{
			AnimList[Anim].MoveOfs=AnimWriteMove(AnimList[Anim]);
		}

// Write Anims (u16 can cause address errors, so write last
		for (Anim=0; Anim<AnimCount; Anim++)
		{
			AnimList[Anim].AnimOfs=AnimWriteAnim(AnimList[Anim]);
		}

// ReWrite FileHdr
		fseek(File, 0, SEEK_SET);
		fwrite(&FileHdr,1,sizeof(sAnimFileHdr),File);

// Rewrite Dummy AnimHdrs

		for (Anim=0; Anim<AnimCount; Anim++)
		{
			Hdr.FrameCount=AnimList[Anim].FrameCount;
			Hdr.Move=(s32*)AnimList[Anim].MoveOfs;
			Hdr.Anim=(u16*)AnimList[Anim].AnimOfs;
			fwrite(&Hdr,1,sizeof(sAnimHdr),File);
		}
	
		fclose(File);
		if (!IncludeFile.Empty()) AnimWriteInclude();
}

//***************************************************************************
int		CMkActor3d::AnimWriteMove(sAnim const &ThisAnim)
{
int		Pos=ftell(File);

		for (int Frame=0; Frame<ThisAnim.FrameCount; Frame++)
		{
			s32		ThisMove=ThisAnim.Move[Frame];
			fwrite(&ThisMove,1,sizeof(s32),File);
//			printf("%i %i %i\n",ThisMove.vx,ThisMove.vy,ThisMove.vz);
		}

		return(Pos);
}

//***************************************************************************
int		CMkActor3d::AnimWriteAnim(sAnim const &ThisAnim)
{
int		Pos=ftell(File);

		for (int Frame=0; Frame<ThisAnim.FrameCount; Frame++)
		{
			for (int Bone=0; Bone<BoneCount; Bone++)
			{
				u16	ThisIdx=ThisAnim.BoneAnim[Bone].Idx[Frame];
				fwrite(&ThisIdx,1,sizeof(u16),File);
			}
		}

		return(Pos);
}

//***************************************************************************
int		CMkActor3d::AnimWriteQuatTable()
{
int		Pos=ftell(File);
int		ListSize=QuatList.size();

		for (int i=0; i<ListSize; i++)
		{
			sQuat	&ThisQuat=QuatList[i];
			fwrite(&ThisQuat,1,sizeof(sQuat),File);
		}

//------------	
		printf("%i Anims\n",AnimList.size());
		return(Pos);
}

//***************************************************************************
void	CMkActor3d::AnimWriteInclude()
{
GString	Filename=IncFile+"_Anim.h";

		File=fopen(Filename,"wt");

		fprintf(File,"// %s Anim Header\n",Name);
		fprintf(File,"\n");
		fprintf(File,"#ifndef\t__ANIM_%s_HEADER__\n",Name);
		fprintf(File,"#define\t__ANIM_%s_HEADER__\n",Name);
		fprintf(File,"\n");
		fprintf(File,"\n");
		fprintf(File,"enum\tANIM_%s_LIST\n",Name);
		fprintf(File,"{\n");

int		ListSize=AnimList.size();
		for (int i=0; i<ListSize; i++)
		{
			sAnim	&ThisAnim=AnimList[i];
			fprintf(File,"\tANIM_%s_%s",Name,ThisAnim.Name);
			if (i==0)
			{
			fprintf(File,"=0");
			}
		fprintf(File,",\n");
		}

		fprintf(File,"\tNUM_ANIM_%s\n",Name);
		fprintf(File,"};\n");
		fprintf(File,"\n");
		fprintf(File,"#endif\n");

		fclose(File);
}
