/*=========================================================================

	FRAME.CPP

	Author:  Gary Liddon @ Fareham
	Created:
	Project:
	Purpose:

	Copyright (c) 1997 G R Liddon

===========================================================================*/

/*----------------------------------------------------------------------
	Includes
	-------- */

/*	Std Lib
	------- */
#include <conio.h>
#include <math.h>

/*	STL
	--- */
//#include <alogrithm>
#include <ALGORITHM>


/*	Glib
	---- */

/*	Local
	----- */
#include "frame.hpp"
#include "ilbm.hpp"
#include "gutils.h"
#include "misc.hpp"

using namespace std;

static void load_bmp(Frame & frm,char const *filename);


/*----------------------------------------------------------------------
	Function:		Frame::Frame(void)
  ---------------------------------------------------------------------- */
Frame::Frame(void)
{
	InitFrame();
}

/*----------------------------------------------------------------------
	Function:		Copy Constructor
  ---------------------------------------------------------------------- */
Frame::Frame(Frame const & Fr)
{
	InitFrame();
	CopyFrame(Fr);
}

/*----------------------------------------------------------------------
	Function:		Destructor
  ---------------------------------------------------------------------- */
Frame::~Frame(void)
{
	if (Buffa)
		delete Buffa;
}

/*----------------------------------------------------------------------
	Function:		Init the Frame
  ---------------------------------------------------------------------- */
void Frame::InitFrame(void)
{
	Buffa=NULL;
	X=0;
	Y=0;
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void Frame::CopyFrame(Frame const & Fr)
{
	DumpStuff();

	Width=Fr.Width;
	Height=Fr.Height;

	if (Fr.Buffa)
		{
		if (!(Buffa=new u8[Width*Height]))
			Error(ERM_OUTOFMEM);

		memcpy(Buffa,Fr.Buffa,Width*Height);
		}
	else
		Buffa=NULL;

	Name=Fr.Name;

	MyPal=Fr.MyPal;

	X=Fr.X;
	Y=Fr.Y;
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void Frame::DumpStuff(void)
{
	if (Buffa)
		delete Buffa;
	Width=0;
	Height=0;
	X=0;
	Y=0;

}

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
bool Frame::IsBlank(u8 BlankVal)
{
	if (Buffa)
		{
		for (int f=0;f<Width*Height;f++)
			{
			if (Buffa[f] != BlankVal)
				return(false);
			}
		}

	return(true);
}


/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
bool Frame::Grab(Frame const & Fr,Rect const & RetRect)
{
	Rect GrabRect(RetRect);
	Rect FromRect=Fr;

	FromRect.X=0;
	FromRect.Y=0;
	FromRect.ClipRect(GrabRect);

	if (GrabRect)
		{
		InitFrame();
		u8 const * GrAddr=&Fr.Buffa[GrabRect.Y*Fr.Width+GrabRect.X];

		if (Buffa=new u8[GrabRect.W*GrabRect.H])
			{
			Width=GrabRect.W;
			Height=GrabRect.H;
			MyPal=Fr.MyPal;

			for (int y=0;y<Height;y++)
				memcpy(&Buffa[y*Width],&GrAddr[y*Fr.Width],Width);
			}
		else
			Error(ERM_OUTOFMEM);

		return(true);
		}

	return(false);
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
bool Frame::DrawBox(Rect const & RetRect,u8 Col)
{
	Rect GrabRect(RetRect);
	Rect FromRect=*this;

	FromRect.X=0;
	FromRect.Y=0;
	FromRect.ClipRect(GrabRect);

	if (GrabRect)
		{
		u8 * GrAddr=&Buffa[GrabRect.Y*Width+GrabRect.X];

		for (int y=0;y<Height;y++)
			memset(&GrAddr[y*Width],Col,Width);

		return(true);
		}

	return(false);
}


/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void Frame::SetFrame(u8 const * nBuffa,int W,int H,Palette const & NewPal)
{
	DumpStuff();

	Width=W;
	Height=H;

	MyPal=NewPal;
	
	if (!(Buffa=new u8[Width*Height]))
		Error(ERM_OUTOFMEM);

	memcpy(Buffa,nBuffa,Width*Height);
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void Frame::ReduceSinglePixels(int Threshold)
{
	UINT		cury,curx,col,lastcol,nextcol;
	UINT 		r,g,b,lastr,lastg,lastb,nextr,nextg,nextb,dist1,dist2;
	UCHAR *	p;

	UINT	RDLBM256_sy=Height;
	UINT	RDLBM256_sx=Width;
	u8 *	inbuf=Buffa;

	for(cury=0;cury<RDLBM256_sy;cury++)
	{
		p = inbuf+(cury*RDLBM256_sx);
		for(curx=0;curx<RDLBM256_sx;curx++)
			{
			col = *p;
			nextcol = *(p+1);

			if(curx && curx!=RDLBM256_sx-1 && col && col != lastcol && col != nextcol)
				{
				Colour *	C;
				Colour *	LC;
				Colour *	NC;

				C=&MyPal[col];
				LC=&MyPal[lastcol];
				NC=&MyPal[nextcol];

				r = C->GetR();
				g = C->GetG();;
				b = C->GetB();;

				lastr = LC->GetR();
				lastg = LC->GetG();
				lastb = LC->GetB();

				nextr = NC->GetR();
				nextg = NC->GetG();
				nextb = NC->GetB();

				dist1 = ((lastr-r)*(lastr-r)) + ((lastg-g)*(lastg-g)) + ((lastb-b)*(lastb-b));
				dist2 = ((nextr-r)*(nextr-r)) + ((nextg-g)*(nextg-g)) + ((nextb-b)*(nextb-b));

				if(dist1 < dist2)
					{
					if(dist1 <= Threshold)
						{
						*p = lastcol;
						}
					}
				else
					{
					if(dist2 <= Threshold)
						*p = nextcol;
					}
				}
			lastcol = *p++;
		}
	}
}


/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void Frame::ReduceSize(void)
{
	Rect	MRect=FindBoundingRect();
	Crop(MRect);
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void Frame::Crop(Rect const & NewRect)
{
	if (Buffa && NewRect)
		{
		Rect	CopyRect;
		CopyRect=*this;
		CopyRect.X=0;
		CopyRect.Y=0;

		Rect MyRect=NewRect;

		CopyRect.ClipRect(MyRect);
		CopyRect=MyRect;

		if (CopyRect)
			{
			u8 *	DestBuffa;

			if (!(DestBuffa=new u8[NewRect.Area()]))
				Error(ERM_OUTOFMEM);

			memset(DestBuffa,0,NewRect.Area());

			u8 * Src=&Buffa[CopyRect.X+CopyRect.Y*Width];
			u8 * Dst=DestBuffa;

			for (int yy=0;yy<CopyRect.H;yy++)
				{
				memcpy(Dst,Src,CopyRect.W);
				Src+=Width;
				Dst+=NewRect.W;
				}

			delete Buffa;
			Buffa=DestBuffa;

			Width=NewRect.W;
			Height=NewRect.H;
			}
		}
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:	Resize this image to another size (uses point sampling)
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void Frame::Resize(int NewWidth,int NewHeight)
{
	if (NewHeight != Height || NewWidth != Width)
		{
		vector<u8>	NewPic;
		
		NewPic.resize(NewWidth*NewHeight);

		for (int y=0;y<NewHeight;y++)
			for (int x=0;x<NewWidth;x++)
				{
				float	XFrac=float(x)/float(NewWidth);
				float	YFrac=float(y)/float(NewHeight);
				int	FromX=float(Width)*XFrac;
				int	FromY=float(Height)*YFrac;
				NewPic[y*NewWidth+x]=Buffa[FromY*Width+FromX];
				}

		DumpStuff();

		Width=NewWidth;
		Height=NewHeight;

		if(!(Buffa=new u8[Width*Height]))
			Error(ERM_OUTOFMEM);

		memcpy(Buffa,&NewPic[0],Width*Height);
		}

}

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
Rect Frame::FindBoundingRect(void)
{
	Rect	RetRect;
	u8 *	Bmap;

	Bmap=Buffa;

	if (Bmap)
		{
		int		Bottom;
		int		x,y;

		Bottom=-1;

		for (y=Height-1;y>=0 && Bottom==-1;y--)
			{
			for (x=0;x<Width && Bottom==-1;x++)
				if (Bmap[y*Width+x])
					Bottom=y;
			}

		if (Bottom != -1)
			{
			int		Top=-1;
			for (y=0;y<=Bottom && Top==-1;y++)
				{
				for (x=0;x<Width && Top==-1;x++)
					if (Bmap[y*Width+x])
						Top=y;
				}

			if (Top != -1)
				{
				int	Left=-1;

				for (x=0;x<Width && Left==-1;x++)
					{
					for (y=Top;y<=Bottom && Left==-1;y++)
						if (Bmap[y*Width+x])
							Left=x;
					}

				if (Left!=-1)
					{
					int	Right=-1;

					for (x=Width-1;x>=Left && Right==-1;x--)
						{
						for (y=Top;y<=Bottom && Right==-1;y++)
							if (Bmap[y*Width+x])
								Right=x;
						}

					if (Right !=-1)
						{
						RetRect.X=Left;
						RetRect.Y=Top;
						RetRect.W=Right-Left+1;
						RetRect.H=Bottom-Top+1;
						}
					else
						Error(ERR_FATAL,"Strange frame");
					}
				else
					Error(ERR_FATAL,"Strange frame");
				}
			else
				Error(ERR_FATAL,"Strange frame");
			}
		}

	return(RetRect);
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void Frame::SaveLbm(char const * Lbm)
{
	u8 * Pal;

	Pal=MyPal.MakeDpPal();
	nilbm::SavePbm((char *)Lbm,Pal,Buffa,Width,Height);
	delete Pal;
}


/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void Frame::LoadLbm(char const * Lbm)
{
	nilbm	MyLbm(Lbm);

	if (!MyLbm.error())
		{
		if (Buffa)
			delete Buffa;

		u8 const *	CMap;

		Buffa=MyLbm.TakeBmap();
		CMap=MyLbm.SeeCmap();

		Width=MyLbm.GetWidth();
		Height=MyLbm.GetHeight();

		int		NumOfCols=1<<MyLbm.GetPlanes();

		for (int f=0;f<NumOfCols;f++)
			{
			MyPal[f].SetR(CMap[f*3+0]);
			MyPal[f].SetG(CMap[f*3+1]);
			MyPal[f].SetB(CMap[f*3+2]);
			}
		}
	else
		Error(ERR_FATAL,"Can't load lbm %s",Lbm);
}


/*----------------------------------------------------------------------
	Function:	
	Purpose:	Load a paletted BMP file
	Params:
	Returns:
  ---------------------------------------------------------------------- */

void Frame::LoadBMP(char const * FileName)
{
	load_bmp(*this,FileName);
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int Frame::GetNumOfCols(void) const
{
	int	Ret;

	Ret=0;

	if (Buffa)
		{
		for (int f=0;f<Width*Height;f++)
			{
			if (Buffa[f] > Ret)
				Ret=Buffa[f];
			}
		}

	return(Ret+1);
}



/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void Frame::FlipX(void)
{
	if (Buffa && Width && Height)
		{
		vector<u8>	Buffa2;

		Buffa2.resize(Width*Height);

		for (int y=0;y<Height;y++)
			{
			for (int x=0;x<Width;x++)
				Buffa2[((Width-1)-x)+y*Width]=Buffa[x+y*Width];
			}
		memcpy(Buffa,&Buffa2[0],Width*Height);
		}
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void Frame::FlipY(void)
{
	if (Buffa && Width && Height)
		{
		vector<u8>	Buffa2;

		Buffa2.resize(Width*Height);

		for (int y=0;y<Height;y++)
			memcpy(&Buffa2[((Height-1)-y)*Width],&Buffa[y*Width],Width);

		memcpy(Buffa,&Buffa2[0],Width*Height);
		}
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void Frame::Remap(Palette const & P)
{
	if (Buffa)
		{
		int *	RemapBuffer;

		RemapBuffer=P.MakeRemapTable(MyPal);

		for (int f=0;f<Width*Height;f++)
			Buffa[f]=RemapBuffer[Buffa[f]];

		MyPal=P;
		delete RemapBuffer;
		}
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void Frame::MakeRGBA(u8 * Dest,bool ZeroIsTrans)
{
	if (Buffa)
		{
		int		Area;
		Area=Width*Height;
		
		for (int f=0;f<Area;f++)
			{
			Dest[f*4+0]=MyPal[Buffa[f]].GetR();
			Dest[f*4+1]=MyPal[Buffa[f]].GetG();
			Dest[f*4+2]=MyPal[Buffa[f]].GetB();

			if (ZeroIsTrans)
				Dest[f*4+3]=Buffa[f] ? 255 : 0;
			else
				Dest[f*4+3]=255;
			}
		}
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void Frame::MakeRGB(u8 * Dest)
{
	if (Buffa)
		{
		int		Area;
		Area=Width*Height;
		
		for (int f=0;f<Area;f++)
			{
			Dest[f*3+0]=MyPal[Buffa[f]].GetR();
			Dest[f*3+1]=MyPal[Buffa[f]].GetG();
			Dest[f*3+2]=MyPal[Buffa[f]].GetB();
			}
		}
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void Frame::Plot(Frame & Dest,int X,int Y)
{
	if (Buffa && Dest.Buffa)
		{
		Rect	DestRect=Dest;
		Rect	NewR(X,Y,Width,Height);

		DestRect.X=0;
		DestRect.Y=0;

		DestRect.ClipRect(NewR);

		if (NewR)
			{
			int		NumOfCols=MyPal.GetNumOfCols();
			int *	RemapBuffer;

			int 	XIndent=NewR.X-X;
			int 	YIndent=NewR.Y-Y;

			u8 *	PicSrc=&Buffa[XIndent+YIndent*Width];

			RemapBuffer=Dest.MyPal.MakeRemapTable(MyPal);

			for (int yy=NewR.Y;yy<NewR.Y+NewR.H;yy++)
				{
				for (int xx=0;xx<NewR.W;xx++)
					Dest.Buffa[yy*Dest.Width+NewR.X+xx]=RemapBuffer[PicSrc[(yy-NewR.Y)*Width+xx]];
				}

			delete RemapBuffer;
			}
		}
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void Frame::PlotBox(Rect const & R,int Col)
{
	Rect	NewR(0,0,Width,Height);
	Rect	DRect;

	DRect=R;
	NewR.ClipRect(DRect);

	if (DRect && Buffa)
		{
		for (int yy=DRect.Y;yy<DRect.Y+DRect.H;yy++)
			memset(&Buffa[yy*Width+DRect.X],Col,DRect.W);

		}
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void Frame::PlotTrans(Frame & Dest,int X,int Y,int TrCol)
{
	if (Buffa && Dest.Buffa)
		{
		Rect	DestRect=Dest;
		Rect	NewR(X,Y,Width,Height);

		DestRect.X=0;
		DestRect.Y=0;

		DestRect.ClipRect(NewR);

		if (NewR)
			{
			int		NumOfCols=MyPal.GetNumOfCols();
			int *	RemapBuffer;

			int 	XIndent=NewR.X-X;
			int 	YIndent=NewR.Y-Y;

			u8 *	PicSrc=&Buffa[XIndent+YIndent*Width];

			RemapBuffer=Dest.MyPal.MakeRemapTable(MyPal);

			for (int yy=NewR.Y;yy<NewR.Y+NewR.H;yy++)
				{
				for (int xx=0;xx<NewR.W;xx++)
					{
					if (PicSrc[(yy-NewR.Y)*Width+xx] != TrCol)
						Dest.Buffa[yy*Dest.Width+NewR.X+xx]=RemapBuffer[PicSrc[(yy-NewR.Y)*Width+xx]];
					}
				}

			delete RemapBuffer;
			}
		}
}


void Frame::ReplaceColour(int ColNum,int RepNum)
{
	if (Buffa && Width && Height)
		{
		if ((ColNum<MyPal.GetNumOfCols()) && (RepNum<MyPal.GetNumOfCols()))
			{
			for (int f=0;f<Width*Height;f++)
				{
				if (Buffa[f]==ColNum)
					Buffa[f]=RepNum;
				}
			}
		}
}


/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void Frame::ReducePal(void)
{
	if (Buffa && Width && Height)
		MyPal.SetPalSize(GetNumOfCols());
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void Frame::Clear(int Col)
{
	if (Buffa && Width && Height)
		memset(Buffa,Col,Width*Height);
}



/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
struct ColPair
{
	Colour	C;
	int		Uses;

	bool	operator==(ColPair const &Col) const
		{return(Uses==Col.Uses);}

	bool	operator<(ColPair const &Col) const
		{return(Uses<Col.Uses);}
};

class PredicateColPair
{
public:
	bool operator()(ColPair const & R1,ColPair const & R2) const
		{
		return (R2.Uses<R1.Uses);
		}
};

void Frame::ReduceColours(int TargColours)
{
	if (Buffa && TargColours <=255)
		{
		Palette	NewPal;
		
		for (int f=0;f<TargColours;f++)
			NewPal[f].SetRGB(0xff,0,0xff);

		vector<ColPair>	Colours;
		Colours.resize(256);
		
		for (f=0;f<256;f++)
			Colours[f].Uses=0;

		for (f=0;f<Width*Height;f++)
			{
			if (Buffa[f])
				{
				Colours[Buffa[f]].C=MyPal[Buffa[f]];
				Colours[Buffa[f]].Uses++;
				}
			}

		sort(Colours.begin(),Colours.end());

		for (f=63;f>0;f--)
			{
			int ColIndex=255-63+f;

			if (Colours[ColIndex].Uses)
				NewPal[f]=Colours[ColIndex].C;
			}
		Remap(NewPal);
		}
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void Frame::AddPixelSurround(int Col)
{
	if (Buffa && Col < MyPal.GetNumOfCols())
		{
		Frame	NewFrame;

		NewFrame=*this;
		NewFrame.Width+=2;
		NewFrame.Height+=2;

		NewFrame.DumpStuff();

		if(!(NewFrame.Buffa=new u8[NewFrame.Width*NewFrame.Height]))
			Error(ERM_OUTOFMEM);

		memset(NewFrame.Buffa,0,NewFrame.Width*NewFrame.Height);

		PlotTrans(NewFrame,0,0);
		PlotTrans(NewFrame,0,1);
		PlotTrans(NewFrame,0,2);
		PlotTrans(NewFrame,1,0);
		PlotTrans(NewFrame,1,2);
		PlotTrans(NewFrame,2,0);
		PlotTrans(NewFrame,2,1);
		PlotTrans(NewFrame,2,2);

		for (int f=0;f<NewFrame.Width*NewFrame.Height;f++)
			{
			if (NewFrame.Buffa[f])
				NewFrame.Buffa[f]=Col;
			}

		PlotTrans(NewFrame,1,1);
		*this=NewFrame;
		}
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void Frame::Expand(int BorderWidth,bool DupeEdges)
{
	Frame		NewFrame;
	NewFrame=*this;
	NewFrame.Width+=BorderWidth*2;
	NewFrame.Height+=BorderWidth*2;

	if(!(NewFrame.Buffa=new u8[NewFrame.Width*NewFrame.Height]))
		Error(ERM_OUTOFMEM);

	memset(NewFrame.Buffa,0,NewFrame.Width*NewFrame.Height);

	if (DupeEdges)
		{
		for (int f=0;f<BorderWidth*2+1;f++)
			Plot(NewFrame,f,BorderWidth);

		for (f=0;f<BorderWidth*2+1;f++)
			Plot(NewFrame,BorderWidth,f);
		}

	Plot(NewFrame,BorderWidth,BorderWidth);
		
	*this=NewFrame;

}

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
Rect::Rect(void)
{

	X=0;
	Y=0;
	W=0;
	H=0;
}
	
/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
ostream & operator<<(ostream & str,Rect const & Fr)
{
	str<<"X,Y = ("<<Fr.X<<","<<Fr.Y<<") WH=("<<Fr.W<<","<<Fr.H<<")";
	return(str);
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void Rect::ClipRect(Rect & RectToClip) const
{
	if (IsColiding(RectToClip))
		{
		if (RectToClip.X < X)
			{
			RectToClip.W-=X-RectToClip.X;
			RectToClip.X=X;
			}

		if (RectToClip.Y < Y)
			{
			RectToClip.H-=Y-RectToClip.Y;
			RectToClip.Y=Y;
			}

		if ((RectToClip.X+RectToClip.W) > (X+W))
			RectToClip.W-=(RectToClip.X+RectToClip.W)-(X+W);

		if ((RectToClip.Y+RectToClip.H) > (Y+H))
			RectToClip.H-=(RectToClip.Y+RectToClip.H)-(Y+H);
		}
	else
		{
		RectToClip.W=0;
		RectToClip.H=0;
		RectToClip.X=0;
		RectToClip.Y=0;
		}
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
bool Rect::IsColiding(Rect const & NewRect) const
{
	if ((NewRect.X+NewRect.W) <= X)
		return(false);

	if ((X+W) <= NewRect.X)
		return(false);

	if ((NewRect.Y+NewRect.H) <= Y)
		return(false);

	if ((Y+H) <= NewRect.Y)
		return(false);

	return(true);
}


struct RGB
{
	u8	r,g,b;
};

class BITMAP
{
public:
	BITMAP(void)
		{
		m_width=0;
		m_height=0;
		}

	void setSize(int width,int height)
		{
		bitMap.resize(width*height);

		m_width=width;
		m_height=height;
		}

	void clear(void)
		{
		if (m_width && m_height)
			memset(&bitMap[0],0,m_width*m_height);
		}

	void line(unsigned int y,unsigned int x,u8 pix)
		{
		if (x >= m_width || y >= m_height)
			GObject::Error(ERR_FATAL,"out of bounds");
		else
			bitMap[y*m_width+x]=pix;
		}

	u8 const * getBitMap(void) const
		{return(&bitMap[0]);}

protected:
	int				m_width;
	int				m_height;
	vector<u8>		bitMap;
};

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <limits.h>


#define BI_RGB          0
#define BI_RLE8         1
#define BI_RLE4         2
#define BI_BITFIELDS    3

#define OS2INFOHEADERSIZE  12
#define WININFOHEADERSIZE  40


typedef struct BITMAPFILEHEADER
{
   unsigned long  bfType;
   unsigned long  bfSize;
   unsigned short bfReserved1;
   unsigned short bfReserved2;
   unsigned long  bfOffBits;
} BITMAPFILEHEADER;


/* Used for both OS/2 and Windows BMP. 
 * Contains only the parameters needed to load the image 
 */
typedef struct BITMAPINFOHEADER
{
   unsigned long  biWidth;
   unsigned long  biHeight;
   unsigned short biBitCount;
   unsigned long  biCompression;
} BITMAPINFOHEADER;


typedef struct WINBMPINFOHEADER  /* size: 40 */
{
   unsigned long  biWidth;
   unsigned long  biHeight;
   unsigned short biPlanes;
   unsigned short biBitCount;
   unsigned long  biCompression;
   unsigned long  biSizeImage;
   unsigned long  biXPelsPerMeter;
   unsigned long  biYPelsPerMeter;
   unsigned long  biClrUsed;
   unsigned long  biClrImportant;
} WINBMPINFOHEADER;


typedef struct OS2BMPINFOHEADER  /* size: 12 */
{
   unsigned short biWidth;
   unsigned short biHeight;
   unsigned short biPlanes;
   unsigned short biBitCount;
} OS2BMPINFOHEADER;



/* read_bmfileheader:
 *  Reads a BMP file header and check that it has the BMP magic number.
 */
static int read_bmfileheader(Gifstream & f, BITMAPFILEHEADER *fileheader)
{
   fileheader->bfType = f.Get16();
   fileheader->bfSize= f.Get32();
   fileheader->bfReserved1= f.Get16();
   fileheader->bfReserved2= f.Get16();
   fileheader->bfOffBits= f.Get32();

   if (fileheader->bfType != 19778)
      return -1;

   return 0;
}



/* read_win_bminfoheader:
 *  Reads information from a BMP file header.
 */
static int read_win_bminfoheader(Gifstream & f, BITMAPINFOHEADER *infoheader)
{
   WINBMPINFOHEADER win_infoheader;

   win_infoheader.biWidth = f.Get32();
   win_infoheader.biHeight = f.Get32();
   win_infoheader.biPlanes = f.Get16();
   win_infoheader.biBitCount = f.Get16();
   win_infoheader.biCompression = f.Get32();
   win_infoheader.biSizeImage = f.Get32();
   win_infoheader.biXPelsPerMeter = f.Get32();
   win_infoheader.biYPelsPerMeter = f.Get32();
   win_infoheader.biClrUsed = f.Get32();
   win_infoheader.biClrImportant = f.Get32();

   infoheader->biWidth = win_infoheader.biWidth;
   infoheader->biHeight = win_infoheader.biHeight;
   infoheader->biBitCount = win_infoheader.biBitCount;
   infoheader->biCompression = win_infoheader.biCompression;

   return 0;
}



/* read_os2_bminfoheader:
 *  Reads information from an OS/2 format BMP file header.
 */
static int read_os2_bminfoheader(Gifstream & f, BITMAPINFOHEADER *infoheader)
{
   OS2BMPINFOHEADER os2_infoheader;

   os2_infoheader.biWidth = f.Get16();
   os2_infoheader.biHeight = f.Get32();
   os2_infoheader.biPlanes = f.Get32();
   os2_infoheader.biBitCount = f.Get32();

   infoheader->biWidth = os2_infoheader.biWidth;
   infoheader->biHeight = os2_infoheader.biHeight;
   infoheader->biBitCount = os2_infoheader.biBitCount;
   infoheader->biCompression = 0;

   return 0;
}


/* read_bmicolors:
 *  Loads the color pallete for 1,4,8 bit formats.
 */
static void read_bmicolors(int ncols, RGB *pal, Gifstream & f,int win_flag)
{
	int i;

	for (i=0; i<ncols; i++)
		{
		pal[i].b = f.get();
		pal[i].g = f.get();
		pal[i].r = f.get();

		if (win_flag)
			f.get();
		}
}



/* read_1bit_line:
 *  Support function for reading the 1 bit bitmap file format.
 */
static void read_1bit_line(int length, Gifstream & f, BITMAP *bmp, int line)
{
   unsigned char b[32];
   unsigned long n;

   int i, j, k;
   int pix;

	for (i=0; i<length; i++)
		{
		j = i % 32;
		if (j == 0)
			{
			n = f.Get32();

			for (k=0; k<32; k++)
				{
				b[31-k] = n & 1;
				n = n >> 1;
				}
	      }

		pix = b[j];
		bmp->line(line,i,pix);
		}
}



/* read_4bit_line:
 *  Support function for reading the 4 bit bitmap file format.
 */
static void read_4bit_line(int length, Gifstream & f, BITMAP *bmp, int line)
{
   unsigned char b[8];
   unsigned long n;
   int i, j, k;
   int temp;
   int pix;

	for (i=0; i<length; i++)
		{
		j = i % 8;
		if (j == 0)
			{
			n = f.Get32();

			for (k=0; k<4; k++)
				{
				temp = n & 255;
				b[k*2+1] = temp & 15;
				temp = temp >> 4;
				b[k*2] = temp & 15;
				n = n >> 8;
				}
			}

		pix = b[j];
		bmp->line(line,i,pix);
		}
}



/* read_8bit_line:
 *  Support function for reading the 8 bit bitmap file format.
 */
static void read_8bit_line(int length, Gifstream & f, BITMAP *bmp, int line)
{
   unsigned char b[4];
   unsigned long n;
   int i, j, k;
   int pix;

	for (i=0; i<length; i++)
		{
		j = i % 4;
		if (j == 0)
			{
			n = f.Get32();
			for (k=0; k<4; k++)
				{
				b[k] = n & 255;
				n = n >> 8;
				}
			}
		pix = b[j];
		bmp->line(line,i,pix);
		}
}


/* read_24bit_line:
 *  Support function for reading the 24 bit bitmap file format, doing
 *  our best to convert it down to a 256 color pallete.
 */
static void read_24bit_line(int length, Gifstream & f, BITMAP *bmp, int line)
{
#if 0
   int i, nbytes;
   RGB c;

   nbytes=0;

   for (i=0; i<length; i++) {
      c.b = f.get();
      c.g = f.get();
      c.r = f.get();
      bmp->line[line][i*3+_rgb_r_shift_24/8] = c.r;
      bmp->line[line][i*3+_rgb_g_shift_24/8] = c.g;
      bmp->line[line][i*3+_rgb_b_shift_24/8] = c.b;
      nbytes += 3;
   }

   nbytes = nbytes % 4;
   if (nbytes != 0)
      for (i=nbytes; i<4; i++)
	 f.get();;
#endif
}



/* read_image:
 *  For reading the noncompressed BMP image format.
 */
static void read_image(Gifstream & f, BITMAP *bmp, BITMAPINFOHEADER *infoheader)
{
   int i, line;

   for (i=0; i<(int)infoheader->biHeight; i++) {
      line = i;

      switch (infoheader->biBitCount) {

	 case 1:
	    read_1bit_line(infoheader->biWidth, f, bmp, infoheader->biHeight-i-1);
	    break;

	 case 4:
	    read_4bit_line(infoheader->biWidth, f, bmp, infoheader->biHeight-i-1);
	    break;

	 case 8:
	    read_8bit_line(infoheader->biWidth, f, bmp, infoheader->biHeight-i-1);
	    break;

	 case 24:
	    read_24bit_line(infoheader->biWidth, f, bmp, infoheader->biHeight-i-1);
	    break;
      }
   }
}



/* read_RLE8_compressed_image:
 *  For reading the 8 bit RLE compressed BMP image format.
 */
static void read_RLE8_compressed_image(Gifstream & f, BITMAP *bmp, BITMAPINFOHEADER *infoheader)
{
   unsigned char count, val, val0;
   int j, pos, line;
   int eolflag, eopicflag;

   eopicflag = 0;
   line = infoheader->biHeight - 1;

   while (eopicflag == 0) {
      pos = 0;                               /* x position in bitmap */
      eolflag = 0;                           /* end of line flag */

      while ((eolflag == 0) && (eopicflag == 0)) {
	 count = f.get();
	 val = f.get();

	 if (count > 0) {                    /* repeat pixel count times */
	    for (j=0;j<count;j++) {
	       bmp->line(line,pos,val);
	       pos++;
	    }
	 }
	 else {
	    switch (val) {

	       case 0:                       /* end of line flag */
		  eolflag=1;
		  break;

	       case 1:                       /* end of picture flag */
		  eopicflag=1;
		  break;

	       case 2:                       /* displace picture */
		  count = f.get();
		  val = f.get();
		  pos += count;
		  line -= val;
		  break;

	       default:                      /* read in absolute mode */
		  for (j=0; j<val; j++) {
		     val0 = f.get();
		     bmp->line(line,pos,val0);
		     pos++;
		  }

		  if (j%2 == 1)
		     val0 = f.get();    /* align on word boundary */
		  break;

	    }
	 }

	 if (pos > (int)infoheader->biWidth)
	    eolflag=1;
      }

      line--;
      if (line < 0)
	 eopicflag = 1;
   }
}



/* read_RLE4_compressed_image:
 *  For reading the 4 bit RLE compressed BMP image format.
 */
static void read_RLE4_compressed_image(Gifstream & f, BITMAP *bmp, BITMAPINFOHEADER *infoheader)
{
   unsigned char b[8];
   unsigned char count;
   unsigned short val0, val;
   int j, k, pos, line;
   int eolflag, eopicflag;

   eopicflag = 0;                            /* end of picture flag */
   line = infoheader->biHeight - 1;

   while (eopicflag == 0) {
      pos = 0;
      eolflag = 0;                           /* end of line flag */

      while ((eolflag == 0) && (eopicflag == 0)) {
	 count = f.get();
	 val = f.get();

	if (count > 0) {                    /* repeat pixels count times */
	    b[1] = val & 15;
	    b[0] = (val >> 4) & 15;
	    for (j=0; j<count; j++) {
	       bmp->line(line,pos,b[j%2]);
	       pos++;
	    }
	 }
	 else {
	    switch (val) {

	       case 0:                       /* end of line */
		  eolflag=1;
		  break;

	       case 1:                       /* end of picture */
		  eopicflag=1;
		  break;

	       case 2:                       /* displace image */
		  count = f.get();
		  val = f.get();
		  pos += count;
		  line -= val;
		  break;

	       default:                      /* read in absolute mode */
		  for (j=0; j<val; j++) {
		     if ((j%4) == 0) {
			val0 = f.Get16();
			for (k=0; k<2; k++) {
			   b[2*k+1] = val0 & 15;
			   val0 = val0 >> 4;
			   b[2*k] = val0 & 15;
			   val0 = val0 >> 4;
			}
		     }
		     bmp->line(line,pos,b[j%4]);
		     pos++;
		  }
		  break;
	    }
	 }

	 if (pos > (int)infoheader->biWidth)
	    eolflag=1;
      }

      line--;
      if (line < 0)
	 eopicflag = 1;
   }
}



/* load_bmp:
 *  Loads a Windows BMP file, returning a bitmap structure and storing
 *  the pallete data in the specified pallete (this should be an array of
 *  at least 256 RGB structures).
 *
 *  Thanks to Seymour Shlien for contributing this function.
 */

static void load_bmp(Frame & frm,char const *filename)
{
	BITMAPFILEHEADER fileheader;
	BITMAPINFOHEADER infoheader;

	RGB		pal[256];
	BITMAP	bmp;

	Gifstream	f(Gifstream::LITTLE_ENDIAN);

	int ncol;
	unsigned long biSize;

	f.open(filename,ios::in|ios::binary);

	if (!f)
		GObject::Error(ERR_FATAL,"couldn't open file %s",filename);

	if (read_bmfileheader(f, &fileheader) != 0)
		{
		GObject::Error(ERR_FATAL,"error reading bmp hdr for %s",filename);
		}

   biSize = f.Get32();

	if (biSize == WININFOHEADERSIZE)
		{
		if (read_win_bminfoheader(f, &infoheader) != 0)
			GObject::Error(ERR_FATAL,"error reading windows bmp info hdr for %s",filename);

		/* compute number of colors recorded */
		ncol = (fileheader.bfOffBits - 54) / 4;
		read_bmicolors(ncol, pal, f, 1);
		}
	else if (biSize == OS2INFOHEADERSIZE)
		{
		if (read_os2_bminfoheader(f, &infoheader) != 0)
			GObject::Error(ERR_FATAL,"error reading os2 bmp info hdr for %s",filename);
		
		/* compute number of colors recorded */
		ncol = (fileheader.bfOffBits - 26) / 3;
		read_bmicolors(ncol, pal, f, 0);
		}
	else
		{
		GObject::Error(ERR_FATAL,"error finding correct hdr for bmp %s",filename);
		}

	if (infoheader.biBitCount != 4 && infoheader.biBitCount != 8)
		GObject::Error(ERR_FATAL,"only handles 4 && 8 bit bmps not %d : %s",infoheader.biBitCount,filename);


	bmp.setSize(infoheader.biWidth, infoheader.biHeight);

	bmp.clear();

	switch (infoheader.biCompression)
		{

		case BI_RGB:
			read_image(f, &bmp, &infoheader);
			break;

		case BI_RLE8:
			read_RLE8_compressed_image(f, &bmp, &infoheader);
			break;

		case BI_RLE4:
			read_RLE4_compressed_image(f, &bmp, &infoheader);
			break;

		default:
			GObject::Error(ERR_FATAL,"unknown compression foramt for %s",filename);
			break;
		}

	f.close();


		{
		Palette	palObj;



		for (int f=0;f<ncol;f++)
			{
			Colour & col = palObj[f];
			col.SetRGB(pal[f].r,pal[f].g,pal[f].b);
			}

		frm.SetFrame(bmp.getBitMap(),infoheader.biWidth,infoheader.biHeight,palObj);
		}

}

/*===========================================================================
 end */


