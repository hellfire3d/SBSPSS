/*=========================================================================

	SPRSET.CPP	

	Author:		Gary Liddon @ Farehame
	Created:
	Project:	TPW Parkgrab
	Purpose:	Object that reads in all the frames for sprites
				processes them into tpages and spits it all out to
				disk
							
	Copyright (c) 1998 G R Liddon

===========================================================================*/

#ifndef __SPRSET_H__
#define __SPRSET_H__

/*----------------------------------------------------------------------
	Includes
	-------- */

/*	Std Lib
	------- */
#include <vector>

/*	Glib
	---- */
#include <gobject.hpp>
#include <gstring.hpp>
#include <frame.hpp>
#include <misc.hpp>

/*	Local
	----- */
#include "tpage.h"
#include "vimage.h"


/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */
struct	sTexOutInfo
{
	GString		Name;
	u16			Clut;
	u16			Tpage;
	bool		Rotated;
	int			u,v,w,h;
	int			XOfs,YOfs;
	int			OrigW,OrigH;
};

/*	Encapsulates a file and all the infomation parkgrab needs to process it
	----------------------------------------------------------------------- */
class FileInfo
	{
	public:
		FileInfo(void)
			{
			CrossHair=false;
			ForceOffsets=false;
			MemFrame=NULL;
			}

		FileInfo(FileInfo const & Fi)	
			{
			operator=(Fi);
			}

		void operator=(FileInfo const & Fi)	
			{
			FileName = Fi.FileName;
			strcpy(ActualFileName,Fi.ActualFileName);
			CrossHair=Fi.CrossHair;
			ZeroColZero = Fi.ZeroColZero;
			PlusColZero = Fi.PlusColZero;
			MoveUVs = Fi.MoveUVs;
			AllowRotate=Fi.AllowRotate;
			ForceOffsets=Fi.ForceOffsets;
			XOff=Fi.XOff;
			YOff=Fi.YOff;
			XOff1=Fi.XOff1;
			YOff1=Fi.YOff1;
			ShrinkToFit=Fi.ShrinkToFit;
			m_allocateAs16Bit=Fi.m_allocateAs16Bit;
			MemFrame=Fi.MemFrame;
			}

		void SetForceOffsets(int x,int y)
			{
			ForceOffsets=true;
			XOff=x;
			YOff=y;
			}

		GString  CheckForPlus(GString s)
		{
			GString out;
			const char *textin = (const char*)s;
			char *textout = new char[s.Len()+1];
			memset(textout, 0, s.Len()+1);
			int p;
			bool	plus;

			p=0;
			if (textin[0] == '+')	plus = true;
			else					plus = false;
/*			for (i=0; i<s.Len(); i++)
			{
				textout[i]=textin[i];
				if (textout[i] == '+')	plus = true;
//				p++;
//				else					p++;
			}
*/			PlusColZero = plus;

			out = textin;
			delete textout;
			return out;
		}

		void SetInfo(const char * NewFileName,bool NewCrossHair,bool NewZeroColZero, bool NewMoveUVs,bool NewAllowRotate,bool NewShrinkToFit,bool allocateAs16Bit,Frame *NewMemFrame=NULL)
			{
			CrossHair=NewCrossHair;
			ZeroColZero=NewZeroColZero;
			MoveUVs  =NewMoveUVs;
			AllowRotate=NewAllowRotate;
			SetName(NewFileName);
			ShrinkToFit=NewShrinkToFit;
			
			m_allocateAs16Bit=allocateAs16Bit;
			if (NewMemFrame)
			{
				MemFrame=new Frame;
				*MemFrame=*NewMemFrame;
			}
			/*
			if we're allocating on a 16 bit pixel boundary then
			we don't want the texture to be rotated
			*/

			if (allocateAs16Bit)
				AllowRotate=false;
			}

		const char * GetFileName(void) const
			{return(FileName);}

		const char * GetActualFileName(void) const
			{return(ActualFileName);}

		void SetName(const char * NewName)
		{
			if (NewName)	strcpy(ActualFileName,NewName);
			FileName = CheckForPlus(ActualFileName);
		}

		bool	GetCrossHair(void) const
			{return(CrossHair);}

		bool	GetZeroColZero(void) const
			{return(ZeroColZero);}

		bool	GetPlusColZero(void) const
			{return(PlusColZero);}

		bool	GetMoveUVs(void) const
			{return(MoveUVs);}

		bool	GetAllowRotate(void) const
			{return(AllowRotate);}

		bool	GetForceOffsets(void) const
			{
			return(ForceOffsets);
			}
		
		int		GetXOff(void) const
			{
			return(XOff);
			}

		int		GetYOff(void) const
			{
			return(YOff);
			}
		int		GetXOff1(void) const
			{
			return(XOff1);
			}

		int		GetYOff1(void) const
			{
			return(YOff1);
			}

		bool getAllocateAs16Bit(void) const
			{return(m_allocateAs16Bit);}

		bool getHasMemFrame(void) const
			{return(MemFrame!=NULL);}
//			{return(MemFrame.SeeData()!=NULL);}

		Frame const &getMemFrame() const 
			{return(*MemFrame);}

	char const * GetActualName(void) const
		{return(ActualFileName);}

	bool GetShrinkToFit(void) const
		{return(ShrinkToFit);}

	protected:
		GString	FileName;
		char	ActualFileName[256];
		bool	CrossHair;
		bool	ZeroColZero;
		bool	PlusColZero;
		bool	MoveUVs;
		bool	AllowRotate;
		bool	ForceOffsets;
		bool	ShrinkToFit;
		
		bool	m_allocateAs16Bit;

		int		XOff,YOff;
		int		XOff1,YOff1;

		Frame	*MemFrame;
	};

typedef std::vector<FileInfo> FIVec;
typedef std::vector<FileInfo>::iterator FIVecIt;
typedef std::vector<FileInfo>::const_iterator FIVecConstIt;


/*  A Palette of graphics with sprset specific stuff
	---------------------------------------------- */
class SprPal : public Palette
{
public:
	SprPal(void)
		{}
	
	SprPal(SprPal const & Pal)
		{MakeCopy(Pal);}
	
	void operator=(SprPal const & Pal)
		{MakeCopy(Pal);}

	void operator=(Palette const & Pal)
		{Palette::CopyPal(Pal);}

	bool operator==(Palette const & Pal)
		{return(Palette::operator==(Pal));}

	bool operator<(SprPal const & Pal)
		{return(GetNumOfCols() < Pal.GetNumOfCols());}

	void SetPalIndex(int NewPalIndex)
		{PalIndex=NewPalIndex;}

	int GetPalIndex(void) const
		{return(PalIndex);}

	void SetVRAMPos(TPRect & NewRect)
		{
		MyRect=NewRect;
		}

	void SetTpBase(int NewTpBase)
		{
		TpBase=NewTpBase;
		BaseTpX=((TpBase&0xf)*64);
		BaseTpY=(TpBase>>4)*256;
		}

	u16 GetClut(void)
		{
		int	x=(MyRect.X/4)+BaseTpX;
		int	y=MyRect.Y+BaseTpY;
		return((y<<6)|((x>>4)&0x3f));
		}

	TPRect const & GetTPRect(void) const
		{return(MyRect);}

	char const * GetName(void) const
		{return(Name);}

	void SetName(char const * NewName)
		{Name=NewName;}

	void	SetZeroColZero(bool New)
		{ZeroColZero=New;}

	bool	GetZeroColZero(void) const
		{return(ZeroColZero);}

	void	SetPlusColZero(bool New)
		{PlusColZero=New;}

	bool	GetPlusColZero(void) const
		{return(PlusColZero);}

	void		Write(Gofstream & Out) const;
	void		MakePSXPal(std::vector<u16> & OutWord) const;
	u16	GetPsxCol(Colour const & Col) const;

protected:
	int		PalIndex;
	int		TpBase;
	int		BaseTpX;
	int		BaseTpY;
	bool	ZeroColZero;
	bool	PlusColZero;
	TPRect	MyRect;
	GString	Name;

	void MakeCopy(SprPal const & NewPal)
		{
		Palette::CopyPal(NewPal);
		PalIndex=NewPal.PalIndex;
		TpBase=NewPal.TpBase;
		BaseTpX=NewPal.BaseTpX;
		BaseTpY=NewPal.BaseTpY;
		MyRect=NewPal.MyRect;
		Name=NewPal.Name;
		ZeroColZero=NewPal.ZeroColZero;
		PlusColZero=NewPal.PlusColZero;
		}
};

/*  A Frame of graphics with sprset specific stuff
	---------------------------------------------- */
class SprFrame : public Frame
{
public:
	enum BDEPTH
		{
		BITS_4,
		BITS_8,
		};

	SprFrame(void);
	SprFrame(SprFrame const & NewFrame)
		{MakeCopy(NewFrame);}

	void operator=(SprFrame const & NewFrame)
		{MakeCopy(NewFrame);}

	void SetFrameAndInfo(Frame const & Fr,FileInfo const & MyFileInfo,int MaxSize);
	FileInfo * GetFileInfo(void)
	{
		return (&MyFileInfo);
	}

	void Process(void);
	
	void SetPalIndex(int NewPalIndex)
		{PalIndex=NewPalIndex;}

	int	GetPalIndex(void) const
		{return(PalIndex);}

	void SetVRAMPos(TPRect const & Rect);

	BDEPTH	GetBitDepth(void) const
		{
		BDEPTH	RetDepth;
		int		NumOfCols;

		NumOfCols=MyPal.GetNumOfCols();

		if (NumOfCols<=16)
			RetDepth=BITS_4;
		else
			RetDepth=BITS_8;

		return(RetDepth);
		}

	TPRect const & GetTPRect(void) const
		{return(MyRect);}

	bool IsRotated(void) const
		{return(MyRect.GetRotate());}

	void SetTpBase(int NewTpBase)
		{
		TpBase=NewTpBase;
		BaseTpX=((TpBase&0xf)*64);
		BaseTpY=(TpBase>>4)*256;
		}

	void SetClut(u16 NewClut)
		{Clut=NewClut;}

	u16 GetClut(void) const
		{return(Clut);}

	void WriteHeader(Gofstream & Out);

	bool	GetZeroColZero(void) const
		{return(MyFileInfo.GetZeroColZero());}

	bool	GetPlusColZero(void) const
		{return(MyFileInfo.GetPlusColZero());}

	bool	GetAllowRotate(void) const
		{return(MyFileInfo.GetAllowRotate());}


	void	Write(Gofstream & Out) const;

	void	WriteHeaderNotInVram(Gofstream & Out);


	int		getV(void)
		{return(MyRect.X&0xff);}

	int		getU(void)
		{
		u16	tpage;
		int	u;
		int	pageX;

		tpage=GetTpage();
		pageX=(tpage&0xf)*256;
		u=MyRect.X-pageX;

		switch(GetBitDepth())
			{
			case BITS_8:
				u/=2;
				break;
			}

		return(u);
		}

	u16 GetTpage(void)
		{
		int	tp;
		BDEPTH	RetDepth;

		RetDepth=GetBitDepth();

		int	abr=0;
		int x=BaseTpX + (MyRect.X/4);  //MA
		int y=BaseTpY + MyRect.Y;  //MA

		switch(RetDepth)
			{
			case BITS_4:
				tp=0;
				break;
			case BITS_8:
				tp=1;
				break;
			}

		return((((tp)&0x3)<<7)|(((abr)&0x3)<<5)|(((y)&0x100)>>4)|(((x)&0x3ff)>>6)| (((y)&0x200)<<2));
		}
	
	bool	IsAnimated()	
		{
		return(MyFileInfo.getAllocateAs16Bit());
		}
	int		getX(void)	{return(X);}

	int		getY(void)	{return(Y);}

	int		GetOrigW()	{return(OrigW);}
	int		GetOrigH()	{return(OrigH);}

	int		getW(void)
	{
		if (MyFileInfo.GetMoveUVs() && Width && !MyFileInfo.getAllocateAs16Bit())
			return(Width-1);
		else
			return(Width);
	}
	
	int		getH(void)
	{
		if (MyFileInfo.GetMoveUVs() && Height && !MyFileInfo.getAllocateAs16Bit())
			return(Height-1);
		else
			return(Height);
	}

protected:
	void	ResizeAndReduce(Frame & Frm,int TargCols,float XPerc,float YPerc,bool ZeroSeeThrough);


	void MakeCopy(SprFrame const & NewFrame)
		{
		Frame::CopyFrame(NewFrame);

		Clut=NewFrame.Clut;
		TpBase=NewFrame.TpBase;
		BaseTpX=NewFrame.BaseTpX;
		BaseTpY=NewFrame.BaseTpY;
		PalIndex=NewFrame.PalIndex;
		MyFileInfo=NewFrame.MyFileInfo;
		MyRect=NewFrame.MyRect;
		OrigW=NewFrame.OrigW;
		OrigH=NewFrame.OrigH;
		}


	u16			Clut;
	int			TpBase;
	int			BaseTpX;
	int			BaseTpY;

	GString		loadFileName;

	int			PalIndex;
	FileInfo	MyFileInfo;
	TPRect		MyRect;
	int			OrigW,OrigH;
};

/*	A collection of sprites
	----------------------- */
class SprSet : protected GObject
{
public:
	SprSet(void)
		{
		Vi=NULL;
		MaxSize=0;
		DebugFlag=false;
		}

	void	SetMaxSize(int New)
		{MaxSize=New;}

	void	AddFiles(FIVec const & FileList);

	void	CreateTexInfo(std::vector<sTexOutInfo> &TexInfo);

	void	Write(char const * FileName,int TpNumber,int WidthInTpages,int HeightInTpages);
	void	WriteLBM(char const * FileName);
	void	WriteSprFile(char const * Name);
	void	WriteReport(char const * Name);
	void	writeRawTPage(char const * File);
	void	SetIncOutFile(char const * NewIncOutFile)
		{IncOutFile=NewIncOutFile;IncOutFile.Lower();}

	void	setAnimatedHeader(bool newVal)
		{m_AnimatedHeadersOnly=newVal;}

	void	setDontOutputBoxes(bool newVal)
		{m_DontOutputBoxes=newVal;}

	void	setAlignHeaders(bool newVal)
		{m_AlignHeaders=newVal;}

	void	setHalfTpage(bool newVal)
		{m_halfTpage=newVal;}

	void	setCompressTpages(bool newVal)
		{m_compressTpages=newVal;}

	void	setNoWriteTpages(bool newVal)
		{m_noWriteTpages=newVal;}
	void	setDebug(bool f)
		{DebugFlag=f;}

protected:
	void	WriteHeaderFile(char const * HName);

	void	ProcessPals(void);
	void	AddFile(FileInfo const & ThisInfo);
	void	AddAnm(FileInfo const & ThisInfo);
	void	AddLbm(FileInfo const & ThisInfo);
	void	AddFrame(Frame const & Fr,FileInfo const & ThisInfo);

	void	AddFramesAndPalsToVRAM(VRAM & Vr,int TpNumber,int WidthInTpages,int HeightInPixels);

	GString		GetHeaderFileName(char const * File);


	typedef std::vector<SprFrame> SprFrVec;
	typedef std::vector<SprFrame>::iterator SprFrIt;

	typedef std::vector<SprPal> SprPalVec;
	typedef std::vector<SprPal>::iterator SprPalVecIt;

	GString	IncOutFile;

	bool	m_AnimatedHeadersOnly;
	bool	m_DontOutputBoxes;
	bool	m_AlignHeaders;
	bool	m_halfTpage;
	bool	m_compressTpages;
	bool	m_noWriteTpages;
	bool	DebugFlag;

	SprFrVec	AllSprFrames;
	SprPalVec	AllSprPals;
	VRAMImage *	Vi;

	int			MaxSize;
};

/*----------------------------------------------------------------------
	Globals
	------- */

/*	Vars
	---- */

/*	Data
	---- */

/*	Functions
	--------- */

/*---------------------------------------------------------------------- */

#else	/* __SPRSET_H__ */

class SprPal;
class SprFrame;

#endif

/*===========================================================================
 end */
