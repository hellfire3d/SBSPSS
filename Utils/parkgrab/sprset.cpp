/*=========================================================================

	SPRSET.CPP	

	Author:		Gary Liddon @ Fareham
	Created:
	Project:	TPW Parkgrab
	Purpose:	Object that reads in all the frames for sprites
				processes them into tpages and spits it all out to
				disk
							
	Copyright (c) 1998 G R Liddon

===========================================================================*/

/*----------------------------------------------------------------------
	Includes
	-------- */

/*	Std Lib
	------- */
#include <algorithm>
#include <conio.h>
#include <math.h>

/*	Glib
	---- */
#include <gfname.hpp>
#include <dpanim.hpp>
#include <misc.hpp>
#include <gutils.h>
#include <tquant.h>

/*	Local
	----- */
#include "sprset.h"
#include "cross.h"

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

using namespace std;

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

/*----------------------------------------------------------------------
	Function Prototypes
	------------------- */

/*----------------------------------------------------------------------
	Vars
	---- */

/*----------------------------------------------------------------------
	Data
	---- */

static const char BadFileChars[] =
{
	'+',	'-',	'*',	'/',	'\\',	'#',	',',
	'.',	'(',	')',	'!',	'"',	'£',	'$',
	'%',	'^',	'&',	'=',	'#',	':',	';',	'<',
	'>',	'?',	'@',	'{',	'}',	'[',	']',	'¬',
};

static const int	nbBadFileChars = (sizeof(BadFileChars) / sizeof(char));

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
GString  ReplaceBadFileChars(GString s)
{
	GString out;
	const char *textin = (const char*)s;
	char *textout = new char[s.Len()+1];
	memset(textout, 0, s.Len()+1);
	int i, p;

	p=0;
	for (i=0; i<s.Len(); i++)
	{
		char	ic;

		ic=textin[i];

		for (int c=0;c<nbBadFileChars;c++)
		{
			if (ic == BadFileChars[c])	ic = '_';
		}
		textout[i] = ic;
	}
	out = textout;
	delete textout;
	return out;
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void SprSet::AddFiles(FIVec const & FileList)
{
	FIVecConstIt	It;

	for (It=FileList.begin();It !=FileList.end();It++)
		AddFile(*It);
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:	Add and process a file from the file info
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void SprSet::AddFile(FileInfo const & ThisInfo)
{
	GFName	FileName(ThisInfo.GetActualFileName());

	GString Ext(FileName.Ext());
	Ext.Upper();

	if (Ext == "LBM" || Ext == "BMP")
		AddLbm(ThisInfo);
	else if (Ext == "ANM")
		AddAnm(ThisInfo);
	else
		Error(ERR_FATAL,"Don't deal with these sort of files : %s", FileName.FullName());
}
/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void SprSet::AddFrame(Frame const & Fr,FileInfo const & ThisInfo)
{


	AllSprFrames.resize(AllSprFrames.size()+1);
	SprFrame & AddFr=AllSprFrames[AllSprFrames.size()-1];
	AddFr.SetFrameAndInfo(Fr,ThisInfo,MaxSize);

}

/*----------------------------------------------------------------------
	Function:	
	Purpose:	Add an LBM file process it according to the file info
	Params:
	Returns:
  ---------------------------------------------------------------------- */

void SprSet::AddLbm(FileInfo const & ThisInfo)
{
	Frame	MyFrame;
	GString	Ext;
	GFName	Fname(ThisInfo.GetActualFileName());
	Ext=Fname.Ext();
	Ext.Upper();


	
	if (Ext=="LBM")
		MyFrame.LoadLbm(ThisInfo.GetActualFileName());
	else if (Ext =="BMP")
		MyFrame.LoadBMP(ThisInfo.GetActualFileName());

	AddFrame(MyFrame,ThisInfo);
}




/*----------------------------------------------------------------------
	Function:	
	Purpose:	Add an ANM file process it according to the file info
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void SprSet::AddAnm(FileInfo const & ThisInfo)
{
	DpAnimFilter	Filter;
	GAnim			ThisAnim;
	FileInfo		TempFileInfo;

	TempFileInfo=ThisInfo;

	ThisAnim.Load(Filter,ThisInfo.GetActualFileName());

	GFName ThisName(ThisInfo.GetFileName());

	GString MyStr(ThisName.File());
	MyStr+="_%04d";

	for (int f=0;f<ThisAnim.NumOfFrames();f++)
		{
		char NameBuff[200];
		sprintf(NameBuff,MyStr,f);
		TempFileInfo.SetName(NameBuff);
		AddFrame(ThisAnim[f],TempFileInfo);
		}

	cout<<"Added anim file "<<ThisInfo.GetFileName()<<" ("<<ThisAnim.NumOfFrames()<<" frames)"<<endl;
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:	Get a list of unique pals from the frames we have
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void SprSet::ProcessPals(void)
{
	SprFrIt it;

	/*
		Reduce each frame's palette
		give each frame a paletteindex
		Also make each zero entry of the palette black
	*/

	for (it=AllSprFrames.begin();it!=AllSprFrames.end();it++)
		{
		SprFrame & frm=(*it);

		frm.ReducePal();

		if (frm.GetZeroColZero() || frm.GetPlusColZero())
			{
			frm.GetPal()[0]=Colour(0,0,0);
			}
		}

	
	/* Create a list of unique palettes from the frames we've got and then
		give each frame a paletteindex
	*/

	for (it=AllSprFrames.begin();it!=AllSprFrames.end();it++)
		{
		SprPalVecIt	PalIt;
		int			PalIndex;

		/* Run through the palettes we already have to see if it's already there */

		Palette const & ThisPal=(*it).GetPal();

		PalIndex=-1;

		for (PalIt=AllSprPals.begin();PalIt != AllSprPals.end() && PalIndex==-1;PalIt++)
			{
			if (ThisPal.IsIntersecting(*PalIt))
				{
				if (ThisPal.GetNumOfCols() > (*PalIt).GetNumOfCols())
					(*PalIt)=ThisPal;
	
				PalIndex=PalIt-AllSprPals.begin();
				}
			}

		/* If Palindex == -1 then we couldn't find the palette we wanted so add this one to the pool */

		if (PalIndex==-1)
			{
			AllSprPals.resize(AllSprPals.size()+1);
			SprPal & ThisSprPal=AllSprPals[AllSprPals.size()-1];
			PalIndex=AllSprPals.size()-1;
			ThisSprPal=ThisPal;
			ThisSprPal.SetPlusColZero((*it).GetPlusColZero());
			ThisSprPal.SetZeroColZero((*it).GetZeroColZero());
			ThisSprPal.SetName((*it).GetName());
			}

		/* Set the sprframe to have the write pal index */

		(*it).SetPalIndex(PalIndex);
		AllSprPals[PalIndex].SetPalIndex(PalIndex);
		}

	cout<<"Found "<<AllSprPals.size()<<" unique pal(s)"<<endl;
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:	Allocate all processed frames into tpages
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void SprSet::WriteLBM(char const * FileName)
{
	if (Vi)
		Vi->SaveAs16ColLbm(FileName);
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
#include <direct.h>

void SprSet::WriteReport(char const * File)
{
	const		int DirLen=1024;
	char		Dir[DirLen+1];
	GString		CurrDir;

	CurrDir=getcwd(Dir,DirLen);
	CurrDir.Upper();
	CurrDir=CurrDir+"\\";

	GString	OutFileName(File);
	OutFileName.Lower();

	Gofstream Out;

	Out.open(OutFileName,ios::out);

	if (Out)
		{
		Out<<"File\tTPage\tClut\tu\tv\tw\th\tRotated"<<endl;

		for (int f=0;f<AllSprFrames.size();f++)
			{
			GString		RelName;
			u16			Clut;
			u16			Tpage;
			int			Rotated;
			GString		Name;
			int			u,v,w,h;

			SprFrame & Frm =AllSprFrames[f];

			Clut=Frm.GetClut();
			Tpage=Frm.GetTpage();
			Rotated=Frm.IsRotated()==true ? 1 : 0;
			Name=Frm.GetFileInfo()->GetActualName();
			u=Frm.GetTPRect().X;
			v=Frm.GetTPRect().Y;
			w=Frm.GetWidth();
			h=Frm.GetHeight();

			RelName=GFName::makerelative(CurrDir,Name,Dir);

			Out<<(char const *)RelName<<"\t$"<<hex<<Tpage<<"\t$"<<Clut<<"\t$"<<u<<"\t$"<<v<<"\t$"<<w<<"\t$"<<h<<" "<<Rotated<<endl;
			}

		Out.close();
		}
	else
		Error(ERR_FATAL,"Couldn't open file %s for output",(char const *)OutFileName);
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:	Allocate all processed frames into tpages
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void SprSet::Write(char const * File,int TpNumber,int WidthInTpages,int HeightInTpages)
{
	int	HeightInPixels;

	if (m_halfTpage)
	{
		HeightInPixels = 128;
	}
	else
	{
		HeightInPixels = HeightInTpages * 256;
	}

	ProcessPals();

	VRAM MyVram(WidthInTpages,HeightInPixels);

	/* Add all the frames to a Tpage object */
	AddFramesAndPalsToVRAM(MyVram,TpNumber,WidthInTpages,HeightInPixels);

	/* Now make the output VRAM image */

	int f;

	Vi=new VRAMImage(WidthInTpages,HeightInPixels);

	if (!Vi)
		Error(ERM_OUTOFMEM);

	for (f=0;f<AllSprFrames.size();f++)
		Vi->PlotFrame(AllSprFrames[f]);

	for (f=0;f<AllSprPals.size();f++)
		Vi->PlotPal(AllSprPals[f]);

	/*
	struct SpriteBankHdr
	{
		u16			NumOfSprites;
		u16			NumOfTPages;
		
		u16			TpageStart;
		u16			WidthInTpages;
		
		u16			HeightInTpages;
		u16			NumOfSpareBoxes;
	};
	*/

	Gofstream Out(Gofstream::LITTLE_ENDIAN);

	GString	OutFileName(File);
	OutFileName.Lower();

	Out.open(OutFileName,ios::out|ios::binary);

	TPRectVec	spareVRAM;

	MyVram.getUnusedSpace(spareVRAM);

	if (Out)
		{
int	FrameCount=AllSprFrames.size();
		if (m_AnimatedHeadersOnly)
			{
			FrameCount=0;
			for (f=0;f<AllSprFrames.size();f++)
				if (AllSprFrames[f].IsAnimated()) FrameCount++;

			}
		Out.Put16(FrameCount);

		if (m_noWriteTpages)
			Out.Put16(0);
		else
			Out.Put16(WidthInTpages*HeightInTpages);

		Out.Put16(TpNumber);

		if (m_noWriteTpages)
			{
			Out.Put16(0);
			Out.Put16(0);
			}
		else
			{
			Out.Put16(WidthInTpages);
			Out.Put16(HeightInPixels);
			}
// Dont output spare boxes
		if (m_DontOutputBoxes)
			Out.Put16(0);
		else
			Out.Put16(spareVRAM.size());
		Out.Put16(m_compressTpages ? 1 : 0);
		Out.Put16(0);

		Out.Align(sizeof(u32));

		int		f;


		int		thisPos,size;

		thisPos=Out.tellp();
		if (m_AnimatedHeadersOnly)
			{
			printf("Writing only animated headers (%i).\n",FrameCount);
			for (f=0;f<AllSprFrames.size();f++)	// Write Animated headers only
				{
				if (AllSprFrames[f].IsAnimated()) AllSprFrames[f].WriteHeader(Out);
				}
			}
		else
			{
			printf("Writing All headers (%i).\n",AllSprFrames.size());
			for (f=0;f<AllSprFrames.size();f++)	// Write all headers
				{
				AllSprFrames[f].WriteHeader(Out);
				}

			}

		size=int(Out.tellp())-thisPos;
		cout<<"size of headers == "<<size<<endl;

		Out.Align(sizeof(u32));

		if (m_DontOutputBoxes)
			{
			printf("Skipping spare boxes.\n");
			}
		else
			{
			for (f=0;f<spareVRAM.size();f++)
				{
				Out.Put16(spareVRAM[f].X);
				Out.Put16(spareVRAM[f].Y);
				Out.Put16(spareVRAM[f].W);
				Out.Put16(spareVRAM[f].H);
				}
			Out.Align(sizeof(u32));
			}

		if (m_AlignHeaders)
			{
			printf("Aligning headers to 2k chunk.\n");
			Out.Align(2048);
			}
		
		if (!m_noWriteTpages)
			{
			Vi->setDoCompress(m_compressTpages);
			Vi->Write(Out);
			}

		Out.close();
		}
	else
		Error(ERR_FATAL,"Error wring file %s",(char const *)OutFileName);

	WriteHeaderFile(GetHeaderFileName(File));
}


GString	SprSet::GetHeaderFileName(char const * File)
{
	GString	HeaderFileName;

	if (IncOutFile == "")
		{
		GFName	HeaderFile(File);
		HeaderFile.Ext("h");
		HeaderFileName=HeaderFile.FullName();
		}
	else
		HeaderFileName=IncOutFile;

	HeaderFileName.Lower();

	return(HeaderFileName);
}

void SprSet::WriteHeaderFile(char const * HName)
{
	ofstream HOut;
	int			f;

	GFName	HeaderFile(HName);
	
	GString OFileDefine;
	OFileDefine="__SPR_";
	OFileDefine+=HeaderFile.File();
	OFileDefine+="_H__";
	OFileDefine.Upper();

	GString		HeaderFileName(HeaderFile.FullName());
	HeaderFileName.Lower();
	
	HOut.open(HeaderFileName,ios::out);

	if (HOut)
		{
		HOut<<"#ifndef "<<(char const *)OFileDefine<<endl;
		HOut<<"#define "<<(char const *)OFileDefine<<endl;

		HOut<<"/* Palette defines */"<<endl;

		for (f=0;f<AllSprPals.size();f++)
			HOut<<"#define PAL_"<<ReplaceBadFileChars(AllSprPals[f].GetName())<<" "<<AllSprPals[f].GetClut()<<endl;

		HOut<<"/* Frame defines */"<<endl;

		for (f=0;f<AllSprFrames.size();f++)
			{
			HOut<<"#define FRM_"<<ReplaceBadFileChars(AllSprFrames[f].GetName())<<" "<<f<<endl;
			}

		HOut<<"#endif "<<endl;

		HOut.close();
		}
	else
		Error(ERR_FATAL,"Error writing out file %s",(char const *)HeaderFileName);

}

/*----------------------------------------------------------------------
	Function:	
	Purpose:	Allocate all processed frames into tpages
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void SprSet::AddFramesAndPalsToVRAM(VRAM & Vr,int TpNumber,int WidthInTpages,int HeightInPixels)
{
	int		f;

	/* Add Palettes first */

	std::vector<SprPal>	CopySprPals;		/* Make a copy of the palettes */

	CopySprPals=AllSprPals;

	std::sort(CopySprPals.begin(),CopySprPals.end());	/* Sort em according to how many cols */

	for (f=0;f<AllSprPals.size();f++)
		{
		TPRect		MyPalRect;
		
		SprPal & OriginalPal=AllSprPals[CopySprPals[f].GetPalIndex()];


		MyPalRect.Set(TP_PAL,0,0,OriginalPal.GetNumOfCols(),1);

		if (Vr.AllocVRAM(MyPalRect))
			OriginalPal.SetVRAMPos(MyPalRect);
		else
			Error(ERR_FATAL,"Couldn't fit frames palettes into VRAM");
		}
	
	
	/* Add Frames */

	std::vector<TPRect>	AllocRects;

	AllocRects.resize(AllSprFrames.size());

	std::vector<TPageType>	oldTypes;

	oldTypes.resize(AllSprFrames.size());

	for (f=0;f<AllSprFrames.size();f++)
		{
		AllocRects[f].InitFromFrame(AllSprFrames[f]);
		AllocRects[f].SetDontRotate(!AllSprFrames[f].GetAllowRotate());
		AllocRects[f].SetId(f);

		if (AllSprFrames[f].GetFileInfo()->getAllocateAs16Bit())
			oldTypes[f]=AllocRects[f].convertTo16Bit();
		}

	/* Allocate int VRAM */

	if(Vr.AllocVRAM(AllocRects,false,true,true))
		{
		/*
			Now go back through all the sprite frames and tell them where
			they are going to sit in VRAM
		*/

		for (f=0;f<AllocRects.size();f++)
			{
			

			if (AllSprFrames[AllocRects[f].GetId()].GetFileInfo()->getAllocateAs16Bit())
				AllocRects[f].convertFrom16Bit(oldTypes[AllocRects[f].GetId()]);

			AllSprFrames[AllocRects[f].GetId()].SetVRAMPos(AllocRects[f]);
			}
		}
	else
		{
		Error(ERR_FATAL,"Couldn't fit frames into VRAM");
		}

	/* Go through and tell all pals and frames what the base tpage is */

	for (f=0;f<AllSprFrames.size();f++)
		AllSprFrames[f].SetTpBase(TpNumber);

	for (f=0;f<AllSprPals.size();f++)
		AllSprPals[f].SetTpBase(TpNumber);

	/* Now go through all the frames and tell them what Clut values they need */

	for (f=0;f<AllSprFrames.size();f++)
		AllSprFrames[f].SetClut(AllSprPals[AllSprFrames[f].GetPalIndex()].GetClut());
}
/*----------------------------------------------------------------------
  ---------------------------------------------------------------------- */
void SprSet::writeRawTPage(char const * file)
{
	ofstream		out;

	out.open(file,ios::binary|ios::out|ios::trunc);

	if (out)
		{
		
		if (Vi)
			Vi->WriteInTpageChunks(out);
		}
	else
		Error(ERR_FATAL,"Error opening raw file %s",file);
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:	Write out sprs and pals to a sprite file (not a VRAM block)
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void SprSet::WriteSprFile(char const * Name)
{
	std::vector<int>	SprOffsets;
	std::vector<int>	PalOffsets;

	ProcessPals();

	Gofstream	SpriteOut(Gofstream::LITTLE_ENDIAN);

	GString		SpriteOutName(Name);
	SpriteOutName.Lower();
	SpriteOut.open(SpriteOutName,ios::binary|ios::out|ios::trunc);

	if (SpriteOut)
		{
		int	NumOfSprs;
		int	NumOfPals;
		int PalOffsetTable;
		int SprOffsetTable;

		/*
		struct SPR_RAM_HDR
			{
			int	NumOfPals;
			int NumOfSprs;
			int PalOffsets[NumOfPals];
			int SprOfffsets[NumOfSprs];
			RAM_FRAME_HDR	Frames[NumOfSprs];
			Pals	Pals[NumOfPals];
			Data	[NumOfSprs]
			}
	
		*/

		NumOfSprs=AllSprFrames.size();
		NumOfPals=AllSprPals.size();

		SpriteOut.Put32(NumOfPals);
		SpriteOut.Put32(NumOfSprs);

		SprOffsets.resize(NumOfSprs);
		PalOffsets.resize(NumOfPals);

		PalOffsetTable=SpriteOut.tellp();
		SpriteOut.seekp(sizeof(u32)*NumOfPals,ios::cur);

		SprOffsetTable=SpriteOut.tellp();
		SpriteOut.seekp(sizeof(u32)*NumOfSprs,ios::cur);

		int	f;

		for (f=0;f<NumOfSprs;f++)
			AllSprFrames[f].WriteHeaderNotInVram(SpriteOut);

		for (f=0;f<NumOfPals;f++)
			{
			PalOffsets[f]=SpriteOut.tellp();
			AllSprPals[f].Write(SpriteOut);
			}

		for (f=0;f<NumOfSprs;f++)
			{
			SprOffsets[f]=SpriteOut.tellp();
			AllSprFrames[f].Write(SpriteOut);
			}

		SpriteOut.seekp(PalOffsetTable,ios::beg);

		for (f=0;f<NumOfPals;f++)
			SpriteOut.Put32(PalOffsets[f]);

		SpriteOut.seekp(SprOffsetTable,ios::beg);

		for (f=0;f<NumOfSprs;f++)
			SpriteOut.Put32(SprOffsets[f]);

		SpriteOut.close();
		}
	else
		Error(ERR_FATAL,"Error opeing sprite file %s",(char const *)SpriteOutName);

	WriteHeaderFile(GetHeaderFileName(Name));
}


/*----------------------------------------------------------------------
	Function:	
	Purpose:	Add an ANM file process it according to the file info
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void SprFrame::SetFrameAndInfo(Frame const & Fr,FileInfo const & NewMyFileInfo,int MaxSize)
{
	Frame::CopyFrame(Fr);
	MyFileInfo=NewMyFileInfo;


	GFName	MyName(MyFileInfo.GetFileName());
	GString	TempName=MyName.File();
	TempName.Upper();

	Name=TempName;
	
	/* Now Process the frame as needed */

	int		XOff;
	int		YOff;

	
	XOff=0;
	YOff=0;


	if (MyFileInfo.GetCrossHair() &&  !NewMyFileInfo.getAllocateAs16Bit())
		{
		CROSS_RES	CrossRes;

		if (CrossRes.FindCrossHair(*this,255))
			{
			XOff=-CrossRes.x;
			YOff=-CrossRes.y;
			ReplaceColour(255,0);
			}
		}

	/* Crop sprite down to only the pixels we want */

	if (MyFileInfo.GetShrinkToFit() &&  !NewMyFileInfo.getAllocateAs16Bit())
		{
		Rect	Bound;
		Bound=FindBoundingRect();
		
		if (Bound)
			{
			}
		else
			{
			Bound.X=0;
			Bound.Y=0;
			Bound.W=1;
			Bound.H=1;
			}

		Crop(Bound);

		XOff+=Bound.X;
		YOff+=Bound.Y;
		}

	if (MyFileInfo.GetForceOffsets() &&  !NewMyFileInfo.getAllocateAs16Bit())
		{
		XOff=-(MyFileInfo.GetXOff()-XOff);
		YOff=-(MyFileInfo.GetYOff()-YOff);
		}

	if (MaxSize &&  !NewMyFileInfo.getAllocateAs16Bit())
		{
		int		NewWidth;
		int		NewHeight;

		NewWidth=GetWidth();
		NewHeight=GetHeight();

		if (NewWidth > MaxSize)
			NewWidth=MaxSize;

		if (NewHeight > MaxSize)
			NewHeight=MaxSize;

		if (NewWidth != GetWidth() || NewHeight != GetHeight())
			{
			cout<<"Reducing frame "<<GetFileInfo()->GetActualFileName()<<endl;
			ResizeAndReduce(*this,GetNumOfCols(),float(NewWidth)/float(GetWidth()),float(NewHeight)/float(GetHeight()),MyFileInfo.GetZeroColZero());

#if 0
			char	Name[1024];
			static int Val;
			sprintf(Name,"c:\\data\\%d.lbm",Val);
			Val++;
			SaveLbm(Name);
#endif
			}
		}

	if (NewMyFileInfo.getAllocateAs16Bit())
	{
		int NewWidth = GU_AlignVal( GetWidth(), 4);
		int NewHeight = GU_AlignVal( GetHeight(), 4);
		if (NewWidth != GetWidth() || NewHeight != GetHeight())
			{
			cout<<"Reducing frame "<<GetFileInfo()->GetActualFileName()<<endl;
			ResizeAndReduce(*this,GetNumOfCols(),float(NewWidth)/float(GetWidth()),float(NewHeight)/float(GetHeight()),MyFileInfo.GetZeroColZero());

#if 1
			char	Name[1024];
			static int Val;
			sprintf(Name,"c:\\data\\%d.lbm",Val);
			Val++;
			SaveLbm(Name);
#endif
			}
	}

	X=XOff;
	Y=YOff;

	if (Width > 254 || Height > 254)
		{
		Error(ERR_FATAL,"Images only allowed to 254x254, this is %dx%d : %s",Width,Height,MyFileInfo.GetActualFileName());
		}


}

struct fRGBA
	{
	void Set(float _R,float _G,float _B,float _A)
		{R=_R;G=_G;B=_B;A=_A;}

	float	R,G,B,A;
	};

void SprFrame::ResizeAndReduce(Frame & Frm,int TargCols,float XPerc,float YPerc,bool ZeroSeeThrough)
{
	std::vector<fRGBA>	FullColNewScreen;
	std::vector<u8>		DestBytePic;
	std::vector<u8>		Bitmap;

	u8			Pal[256*3];
	u8 *		Dest;
	int			f;

	Bitmap.resize(Frm.GetWidth()*Frm.GetHeight()*4);

	Frm.MakeRGBA(&Bitmap[0],ZeroSeeThrough);

	int		Width,Height;

	Width=Frm.GetWidth();
	Height=Frm.GetHeight();

	int NewWidth=float(Width)*XPerc;
	int NewHeight=float(Height)*YPerc;
	
	if (!(Dest=new u8[NewWidth*NewHeight]))
		GObject::Error(ERM_OUTOFMEM);

	FullColNewScreen.resize(NewWidth*NewHeight);
	DestBytePic.resize(NewWidth*NewHeight*3);

	/* Now Super Sample it down */

	const int Samples=8;

	int		DestW=NewWidth;
	int		DestH=NewHeight;
	float	YMul=float(Height)/DestH;
	float	XMul=float(Width)/DestW;

	for (int dy=0;dy<DestH;dy++)
		{
		float	YPos=YMul*float(dy);

		for (int dx=0;dx<DestW;dx++)
			{
			float	XPos=XMul*float(dx);

			float	R=0;
			float	G=0;
			float	B=0;
			float	A=0;

			for (int Ys=0;Ys<Samples;Ys++)
				for (int Xs=0;Xs<Samples;Xs++)
					{
					float	xp=floor((XPos)+(Xs*(XMul/Samples)));
					float	yp=floor((YPos)+(Ys*(YMul/Samples)));
					int		Index=(yp*Width+xp);

					R+=float(Bitmap[Index*4+0])/255.0f;
					G+=float(Bitmap[Index*4+1])/255.0f;
					B+=float(Bitmap[Index*4+2])/255.0f;
					A+=float(Bitmap[Index*4+3])/255.0f;
					}

			R/=float(Samples*Samples);
			G/=float(Samples*Samples);
			B/=float(Samples*Samples);
			A/=float(Samples*Samples);

			if (R>1.0f)
				R=1.0f;

			if (G>1.0f)
				B=1.0f;
			
			if (B>1.0f)
				B=1.0f;

			int DestIndex=(dy*NewWidth+dx);

			FullColNewScreen[DestIndex].Set(R,G,B,A);
			}
		}

	/* Convert Back to RGBA into a dest byte picture */

	const	float	Bound=0.5f;

	for (f=0;f<NewWidth*NewHeight;f++)
		{
		float R=FullColNewScreen[f].R;
		float G=FullColNewScreen[f].G;
		float B=FullColNewScreen[f].B;

		if (FullColNewScreen[f].A > Bound)
			{
			DestBytePic[f*3+0]=R*255;
			DestBytePic[f*3+1]=G*255;
			DestBytePic[f*3+2]=B*255;
			}
		else
			{
			DestBytePic[f*3+0]=0;
			DestBytePic[f*3+1]=0;
			DestBytePic[f*3+2]=0;
			}
		}

	/* Now reduce colours */
	Palette NewPal;

	if (ZeroSeeThrough)
		{
		int	Cols=TargCols-1;

		tquant(&DestBytePic[0],Dest,Pal,Cols,NewWidth*NewHeight);

		for (f=0;f<NewWidth*NewHeight;f++)
			{
			if (FullColNewScreen[f].A > Bound)
				Dest[f]=Dest[f]+1;
			else
				Dest[f]=0;
			}

		for (f=1;f<Cols+1;f++)
			NewPal[f].SetRGB(Pal[(f-1)*3+0],Pal[(f-1)*3+1],Pal[(f-1)*3+2]);

		NewPal[0].SetRGB(255,0,255);
		}
	else
		{
		int	Cols=TargCols;
		
		tquant(&DestBytePic[0],Dest,Pal,Cols,NewWidth*NewHeight);

		for (f=0;f<Cols;f++)
			NewPal[f].SetRGB(Pal[f*3+0],Pal[f*3+1],Pal[f*3+2]);
		}

	Frm.SetFrame(&Dest[0],NewWidth,NewHeight,NewPal);
}


/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
SprFrame::SprFrame(void)
{
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void SprFrame::SetVRAMPos(TPRect const & NewRect)
{
	MyRect=NewRect;
}


/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void SprFrame::WriteHeader(Gofstream & Out)
{
	/*
	struct VRAM_FRAME_HDR
	{
	u16			tpage;
	u16			clut;
	u8			x,y,w,h;
	u8			u,v;
	u8			Rotated;
	};
	*/

	Out.Align(sizeof(u16));
	
	u16	OutTpage=GetTpage();
	u16	OutClut=GetClut();

	Out.Put16(GetTpage());
	Out.Put16(OutClut);
	Out.put(s8(X));
	Out.put(s8(Y));

	if (MyFileInfo.GetMoveUVs() && Width && !MyFileInfo.getAllocateAs16Bit())
		Out.put(s8(Width-1));
	else
		Out.put(s8(Width));

	if (MyFileInfo.GetMoveUVs() && Height && !MyFileInfo.getAllocateAs16Bit())
		Out.put(s8(Height-1));
	else
		Out.put(s8(Height));

	
	BDEPTH	RetDepth;
	RetDepth=GetBitDepth();

	Out.put(getU());
	Out.put(u8(MyRect.Y));

	IsRotated() ? Out.put(u8(1)) : Out.put(u8(0));

	MyFileInfo.getAllocateAs16Bit() ? Out.put(u8(1)) : Out.put(u8(0));
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void SprFrame::WriteHeaderNotInVram(Gofstream & Out)
{
	/*
	struct RAM_FRAME_HDR
		{
		u32			PalNum;
		u8			x,y,w,h;
		};
	*/

	Out.Align(sizeof(u32));

	Out.Put32(PalIndex);
	Out.put(s8(X));
	Out.put(s8(Y));

	if (MyFileInfo.GetMoveUVs() && Width)
		Out.put(s8(Width-1));
	else
		Out.put(s8(Width));

	if (MyFileInfo.GetMoveUVs() && Height)
		Out.put(s8(Height-1));
	else
		Out.put(s8(Height));

	Out.Align(sizeof(u32));
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void SprFrame::Write(Gofstream & Out) const
{
	/* Note, this is shite, it only works on 16 col images. Doh! Gaz */

	const int	AlignPixels=4;
	std::vector<u8>	Body;

	Frame	NewFrame;

	NewFrame=*this;
	
	Rect	OriginalRect;

	OriginalRect=NewFrame;

	OriginalRect.X=0;
	OriginalRect.Y=0;
	OriginalRect.W=GU_AlignVal(OriginalRect.W,AlignPixels);

	NewFrame.Crop(OriginalRect);

	int	nfW,nfH,nfLineWidthBytes,nfAreaBytes;
	
	nfW=NewFrame.GetWidth();
	nfH=NewFrame.GetHeight();
	nfLineWidthBytes=GU_AlignVal(nfW,2)/2;
	nfAreaBytes=nfLineWidthBytes*nfH;

	Body.resize(nfAreaBytes);

	for (int y=0;y<nfH;y++)
		for (int x=0;x<nfW;x++)
			{
			u8 *	PixAddr;
			u8		ScrNib;

			ScrNib=NewFrame.SeeData()[y*nfW+x]&0xf;

			PixAddr=&Body[(x/2)+(nfLineWidthBytes*y)];
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
	
	Out.write((char *)(&Body[0]),nfAreaBytes);
	Out.Align(sizeof(u32));
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void SprPal::Write(Gofstream & Out) const
{
	std::vector<u16> OutPal;
	MakePSXPal(OutPal);

	Out.Put32(OutPal.size());
	
	for (int f=0;f<OutPal.size();f++)
		Out.Put16(OutPal[f]);
	
	Out.Align(sizeof(u32));
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
u16 SprPal::GetPsxCol(Colour const & Col) const
{
	int		R,G,B;
	R=Col.GetR()>>3;
	G=Col.GetG()>>3;
	B=Col.GetB()>>3;
	return((R&0x1f)<<0)|((G&0x1f)<<5)|((B&0x1f)<<10);
}
/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void SprPal::MakePSXPal(std::vector<u16> & OutWords) const
{

	int		NumOfCols;
	int		f;
	
	NumOfCols=(*this).GetNumOfCols();
	OutWords.resize(NumOfCols);

	if (GetZeroColZero() || GetPlusColZero())
		{
		OutWords[0]=0;

		for (f=1;f<NumOfCols;f++)
			{
			u16	Col=GetPsxCol((*this)[f]);
			Col += Col ? 0 : 1;
			OutWords[f]=Col|0x8000;
			}
		}
	else
		{
		for (f=0;f<NumOfCols;f++)
			{
			u16	Col=GetPsxCol((*this)[f]);
			OutWords[f]=Col|0x8000;
			}
		}
}

/*===========================================================================
 end */
