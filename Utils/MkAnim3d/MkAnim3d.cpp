/**********************************/
/*** SpongeBob 3d Anim Creator ***/
/**********************************/

#include "stdio.h"
#include <misc.hpp>
#include <GFName.hpp>
#include <conio.h>
#include <iostream.h>
#include <vector>
#include <DaveLib.h>
#include <FaceStore.h>

#include "MkAnim3d.h"

using namespace std;

int	QuatCount=0;
/*
buttbounceend.gin buttbouncestart.gin deathbackwards.gin deathdry.gin deathfall.gin deathforwards.gin deathspin.gin deathtar.gin electricshock.gin electricshockend.gin electricshockstart.gin faceback.gin facefront.gin fall.gin getup.gin getuprun.gin hitground.gin hover.gin hoverend.gin hoverstart.gin idlebreathe.gin idlehoola.gin idlelook.gin idlewigglearm.gin jumpend.gin karate.gin knockback.gin knockforward.gin run.gin runjumpend.gin runjumpstart.gin runstart.gin runstop.gin soakup.gin talk01.gin talk02.gin talk03.gin talk04.gin teeterback.gin teeterfront.gin
*/

/* duff anims
deathfall.gin
runjumpend.gin
runjumpstart.gin

*/
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
			case 's':
				TpStr= CheckFileString(String);
				Scale=atof(TpStr);
				break;
			case 'i':
				IncludeFile= CheckFileString(String);
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
GFName	Name=Filename;

		printf("%s\t",Name.File());
		Scene.Load(Filename);

// Process Anim
sAnim	ThisAnim;
		ThisAnim.Name=Name.File();
		ThisAnim.Name.Upper();
		ThisAnim.FrameCount=ProcessSkelMove(Scene,ThisAnim,1);
		ProcessSkelAnim(Scene,ThisAnim,1);
		AnimList.push_back(ThisAnim);

		ThisBoneCount=ThisAnim.BoneAnim.size();
		printf("\t(%i Bones, %i Frames)\n",ThisBoneCount,ThisAnim.FrameCount);

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

}

//***************************************************************************
int		CMkAnim3d::ProcessSkelMove(CScene &Scene,sAnim &ThisAnim,int Idx)
{
CNode					&ThisNode=Scene.GetNode(Idx);
vector<sGinAnim> const	&NodeAnim=ThisNode.GetAnim();
int						FrameCount=NodeAnim.size();
vector<s32>				&Move=ThisAnim.Move;

		Move.resize(FrameCount);
		for (int i=0; i<FrameCount; i++)
		{
//			Move[i].vx=round(NodeAnim[i].Pos.x*Scale);
			Move[i]=-round(NodeAnim[i].Pos.y*Scale);
//			Move[i].vz=round(NodeAnim[i].Pos.z*Scale);
		}

		return(FrameCount);	
}

//***************************************************************************
void	CMkAnim3d::ProcessSkelAnim(CScene &Scene,sAnim &ThisAnim,int Idx)
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
					QuatCount++;
				}
				ThisAnim.BoneAnim.push_back(FrameList);
			}

int			ChildCount=ThisNode.GetPruneChildCount();

			for (int Loop=0;Loop<ChildCount;Loop++)	ProcessSkelAnim(Scene,ThisAnim,ThisNode.PruneChildList[Loop]);
}

//***************************************************************************
void	CMkAnim3d::Write(GString &Filename)
{
int				Anim,AnimCount=AnimList.size();
sAnimFileHdr	FileHdr;
sAnimHdr		Hdr;

		File=fopen(Filename,"wb");


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
		FileHdr.QuatTable=(sQuat*)WriteQuatTable();

// Write Movements
		for (Anim=0; Anim<AnimCount; Anim++)
		{
			AnimList[Anim].MoveOfs=WriteMove(AnimList[Anim]);
		}

// Write Anims (u16 can cause address errors, so write last
		for (Anim=0; Anim<AnimCount; Anim++)
		{
			AnimList[Anim].AnimOfs=WriteAnim(AnimList[Anim]);
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

}

//***************************************************************************
int		CMkAnim3d::WriteMove(sAnim const &ThisAnim)
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
int		CMkAnim3d::WriteAnim(sAnim const &ThisAnim)
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
void	CMkAnim3d::WriteInclude(GString const &Filename)
{
GString	Upper=Filename;
		Upper.Upper();
GFName	Name=Upper;

		File=fopen(Filename,"wt");

		fprintf(File,"// %s Header\n",Name.File());
		fprintf(File,"\n");
		fprintf(File,"#ifndef\t__ANIM_%s_HEADER__\n",Name.File());
		fprintf(File,"#define\t__ANIM_%s_HEADER__\n",Name.File());
		fprintf(File,"\n");
		fprintf(File,"\n");
		fprintf(File,"enum\tANIM_%s_LIST\n",Name.File());
		fprintf(File,"{\n");

int		ListSize=AnimList.size();
		for (int i=0; i<ListSize; i++)
		{
			sAnim	&ThisAnim=AnimList[i];
			fprintf(File,"\tANIM_%s_%s",Name.File(),ThisAnim.Name);
			if (i==0)
			{
			fprintf(File,"=0");
			}
		fprintf(File,",\n");
		}

		fprintf(File,"\tNUM_ANIM_%s",Name.File());
		fprintf(File,"};\n");
		fprintf(File,"\n");
		fprintf(File,"#endif\n");

		fclose(File);
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
	printf("   -s:             Set Scale\n");
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

		if (!IncludeFile.Empty())
		{
			AnimBank.WriteInclude(IncludeFile);
		}

		return 0;
}
