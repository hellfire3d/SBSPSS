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

		Name=File.File();
// Create Out Filename from inFilename and outdir
		OutFile=OutDir;				
		OutFile.Append('\\');
		OutFile+=File.File(); 
		
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
		FaceList.SetTexOut(GString("\\temp\\Test.tex"),1,1,1);
		FaceList.SetTexDebugOut(GString("\\temp\\Test.Lbm"));

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
		printf("%i Tris\n",FaceList.GetFaceCount());
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

