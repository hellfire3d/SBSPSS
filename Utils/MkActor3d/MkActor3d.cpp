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

//***************************************************************************
int		TPBase=-1,TPWidth=-1,TPHeight=-1;

//***************************************************************************

char * CycleCommands(char *String,int Num)
{
char	Text[256],*TextPtr;
int		Count;

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
void	CMkActor3d::Process()
{
		BuildSkin();
		FaceList.SetTexBasePath(InPath);
		FaceList.SetTexOut(OutFile+".Tex",TPageBase,TPageWidth,TPageHeight);
		FaceList.SetTexDebugOut(OutFile+".Lbm");

		FaceList.Process();
}

//***************************************************************************
void	CMkActor3d::BuildSkin(int Idx)
{
CNode	&ThisNode=Scene.GetNode(Idx);
vector<sGinTri> const	&NodeTriList =	ThisNode.GetTris();
vector<Vector3> const	&NodeVtxList =	ThisNode.GetPts();
vector<int>	const		&NodeMatList =	ThisNode.GetTriMaterial();
vector<sUVTri> const	&NodeUVList =	ThisNode.GetUVTris();
vector<GString> const	&SceneTexList=	Scene.GetTexList();
vector<Material> const	&SceneMatList=	Scene.GetMaterials();

int		TriCount=NodeTriList.size();
	
		for (int T=0; T<TriCount; T++)
		{
			int	Mat=NodeMatList[T];
			Mat=SceneMatList[Mat].TexId;
			if (Mat<0) Mat=0;
			CFace &ThisFace=FaceList.AddFace( NodeVtxList, NodeTriList[T], NodeUVList[T], SceneTexList[Mat]);
		}

int		ChildCount=ThisNode.GetPruneChildCount();
		for (int Loop=0;Loop<ChildCount ;Loop++) BuildSkin(ThisNode.PruneChildList[Loop]);

}

//***************************************************************************
void	CMkActor3d::Write()
{
GString	OutName=OutFile+".A3d";

		File=fopen(OutName,"wb");

// Write Dummy Hdr
		fwrite(&FileHdr,1,sizeof(sActor3dHdr),File);

// Write Skeleton
		FileHdr.BoneCount=Scene.GetPruneTreeSize()-1-1;	// Skip Scene & skin
		FileHdr.BoneList=(sBone*)ftell(File);
		WriteBone(1);

// Write Tris
		FileHdr.TriCount=FaceList.GetTriFaceCount();
		FileHdr.TriList=(sTri*)FaceList.WriteTriList(File);
		printf("%i Tris\n",FileHdr.TriCount);
// Write Quads
		FileHdr.QuadCount=FaceList.GetQuadFaceCount();
		FileHdr.QuadList=(sQuad*)FaceList.WriteQuadList(File);
		printf("%i Quads\n",FileHdr.QuadCount);
// Write WeightList
		FileHdr.WeightCount=WeightList.size();
		FileHdr.WeightList=(sWeight*)WriteWeightList();
		printf("%i Weight\n",FileHdr.WeightCount);

		printf("Size=%i\n",ftell(File));

// Rewrite Header
		fseek(File, 0, SEEK_SET);
		fwrite(&FileHdr,1,sizeof(sActor3dHdr),File);


}

//***************************************************************************
void	CMkActor3d::WriteBone(int Idx)
{
sBone		ThisBone;
CNode		&ThisNode=Scene.GetNode(Idx);

			if (!ThisNode.Pts.size())		// Dont export Skin as bone
				{
				BuildBoneOut(ThisBone,ThisNode);
				ThisBone.WeightList=(sWeight*)WeightList.size();
				fwrite(&ThisBone, sizeof(sBone), 1, File);

// build Weight List
				for (int Weight=0; Weight<ThisBone.WeightCount; Weight++)
					{
					sWeight	OutWeight;
					BuildWeightOut(OutWeight,ThisNode.Weights[Weight]);
					WeightList.push_back(OutWeight);
					}
				}
int		ChildCount=ThisNode.GetPruneChildCount();
		for (int Loop=0;Loop<ChildCount;Loop++)	WriteBone(ThisNode.PruneChildList[Loop]);
}

//***************************************************************************

void    CMkActor3d::BuildBoneOut(sBone &OutBone,CNode const &InNode)
{
Vector3		const &Vtx=InNode.Pos;

		OutBone.BoneSize.vx =round(Vtx.x*Scale);
		OutBone.BoneSize.vy =round(Vtx.y*Scale);
		OutBone.BoneSize.vz =round(Vtx.z*Scale);
		OutBone.Idx=InNode.PruneIdx-1;
		OutBone.Parent=InNode.PruneParentIdx-1;
		OutBone.WeightCount=InNode.Weights.size();
}

//***************************************************************************
void    CMkActor3d::BuildWeightOut(sWeight &OutWeight,sGinWeight const &InWeight)
{
		OutWeight.vx=round(InWeight.Pos.x*Scale);
		OutWeight.vy=round(InWeight.Pos.y*Scale);
		OutWeight.vz=round(InWeight.Pos.z*Scale);
		OutWeight.VtxNo=InWeight.VertNo;
}

//***************************************************************************
int		CMkActor3d::WriteWeightList()
{
int		ListSize=WeightList.size();
int		Pos=ftell(File);

		for (int i=0; i<ListSize; i++)
		{
			sWeight		&OutWeight=WeightList[i];
			
			fwrite(&OutWeight, sizeof(sWeight), 1, File);
		}

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

