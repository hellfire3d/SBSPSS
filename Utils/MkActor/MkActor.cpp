/*************************/
/*** Actor Sprite Tool ***/
/*************************/

#include "stdio.h"
#include <misc.hpp>
#include <conio.h>
#include <iostream.h>
#include <vector>
#include <DaveLib.h>
#include <io.h>
#include <SprSet.h>
#include "MkActor.h"

#include	<pak.h>

using namespace std;
//#define	CheckDups	1
//#define	OutputTGA

//***************************************************************************
vector<CMkActor>	ActorList;
int			TPBase=-1,TPWidth=-1,TPHeight=-1;
GString		TPOutStr;

GString		RootPath;
GString		SpritePath;
GString		OutPath;
GString		IncludePath;

//***************************************************************************
void	RepAlign(FILE *File,char *Txt=0)		
{							
/*
int	Align=ftell(File)&3;

	if (Align)
	{
		if (Txt)
			printf("%s MisAlign %i\n",Txt,Align);
		else
			printf("%s MisAlign %i\n","File",Align);

	}
*/
	PadFile(File);	
}

//***************************************************************************
char * CycleCommands(char *String,int Num)
{
char	Text[2048],*TextPtr;
int		Count;

	if (String[0]=='-' || String[0]=='/')
		{
		GString		TpStr;
		switch (String[1])
			{
				// Switches 
			case 'o':
				OutPath = CheckFileString(String);
				OutPath.Upper();
				OutPath.Append('\\');
				break;
			case 'i':
				IncludePath = CheckFileString(String);
				IncludePath.Upper();
				IncludePath.Append('\\');
				break;
			case 'd':
				DebugOn =true;
				break;
			case 's':
				SpritePath=CheckFileString(String);
				SpritePath.Upper();
				SpritePath.Append('\\');
				break;
			case 'r':
				RootPath=CheckFileString(String);
				RootPath.Upper();
				RootPath.Append('\\');
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
				break;
			default:
				GObject::Error(ERR_FATAL,"Unknown switch %s",String);
				break;
			}
		} 
	else
		{
			GString	Name=String;
			Name.Upper();
			ActorList.push_back(CMkActor(Name,RootPath,SpritePath));
		}

	return(String);
}

//***************************************************************************

void Usage(char *ErrStr)
{
	printf("\nMkActor (usese scripts): by Dave\n");
	printf("Usage: MkActor <file> [ <file>.. ] [ switches.. ]\n");
	printf("Switches:\n");
	printf("   -o:[FILE]       Set output Dir\n");
	printf("   -p:nn           Root Dir\n");
	printf("   -s:nn           Sprite Dir\n");
	printf("   -t:p,w,h        Set TPage No,Width,Height\n");
	printf("   -d:             Enable Debug output\n");
	GObject::Error(ERR_FATAL,ErrStr);
}

//***************************************************************************
int	main (int argc, char *argv[])
{
int		i,ListSize;

		CommandLine(argc,argv,CycleCommands);
		if (OutPath.Empty()) Usage("No Output File Set\n");

		ListSize=ActorList.size();
		for (i=0; i<ListSize; i++)
		{
			ActorList[i].Load();
			ActorList[i].Process();
			ActorList[i].Write();
			ActorList[i].CleanUp();
		}

		return 0;
}

//***************************************************************************
//***************************************************************************
//***************************************************************************
CMkActor::CMkActor(GString &ActorName,GString &ActorPath,GString &SpritePath)
{
		Name=ActorName;
		ActorDir=ActorPath+ActorName;
		ActorDir.Append('\\');
		SpriteDir=ActorDir+SpritePath;
		OutFile=OutPath+ActorName;
		IncFile=IncludePath+"ACTOR_"+Name;
		
		TexGrab.SetTPage(TPBase,TPWidth,TPHeight);
		TexGrab.SetOutFile(OutFile+".Tex");
		TexGrab.SetDebugOut(OutFile+".Lbm");
		TexGrab.SetIncFile(IncFile+"_TEX.h");
		TexGrab.NoSort();
		TexGrab.AnimatedHeadersOnly(true);
		TexGrab.DontOutputBoxes(true);
		TexGrab.AllowRotate(false);

		DupCount=0;
}

//***************************************************************************
void	CMkActor::CleanUp()
{
int		i,ListSize=BmpList.size();
	
		for (i=0; i<ListSize; i++)
		{
			if (BmpList[i].RGB) free(BmpList[i].RGB);
			if (BmpList[i].Pak) free(BmpList[i].Pak);
			if (BmpList[i].Psx) free(BmpList[i].Psx);
		}
}


//***************************************************************************
void	CMkActor::Load()
{
		ReadScript(ActorDir+"AnimList.Txt",InAnimList);
}

//***************************************************************************
void	CMkActor::ReadScript(const char *Filename,vector<GString> &List)
{
char	*Script,*Ptr;
int		Size;

		File=fopen(Filename,"rb");
		if (!File) return;
		fseek(File,0,SEEK_END);
		Size=ftell(File);
		fseek(File,0,SEEK_SET);
		Script=(char*)malloc(Size+1);
		ASSERT(Script);
		// Load It
		fread(Script,Size,1,File);
		fclose(File);

// Extract Names
		Ptr=Script;
		Script[Size]=0;
		while (*Ptr)
		{
			GString	Str;

			while (*Ptr=='\n' || *Ptr=='\r'|| *Ptr==' ') *Ptr++; // Skip gaff
			if (*Ptr)
			{
				if (*Ptr=='#')
				{ // Skip commented lines
					while (*Ptr!='\n' && *Ptr) Ptr++;
				}
				else
				{ // Read data
					while (*Ptr!=' ' && *Ptr!='\n' && *Ptr!='\r' && *Ptr)
					{
						Str.Append(*Ptr++);
					}
					List.push_back(Str);
				}
			}
		}

		free(Script);
}

//***************************************************************************
void	CMkActor::BuildFrameList()
{
int		i,ListSize=InAnimList.size();
char	AnimName[256];
		AnimList.resize(ListSize);
		for (i=0; i<ListSize; i++)
		{
			sAnim	&ThisAnim=AnimList[i];
			sprintf(AnimName,"%s",InAnimList[i]);
			if (AnimName[0]=='~')
			{ // VRam Sprite
				ThisAnim.Name=&AnimName[1];
				ThisAnim.VRamFlag=true;
			}
			else
			{ // PAK Sprite
				ThisAnim.Name=&AnimName[0];
				ThisAnim.VRamFlag=false;
			}
			FindFrames(ThisAnim);
		}
}

//***************************************************************************
void	CMkActor::FindFrames(sAnim &ThisAnim)
{
GString		Filename;
_finddata_t Find;
long		FileHandle;
int			Error=0;

			Filename=SpriteDir+Name+"_"+ThisAnim.Name+"*.bmp";
			if( (FileHandle= _findfirst( Filename, &Find)) == -1L )
			{
				GObject::Error(ERR_WARNING,"Cant find Anim %s.\n",ThisAnim.Name);
				return;
			}

			while (Error==0)
			{
				sFrame	NewFrame;
				NewFrame.Filename=Find.name;
				ThisAnim.Frames.push_back(NewFrame);

				Error=_findnext( FileHandle, &Find);
			}
			_findclose( FileHandle);
}

//***************************************************************************
void	CMkActor::LoadFrameList()
{
int		i,ListSize=AnimList.size();
		
		for (i=0; i<ListSize; i++)
		{
			sAnim	&ThisAnim=AnimList[i];
			int		FrameCount=ThisAnim.Frames.size();
			for (int f=0; f<FrameCount; f++)
			{
				printf("%s - Load Anim %2d/%2d\tFrame %2d/%2d\r",Name,i+1,ListSize,f,FrameCount);
				LoadFrame(ThisAnim.Frames[f],ThisAnim.VRamFlag);
			}
		}
		printf("\t\t\t\t\t\t\r");
}

//***************************************************************************
void	CMkActor::LoadFrame(sFrame &ThisFrame,bool VRamFlag)
{
		ThisFrame.FrameIdx=LoadBmp(ThisFrame.Filename,VRamFlag);
}

//***************************************************************************
void	CMkActor::MakePsxGfx(sBmp &Bmp)
{
// Copied from SprSet 
// apparantly only works on 16 colors, so I gotta watch it!!
int		AlignPixels=4;
int		nfW,nfH,nfLineWidthBytes,nfAreaBytes;
Frame	&Frm=Bmp.Bmp;
Rect	OriginalRect;

		OriginalRect=Frm;

		OriginalRect.X=0;
		OriginalRect.Y=0;
		OriginalRect.W=GU_AlignVal(OriginalRect.W,AlignPixels);

		Frm.Crop(OriginalRect);

		nfW=Frm.GetWidth();
		nfH=Frm.GetHeight();
		nfLineWidthBytes=GU_AlignVal(nfW,2)/2;
		nfAreaBytes=nfLineWidthBytes*nfH;
		Bmp.PsxSize=nfAreaBytes;

		Bmp.Psx=(u8*)malloc(nfAreaBytes+16);
		ASSERT(Bmp.Psx);

		for (int y=0;y<nfH;y++)
		{
			for (int x=0;x<nfW;x++)
			{
				u8 * PixAddr;
				u8  ScrNib;

				ScrNib=Frm.SeeData()[y*nfW+x]&0xf;

				PixAddr=&Bmp.Psx[(x/2)+(nfLineWidthBytes*y)];

				if (PixAddr>=&Bmp.Psx[nfAreaBytes]) ASSERT(!"");
				if ((x&1))
				{
					*PixAddr&=0x0f;
					*PixAddr|=ScrNib<<4;
				}
				else
				{
					*PixAddr&=0xf0;
					*PixAddr|=ScrNib;
				}
			}
		}
}

//***************************************************************************
int		CMkActor::LoadBmp(GString &Name,bool VRamFlag)
{
GString	Filename=SpriteDir+Name;

int			BmpListSize=BmpList.size();
sBmp		NewBmp;
SprFrame	&NewFrame=NewBmp.Bmp;
FileInfo	ThisInfo;

//			ThisInfo.SetInfo(name,		CrossHair,	ThisZeroColZero,	MoveUVs,	AllowRotate,	ShrinkToFit,	m_allocateAs16bit);
			ThisInfo.SetInfo(Filename,	false,		true,				false,		false,			true,			false);

			NewBmp.RGB=0;
			NewBmp.Pak=0;
			NewBmp.Psx=0;
			NewBmp.VRamFlag=VRamFlag;

			NewFrame.LoadBMP(Filename);
int			ColorCount=NewFrame.GetNumOfCols();
			if (ColorCount>16)
			{
				GObject::Error(ERR_WARNING,"%s has %i colors.\n",Name,ColorCount);
			}


#ifdef	CheckDups
int			Size=NewFrame.GetWidth()*NewFrame.GetHeight();
			NewBmp.RGB=(u8*)malloc(Size*3);
			ASSERT(NewBmp.RGB);
			NewFrame.MakeRGB(NewBmp.RGB);

// Check for dups (Broken at the mo, ah well)
// Gen Chksum
u8		*RGB=NewBmp.RGB;
		NewBmp.ChkR=NewBmp.ChkG=NewBmp.ChkB=0;
		for (i=0; i<Size; i++)
		{
			NewBmp.ChkR+=*RGB++;
			NewBmp.ChkG+=*RGB++;
			NewBmp.ChkB+=*RGB++;
		}

int		Idx=-1;
// Find existing
		for (int i=0; i<BmpListSize && Idx==-1; i++)
		{
			sBmp	&ThisBmp=BmpList[i];
//			if (ThisBmp.ChkR==NewBmp.ChkR && ThisBmp.ChkG==NewBmp.ChkG && ThisBmp.ChkB==NewBmp.ChkB)
			{
				if (IsImageSame(ThisBmp,NewBmp)) Idx=i;
			}
		}

		if (Idx!=-1)
		{
			DupCount++;
			free(NewBmp.RGB);
			return(Idx);
		}
#endif
		BmpList.push_back(NewBmp);
		BmpList[BmpListSize].Bmp.SetFrameAndInfo(BmpList[BmpListSize].Bmp,ThisInfo,0);
		MakePsxGfx(BmpList[BmpListSize]);


#if	_DEBUG && defined(OutputTGA)
		{
		Frame	&OutF=BmpList[BmpListSize].Bmp;
u8		*TGA=(u8*)malloc(OutF.GetWidth()*OutF.GetHeight()*3);
		ASSERT(TGA);
		OutF.FlipY();
		OutF.MakeRGB(TGA);
		OutF.FlipY();

		char	OutName[256];
		sprintf(OutName,"\\x\\%s.tga",Name);
		SaveTGA(OutName,OutF.GetWidth(),OutF.GetHeight(),TGA,true);
		free(TGA);
		}
#endif

		return(BmpListSize);
}

//***************************************************************************
bool	CMkActor::IsImageSame(sBmp &Bmp0,sBmp &Bmp1)
{
int		W0=Bmp0.Bmp.GetOrigW();
int		H0=Bmp0.Bmp.GetOrigH();
int		W1=Bmp1.Bmp.GetOrigW();
int		H1=Bmp1.Bmp.GetOrigH();
int		Size=W0*H0*3;
u8		*RGB0=Bmp0.RGB;
u8		*RGB1=Bmp1.RGB;

		if (W0!=W1 || H0!=H1) return(false);

		for (int i=0; i<Size; i++)
		{
			if (*RGB0++!=*RGB1++) return(false);

		}
		return(true);
}

//***************************************************************************
//***************************************************************************
//***************************************************************************
void	CMkActor::Process()
{
		BuildFrameList();
		LoadFrameList();
		ProcessFrames();

//		TexGrab.Process();
}


//***************************************************************************
void	CMkActor::ProcessFrames()
{
int		i,ListSize=BmpList.size();
int		TotalIn=0;		
int		TotalOut=0;		

		for (i=0; i<ListSize; i++)
		{
			sBmp	&ThisBmp=BmpList[i];

			printf("%s - Processing Frame %2d\\%2d\r",Name,i+1,ListSize);
			ThisBmp.PakSize=PAK_findPakSize(ThisBmp.Psx,ThisBmp.PsxSize);
			ThisBmp.Pak=(u8*)malloc(ThisBmp.PakSize);
			ASSERT(ThisBmp.Pak);
			PAK_doPak(ThisBmp.Pak,ThisBmp.Psx,ThisBmp.PsxSize);

			TotalIn+=ThisBmp.PsxSize;
			TotalOut+=ThisBmp.PakSize;
		}
		printf("\t\t\t\t\t\r");
		printf("%s - Done. %i Dups  (In %i) (Out %i) (Saved %i)\n",Name,DupCount,TotalIn,TotalOut,TotalIn-TotalOut);
}

//***************************************************************************
//***************************************************************************
//***************************************************************************
void	CMkActor::Write()
{
GString			OutName=OutFile+".SBK";

		File=fopen(OutName,"wb");

// Write Dummy Hdr
		fwrite(&FileHdr,1,sizeof(sSpriteAnimBank),File);
		RepAlign(File,"MainHeader");
// Write Palette
		FileHdr.Palette=(u8*)WritePalette();
		RepAlign(File,"PaletteHdr");
// Write AnimList
		FileHdr.AnimList=(sSpriteAnim*)WriteAnimList();
		RepAlign(File,"AnimListHdr");
// Write FrameList
		FileHdr.FrameList=(sSpriteFrame*)WriteFrameList();
//		RepAlign(File,"FrameHdr");

// Rewrite Header
		fseek(File, 0, SEEK_SET);
		fwrite(&FileHdr,1,sizeof(sSpriteAnimBank),File);
		fclose(File);

		WriteInclude();

}

//***************************************************************************
int		CMkActor::WritePalette()
{
int			Pos=ftell(File);
Palette		&PCPalette=BmpList[0].Bmp.GetPal();
SprPal		PsxPalette;
int			i,Count;
vector<u16>	OutPal;

		PsxPalette=PCPalette;
		PsxPalette.SetPlusColZero(true);

		Count=PsxPalette.GetNumOfCols();
		if (Count<16) Count=16;
//		if (Count>16)
//		{
//			GObject::Error(ERR_WARNING,"Too Many Colors.\n");
//		}
		FileHdr.ColorCount=Count;
		PsxPalette.MakePSXPal(OutPal);
		for (i=0; i<Count; i++)
		{
			u16	ThisCol=OutPal[i];
			fwrite(&ThisCol,1,sizeof(u16),File);
		}
		return(Pos);
}

//***************************************************************************
int		CMkActor::WriteAnimList()
{
int		Pos=ftell(File);
int		i,AnimCount=AnimList.size();
vector<sSpriteAnim>	Hdrs;

// Write Dummy Hdrs
		FileHdr.AnimCount=AnimCount;
		Hdrs.resize(AnimCount);
		for (i=0; i<AnimCount; i++)
		{
			fwrite(&Hdrs[i],1,sizeof(sSpriteAnim),File);
		}
		RepAlign(File,"AnimListHdr");
		
// Write Frame Lists
		for (i=0; i<AnimCount; i++)
		{
			sAnim	&ThisAnim=AnimList[i];
			int		f,FrameCount=ThisAnim.Frames.size();

			Hdrs[i].FrameCount=FrameCount;
			Hdrs[i].Anim=(u16*)ftell(File);

			for (f=0; f<FrameCount; f++)
			{
				sFrame	&ThisFrame=ThisAnim.Frames[f];
				u16	FrameNo=ThisFrame.FrameIdx;
				fwrite(&FrameNo,1,sizeof(u16),File);
			}
			RepAlign(File,"AnimIdx");
			
		}

// ReWrite Headers
int		SavePos=ftell(File);
		fseek(File,Pos,SEEK_SET);
		for (i=0; i<AnimCount; i++)
		{
			fwrite(&Hdrs[i],1,sizeof(sSpriteAnim),File);
		}
		fseek(File,SavePos,SEEK_SET);

		return(Pos);
}

//***************************************************************************
int		CMkActor::WriteFrameList()
{
int		Pos=ftell(File);
int		i,FrameCount=BmpList.size();
vector<sSpriteFrame>	Hdrs;

// Write Dummy Hdrs
		FileHdr.FrameCount=FrameCount;
		Hdrs.resize(FrameCount);
		for (i=0; i<FrameCount; i++)
		{
			fwrite(&Hdrs[i],1,sizeof(sSpriteFrame),File);
		}
		RepAlign(File,"FrameListHdr");
		
// Write Frame Lists
		for (i=0; i<FrameCount; i++)
		{
			sBmp	&ThisBmp=BmpList[i];
			if (ThisBmp.VRamFlag)
			{ // VRam
				GObject::Error(ERR_FATAL,"VRam sprites not supported yet (%s)\n",ThisBmp.Bmp.GetName);
			}
			else
			{ // Pak
				int	XOfs=ThisBmp.Bmp.GetX()-(ThisBmp.Bmp.GetOrigW()/2);
				int	YOfs=ThisBmp.Bmp.GetY()-(ThisBmp.Bmp.GetOrigH());
				Hdrs[i].PAKSpr=(u8*)ftell(File);
				Hdrs[i].XOfs=XOfs;
				Hdrs[i].YOfs=YOfs;
				Hdrs[i].W=ThisBmp.Bmp.GetWidth();
				Hdrs[i].H=ThisBmp.Bmp.GetHeight();
				fwrite(ThisBmp.Pak,1,ThisBmp.PakSize,File);
			}


		}
// ReWrite Headers
int		SavePos=ftell(File);
		fseek(File,Pos,SEEK_SET);
		for (i=0; i<FrameCount; i++)
		{
			fwrite(&Hdrs[i],1,sizeof(sSpriteFrame),File);
		}
		fseek(File,SavePos,SEEK_SET);

		return(Pos);
}

//***************************************************************************
void	CMkActor::WriteInclude()
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
			GString	AnimName=ThisAnim.Name;
			AnimName.Upper();
			fprintf(File,"\tANIM_%s_%s",Name,AnimName);
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
// 