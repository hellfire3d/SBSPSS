/*=========================================================================

	FRAME.CPP

	Author:  Gary Liddon @ Fareham
	Created:
	Project:
	Purpose:

	Copyright (c) 1997 G R Liddon

===========================================================================*/

#ifndef __FRAME_HPP__
#define __FRAME_HPP__

/*----------------------------------------------------------------------
	Includes
	-------- */

/*	Std Lib
	------- */
#include <iostream>
#include <vector>

/*	Glib
	---- */
#include "gobject.hpp"
#include "gtypes.h"
#include "gstring.hpp"

/*	Local
	----- */
#include "pal.hpp"

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */
class GLIB_API Rect : public GObject
{
public:
	Rect(Rect const & R)
		{ MakeCopy(R);}
	
	void operator=(Rect const & R)
		{ MakeCopy(R);}
		
	Rect(void);
	Rect(int nX,int nY,int nW,int nH)
		{
		X=nX;
		Y=nY;
		W=nW;
		H=nH;
		}

	/* Compare by volume */

	bool operator< (Rect const & R) const
		{
		return((W*H)<(R.W*R.H));
		}

	bool IsColiding(Rect const & NewRect) const;
	void ClipRect(Rect & RectToClip) const;
	int Area(void)	const {return(W*H);}
	
	operator int() const{return(W || H);}

	GLIB_API friend	std::ostream & operator<<(std::ostream & str,Rect const & Fr);

	int	X,Y;
	int	W,H;

protected:
	void MakeCopy(Rect const & R)
	{
		X=R.X;
		Y=R.Y;
		W=R.W;
		H=R.H;
	}
};

class GLIB_API Frame : public GObject
{
public:
	void	ReduceColours(int TargColours);
	void	Expand(int BorderWidth,bool DupeEdges);
	void	AddPixelSurround(int Col);

	void	MakeRGB(u8 * Dest);
	void	MakeRGBA(u8 * Dest,bool ZeroIsTrans=true);

	Frame(void);
	Frame(Frame const &);
	~Frame(void);

	void Plot(Frame & Dest,int X,int Y);
	void PlotTrans(Frame & Dest,int X,int Y,int TrCol=0);
	void PlotBox(Rect const & R,int Col);

	
	void operator=(Frame const &Fr){CopyFrame(Fr);}

	operator Rect() const{return(Rect(X,Y,Width,Height));}

	bool operator<(Frame const &Fr) const;
	bool operator==(Frame const &Fr) const;

	void SetFrame(u8 const * nBuffa,int W,int H,Palette const & NewPal);

	void ReduceSinglePixels(int Threshold);
	void SaveLbm(char const * Lbm);

	void ReduceSize(void);
	Rect FindBoundingRect(void);
	void Crop(Rect const & RetRect);

	bool Grab(Frame const & Fr,Rect const & RetRect);
	bool IsBlank(u8 BlankVal=0);

	u8 const * SeeData(void) const {return(Buffa);}
	int GetWidth(void) const {return(Width);}
	int GetHeight(void)const {return(Height);}
	int GetNumOfCols(void) const;

	bool DrawBox(Rect const & R,u8 Col);

	const Palette & GetPal(void) const {return(MyPal);}
	
	Palette & GetPal(void){return(MyPal);}

	void	SetPal(Palette const & NewPal){MyPal=NewPal;}
	void	Remap(Palette const & P);
	void 	LoadLbm(char const * Lbm);
	void	ReplaceColour(int ColNum,int RepNum);

	void	SetX(int nX)	{X=nX;}
	void	SetY(int nY)	{Y=nY;}

	int		GetX(void) const {return(X);}
	int		GetY(void) const {return(Y);}
	const char * GetName(void) const	{	return(Name);	}

	void	SetName(const char * NewName)	{	Name=NewName;	}
	void	ReducePal(void);
	void 	Clear(int Col);

	void	LoadBMP(char const * Lbm);

	void	FlipX(void);
	void	FlipY(void);

	void	Resize(int NewWidth,int NewHeight);
	u8		GetPixel(int PixX,int PixY)			{ return(Buffa[PixX+(PixY*Width)]);}
	void	SetPixel(int PixX,int PixY,u8 Col)	{ Buffa[PixX+(PixY*Width)]=Col;}

protected:
	void	CopyFrame(Frame const &);

	void	DumpStuff(void);
	void	InitFrame(void);

	Palette	MyPal;

	GString Name;

	u8 *	Buffa;
	int		Width;
	int		Height;
	int		X,Y;
};

/*---------------------------------------------------------------------- */

#endif	/* __FRAME_HPP__ */

/*===========================================================================
 end */


