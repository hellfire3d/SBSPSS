/*=========================================================================

	PAL.CPP

	Author:  Gary Liddon @ Fareham
	Created:
	Project:
	Purpose:

	Copyright (c) 1997 G R Liddon

===========================================================================*/

#ifndef __PAL_HPP__
#define __PAL_HPP__

/*----------------------------------------------------------------------
	Includes
	-------- */

/*	Std Lib
	------- */

/*	STL
	--- */
#include <vector>

/*	Glib
	---- */
#include "gobject.hpp"
#include "gtypes.h"

/*	Local
	----- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */
class GLIB_API Colour : public GObject
{
public:
	Colour(int nR=0,int nG=0,int nB=0);
	Colour(Colour const & Col)			{CopyCol(Col);}

	void operator=(Colour const &Col)	{CopyCol(Col);}

	int		GetR(void) const {return (R);}
	int		GetG(void) const {return (G);}
	int		GetB(void) const {return (B);}

	void	SetR(int n){R=n;}
	void	SetG(int n){G=n;}
	void	SetB(int n){B=n;}

	void	SetRGB(int nr,int ng,int nb)
		{
		SetR(nr);
		SetG(ng);
		SetB(nb);
		}

	float Distance(Colour const &Col) const;
	int DistanceUnroot(Colour const &Col) const;


	bool	operator==(Colour const &Col) const;
	bool	operator<(Colour const &Col) const;

	bool	operator!=(Colour const &Col) const;

protected:
	void CopyCol(Colour const &);
	int	R;
	int	G;
	int	B;
};


class GLIB_API Palette : public GObject
{
public:
	Palette(void);
	Palette(Palette const &);
	~Palette(void);

	void	operator=(Palette const &Fr);
	bool	operator==(Palette const &Fr) const;
	bool	operator<(Palette const &Fr) const;
	int		GetNumOfCols(void) const {return TheColours.size();}

	int *	MakeRemapTable(Palette const & P) const;

	Colour const & operator[](int) const;
	Colour & operator[](int);

	u8 * MakeDpPal(void) const;

	bool FromLbm(char const * Name);
	void SetPalSize(int NumOfCols);
	bool IsIntersecting(Palette const & ComPal) const;

protected:
	void CopyPal(Palette const &Fr);

	typedef	std::vector <Colour>	ColVec;
	typedef	ColVec::iterator		ColVecIt;


	ColVec	TheColours;
	Colour	DummyCol;
};


/*---------------------------------------------------------------------- */

#endif	/* __PAL_HPP__ */

/*===========================================================================
 end */


