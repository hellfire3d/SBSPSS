/**********************************/
/*** SpongeBob 3d Anim Creator ***/
/**********************************/

#include "stdio.h"
#include <misc.hpp>
#include <conio.h>
#include <iostream.h>
#include <vector>
#include <PsxLib.h>
#include <FaceStore.h>

#include "MkAnim3d.h"

using namespace std;

int	QuatCount=0;

//***************************************************************************

char * CycleCommands(char *String,int Num)
{
//char	Text[256],*TextPtr;
//int		Count;

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
void	CMkAnim3d::Add(GString const &Filename)
{
CScene	Scene;
int		ThisBoneCount;

		printf("%s\n",Filename);		
		Scene.Load(Filename);
		ThisBoneCount=Scene.GetPruneTreeSize()-2;

// Check Skeleton
		if (BoneCount==-1)
		{
			BoneCount=ThisBoneCount;
		}
		else
		{
			if (BoneCount!=ThisBoneCount)
			{
				GObject::Error(ERR_FATAL,"Invalid skeleton\n");
			}
		}

// Process Anim
sAnim	ThisAnim;
		
		ThisAnim.FrameCount=ProcessSkel(Scene,ThisAnim,1);
		AnimList.push_back(ThisAnim);
}

//***************************************************************************

int		CMkAnim3d::ProcessSkel(CScene &Scene,sAnim &ThisAnim,int Idx)
{
CNode					&ThisNode=Scene.GetNode(Idx);
vector<sGinAnim> const	&NodeAnim=ThisNode.GetAnim();
int						FrameCount=NodeAnim.size();

			if (!ThisNode.Pts.size())		// Dont export Skin as bone
			{
				sBoneAnim	FrameList;
				FrameList.Idx.resize(FrameCount);
			
				for (int i=0; i<FrameCount; i++)
				{
					sGinAnim const	&InFrame=NodeAnim[i];
					sQuat			ThisFrame;
					ThisFrame.vx=round(InFrame.Ang.x*4096);
					ThisFrame.vy=round(InFrame.Ang.y*4096);
					ThisFrame.vz=round(InFrame.Ang.z*4096);
					ThisFrame.vw=round(InFrame.Ang.w*4096);
					FrameList.Idx[i]=QuatList.Add(ThisFrame);
					QuatCount++;
				}
				ThisAnim.BoneAnim.push_back(FrameList);
			}

int			ChildCount=ThisNode.GetPruneChildCount();

			for (int Loop=0;Loop<ChildCount;Loop++)	ProcessSkel(Scene,ThisAnim,ThisNode.PruneChildList[Loop]);

		return(FrameCount);
}

//***************************************************************************
void	CMkAnim3d::Write(GString &Filename)
{
int				Anim,AnimCount=AnimList.size();
sAnim3dFileHdr	FileHdr;
sAnim3dHdr		Hdr;

		File=fopen(Filename,"wb");


// Write Dummy FileHdr		
		FileHdr.AnimCount=AnimCount;
		FileHdr.BoneCount=BoneCount;
		fwrite(&FileHdr,1,sizeof(sAnim3dFileHdr),File);

// Write Dummy AnimHdrs

		for (Anim=0; Anim<AnimCount; Anim++)
		{
			fwrite(&Hdr,1,sizeof(sAnim3dHdr),File);
		}
// Write Anims
		for (Anim=0; Anim<AnimCount; Anim++)
		{
			AnimList[Anim].FileOfs=WriteAnim(AnimList[Anim]);
		}

// Write QuatTable
		FileHdr.QuatTable=(sQuat*)WriteQuatTable();
	
// ReWrite FileHdr
		fseek(File, 0, SEEK_SET);
		fwrite(&FileHdr,1,sizeof(sAnim3dFileHdr),File);

// Rewrite Dummy AnimHdrs

		for (Anim=0; Anim<AnimCount; Anim++)
		{
			Hdr.FrameCount=AnimList[Anim].FrameCount;
			Hdr.Anim=(u16*)AnimList[Anim].FileOfs;
			fwrite(&Hdr,1,sizeof(sAnim3dHdr),File);
		}
	
		fclose(File);

}

//***************************************************************************
int		CMkAnim3d::WriteAnim(sAnim const &ThisAnim)
{
int		Pos=ftell(File);

		for (int Frame=0; Frame<ThisAnim.FrameCount; Frame++)
		{
//			printf("Frame %i/%i\r",Frame,ThisAnim.FrameCount);
			for (int Bone=0; Bone<BoneCount; Bone++)
			{
				u16	ThisIdx=ThisAnim.BoneAnim[Bone].Idx[Frame];
				fwrite(&ThisIdx,1,sizeof(u16),File);
			}
		}

		return(Pos);
}

//***************************************************************************
int		CMkAnim3d::WriteQuatTable()
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
		printf("QuatCount=%i\t=%i\n",QuatCount,QuatCount*sizeof(sQuat));
		printf("ListSize=%i\t=%i\n",ListSize,(ListSize*sizeof(sQuat))+(QuatCount*sizeof(u16)));

		return(Pos);
}

//***************************************************************************
//***************************************************************************
//***************************************************************************

void Usage(char *ErrStr)
{
	printf("\nMkAnim3d: by Dave\n");
	printf("Usage: MkAnim3d <file> [ <file>.. ] [ switches.. ]\n");
	printf("Switches:\n");
	printf("   -o:[FILE]       Set output File\n");
	printf("   -d:             Enable Debug output\n");
	GObject::Error(ERR_FATAL,ErrStr);
}

//***************************************************************************
int	main (int argc, char *argv[])
{
CMkAnim3d	AnimBank;

		CommandLine(argc,argv,CycleCommands);
		if (OutStr.Empty()) Usage("No Output File Set\n");

vector<GString> const &Files = MyFiles.GetFileInfoVector();

		for (int Loop=0; Loop<Files.size(); Loop++) 
		{
			AnimBank.Add(Files[Loop]);
		}

		AnimBank.Write(OutStr);

		return 0;
}
/*
c:\spongebob\graphics\characters\spongebob\sbanim\buttbounce.gin c:\spongebob\graphics\characters\spongebob\sbanim\deathelectric.gin c:\spongebob\graphics\characters\spongebob\sbanim\deathfall.gin c:\spongebob\graphics\characters\spongebob\sbanim\electricshock.gin c:\spongebob\graphics\characters\spongebob\sbanim\fall.gin c:\spongebob\graphics\characters\spongebob\sbanim\fireaim.gin c:\spongebob\graphics\characters\spongebob\sbanim\firerecoill.gin c:\spongebob\graphics\characters\spongebob\sbanim\float.gin c:\spongebob\graphics\characters\spongebob\sbanim\getup.gin c:\spongebob\graphics\characters\spongebob\sbanim\hitground01.gin c:\spongebob\graphics\characters\spongebob\sbanim\hover.gin c:\spongebob\graphics\characters\spongebob\sbanim\idleboots.gin c:\spongebob\graphics\characters\spongebob\sbanim\idlecoral.gin c:\spongebob\graphics\characters\spongebob\sbanim\idlecoral01.gin c:\spongebob\graphics\characters\spongebob\sbanim\idlegeneric01.gin c:\spongebob\graphics\characters\spongebob\sbanim\idlegeneric02.gin c:\spongebob\graphics\characters\spongebob\sbanim\idlegeneric03.gin c:\spongebob\graphics\characters\spongebob\sbanim\idlegeneric04.gin c:\spongebob\graphics\characters\spongebob\sbanim\idlegeneric05.gin c:\spongebob\graphics\characters\spongebob\sbanim\idlelauncher.gin c:\spongebob\graphics\characters\spongebob\sbanim\idlenet.gin -o:\temp\anim.bnk
*/