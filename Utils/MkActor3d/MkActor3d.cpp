/**********************************/
/*** SpongeBob 3d Actor Creator ***/
/**********************************/

#include "stdio.h"
#include <misc.hpp>
#include <conio.h>
#include <iostream.h>
#include <vector>
#include <PsxLib.h>
#include <FaceStore.h>

#include "MkActor3d.h"

using namespace std;

//graphics/Characters/SpongeBob/SpongeBob.Gin -o:out/USA/data/Actors -t:24,1,1 -s:256 
// -a:SBEyesAngry.bmp,SBEyesBlink.bmp,SBEyesDown.bmp,SBEyesfiece.bmp,SBEyesLeft.bmp,SBEyesSheepish.bmp,SBEyesUp.bmp,SBEyesWorried.bmp,SBEyesRight.bmp, SBmouth01.bmp,SBMouthGasp.bmp,SBMouthSheepishSBMouthSmile.bmp,SBMouthTerror.bmp,SBMouthWhistle
//***************************************************************************
int		TPBase=-1,TPWidth=-1,TPHeight=-1;
std::vector<GString>	ExtraTex;

//***************************************************************************

char * CycleCommands(char *String,int Num)
{
char	Text[256],*TextPtr;
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
				if (Count!=2)
						GObject::Error(ERR_FATAL,"Problem with option %s\n",String);
				TPBase=atol(TextPtr);
				TextPtr+=strlen(TextPtr)+1;
				TPWidth=atol(TextPtr);
				TextPtr+=strlen(TextPtr)+1;
				TPHeight=atol(TextPtr);
				break;
			case 'a':
				TpStr= CheckFileString(String);
				TextPtr=Text;
				strcpy(TextPtr,TpStr);
				Count=ZeroAndCountCommas(TextPtr);
				for (i=0; i<Count+1; i++)
				{
					ExtraTex.push_back(TextPtr);
					TextPtr+=strlen(TextPtr)+1;
		
				}

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
		GString	UpperName(String);
		UpperName.Upper();
		MyFiles.AddFile(UpperName);
		}

	return(String);
}

//***************************************************************************
//***************************************************************************
//***************************************************************************
CMkActor3d::CMkActor3d(GString const &In,GString const &Out,int TPBase,int TPW,int TPH)
{
GFName	File=In;

		InFilename=In;
		InPath=File.Drive();
		InPath+=File.Dir();
		OutDir=Out;
		OutDir.Append('\\');

		Name=File.File();
// Create Out Filename from inFilename and outdir
		OutFile=OutDir+File.File();
//		OutFile+=File.File(); 
		
		TPageBase=TPBase; 
		TPageWidth=TPW ;
		TPageHeight=TPH;
}

//***************************************************************************
void	CMkActor3d::Load()
{
		Scene.Load(InFilename);
//		Scene.PrintPruneTree();
}

//***************************************************************************
//***************************************************************************
//***************************************************************************
void    CMkActor3d::BuildBoneOut(sBone &OutBone,CNode const &InNode,int ParentBoneIdx)
{
		OutBone.BoneSize.vx =round(InNode.Pos.x*Scale);
		OutBone.BoneSize.vy =round(InNode.Pos.y*Scale);
		OutBone.BoneSize.vz =round(InNode.Pos.z*Scale);
		OutBone.BoneAng.vx=round(InNode.Ang.x*4096);
		OutBone.BoneAng.vy=round(InNode.Ang.y*4096);
		OutBone.BoneAng.vz=round(InNode.Ang.z*4096);
		OutBone.BoneAng.vw=round(InNode.Ang.w*4096);
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
vector<Vector3> const	&NodeVtxList =	ThisNode.GetRelPts();
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
				printf("%s %i\n",ThisNode.Name,WeightCount);
				for (int i=0; i<WeightCount; i++)
				{
					sGinWeight	&ThisWeight=ThisNode.Weights[i];
					printf("%i %f %f %f\t\n",ThisWeight.VertNo,NodeVtxList[ThisWeight.VertNo].x,NodeVtxList[ThisWeight.VertNo].y,NodeVtxList[ThisWeight.VertNo].z);
					ThisBone.FaceList.AddVtx((Vector3) NodeVtxList[ThisWeight.VertNo]);
				}
			printf("%i\n",ThisBone.FaceList.GetVtxCount());
			}

			Skel.push_back(ThisBone);

		}
		else
		{ // Model, attach to parent bone
// build TX Vtx List
			int	ListSize=NodeVtxList.size();
			VtxList.resize(ListSize);
Matrix4x4	Mtx=ThisNode.Mtx;
Matrix4x4	PMtx=ParentNode.Mtx;
			PMtx.Invert();
			for (int i=0; i<ListSize; i++)
			{
				Vector3	Vtx=NodeVtxList[i];
				Vtx=Mtx*Vtx;
				Vtx=PMtx*Vtx;
				VtxList[i]=Vtx;
			}

int			WeightCount=ParentNode.Weights.size();
			if (WeightCount) 
			{
				printf("%s %i\n",ParentNode.Name,WeightCount);
				for (int i=0; i<WeightCount; i++)
				{
					sGinWeight	&ThisWeight=ParentNode.Weights[i];
					printf("%i %f %f %f\t\n",ThisWeight.VertNo,VtxList[ThisWeight.VertNo].x,VtxList[ThisWeight.VertNo].y,VtxList[ThisWeight.VertNo].z);
				}
//			printf("%i\n",ThisBone.FaceList.GetVtxCount());
			}

			for (int T=0; T<TriCount; T++)
			{
				int		Mat=SceneUsedMatList[NodeMatList[T]];

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
//					FaceList.ProcessVtx(ThisBone.FaceList[F]);
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
//			printf("%i %i %i\n",i,ThisBone.TriStart,ThisBone.TriCount);
//			printf("%i %i\n",i,ThisBone.VtxCount);
		}
}


//***************************************************************************
void	CMkActor3d::Process()
{
		ProcessSkel(1,-1);
		BuildSkelOut();
		printf("Skel has %i bones\n",Skel.size());

		FaceList.SetTexBasePath(InPath);
		FaceList.SetTexOut(OutFile+".Tex",TPageBase,TPageWidth,TPageHeight);
		FaceList.SetTexDebugOut(OutFile+".Lbm");

int		ListSize=ExtraTex.size();
		for (int i=0; i<ListSize; i++)
		{
			FaceList.AddTex(ExtraTex[i]);
		}
		
		FaceList.ProcessTextures();
		FaceList.Process();
}


//***************************************************************************
void	CMkActor3d::Write()
{
GString	OutName=OutFile+".A3d";

		File=fopen(OutName,"wb");

// Write Dummy Hdr
		fwrite(&FileHdr,1,sizeof(sActor3dHdr),File);

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
		fwrite(&FileHdr,1,sizeof(sActor3dHdr),File);
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
		printf("%i Materials\n",ListSize);

		return (Pos);
}

//***************************************************************************
//***************************************************************************
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
	printf("   -q:             Enable Quadding\n");
	GObject::Error(ERR_FATAL,ErrStr);
}

//***************************************************************************
int	main (int argc, char *argv[])
{
		CommandLine(argc,argv,CycleCommands);
		if (OutStr.Empty()) Usage("No Output File Set\n");

		if (TPBase==-1) Usage("No TPage Set\n");

vector<GString> const &Files = MyFiles.GetFileInfoVector();

		for (int Loop=0; Loop<Files.size(); Loop++) 
		{
			CMkActor3d	ThisActor(Files[Loop],OutStr,TPBase,TPWidth,TPHeight);

			ThisActor.Load();
			ThisActor.Process();
			ThisActor.Write();
		}
		return 0;
}

