/*=========================================================================

	PAL.CPP

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
#include <fstream.h>
#include <math.h>
#include <minmax.h>

/*	STL
	--- */

/*	Glib
	---- */
#include "ilbm.hpp"

/*	Local
	----- */
#include "pal.hpp"


/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Function Prototypes
	------------------- */

/*----------------------------------------------------------------------
	Vars
	---- */

/*----------------------------------------------------------------------
	Data
	---- */

/*----------------------------------------------------------------------
	Palette Class Stuff
  ---------------------------------------------------------------------- */
Palette::Palette(void)
{
//	TheColours.reserve(300);
}


Palette::Palette(Palette const &Fr)
{
	CopyPal(Fr);
}

Palette::~Palette(void)
{
}

void Palette::operator=(Palette const &Fr)
{
	CopyPal(Fr);
}

void Palette::CopyPal(Palette const &Fr)
{
	TheColours.resize(Fr.TheColours.size());

	for (int f=0;f<Fr.TheColours.size();f++)
		{
		TheColours[f]=Fr.TheColours[f];
		}
}

Colour & Palette::operator[](int f)
{
	if (f>=TheColours.capacity())
		{
		TheColours.reserve(TheColours.capacity()+300);
		}


	if (f>=TheColours.size())
		TheColours.resize(f+1);

	return(TheColours[f]);
}

Colour const & Palette::operator[](int f) const
{
	if (f>=TheColours.size())
		return(DummyCol);
	else
		return(TheColours[f]);
}

u8 * Palette::MakeDpPal() const
{
	u8 *	Pals;

	if (!(Pals=new u8[256*3]))
		Error(ERM_OUTOFMEM);

	memset(Pals,0,256*3);

	for (int f=0;f<TheColours.size();f++)
		{
		Pals[f*3+0]=TheColours[f].GetR();
		Pals[f*3+1]=TheColours[f].GetG();
		Pals[f*3+2]=TheColours[f].GetB();
		}
	return(Pals);
}


bool Palette::operator==(Palette const &Fr) const
{
	if (TheColours.size()==Fr.TheColours.size())
		{
		for (unsigned int f=0;f<TheColours.size();f++)
			{
			if (TheColours[f]!=Fr.TheColours[f])
				return(false);
			}
		return(true);
		}

	return(false);
}

bool Palette::FromLbm(char const * Name)
{
	nilbm	MyLbm(Name);

	if (MyLbm.error())
		return(false);
	else
		{
		u8 const *	CMap;
		int			NumOfCols=1<<MyLbm.GetPlanes();
		CMap=MyLbm.SeeCmap();

		for (int f=0;f<NumOfCols;f++)
			{
			(*this)[f].SetR(CMap[f*3]);
			(*this)[f].SetG(CMap[f*3+1]);
			(*this)[f].SetB(CMap[f*3+2]);
			}
		}

	return(true);

}

void Palette::SetPalSize(int NumOfCols)
{
	TheColours.resize(NumOfCols);
}

bool Palette::IsIntersecting(Palette const & ComPal) const
{
	int MinColours=min(TheColours.size(),ComPal.TheColours.size());

	if (MinColours)
		{
		for (int f=0;f<MinColours;f++)
			{
			if (!(TheColours[f] ==ComPal.TheColours[f]))
				return(false);
			}
		}

	return(true);
}

/*----------------------------------------------------------------------
	Colour Class Stuff
  ---------------------------------------------------------------------- */


Colour::Colour(int nR,int nG,int nB)
{
	R=nR;
	G=nG;
	B=nB;
}

bool Colour::operator==(Colour const &Col) const
{
	return((R==Col.R) && (G==Col.G) && (B==Col.B));
}

bool Colour::operator!=(Colour const &Col) const
{
	return(!(*this==Col));
	
}

void Colour::CopyCol(Colour const &Col)
{	 
	R=Col.R;
	G=Col.G;
	B=Col.B;
}


int Colour::DistanceUnroot(Colour const &Col) const
{
	int		Dist;

	int 	RDif=R-Col.R;
	int		GDif=G-Col.G;
	int		BDif=B-Col.B;

	Dist=RDif*RDif+GDif*GDif+BDif*BDif;

	return(Dist);
}

float Colour::Distance(Colour const &Col) const
{
	float	Dist;

	float	RDif=R-Col.R;
	float	GDif=G-Col.G;
	float	BDif=B-Col.B;

	Dist=sqrt(RDif*RDif+GDif*GDif+BDif*BDif);

	return(Dist);
}



int * Palette::MakeRemapTable(Palette const & P) const
{
	int	*		RetTab;

	if (!(RetTab=new int[P.GetNumOfCols()]))
		Error(ERM_OUTOFMEM);


	int		NumOfFCols=P.GetNumOfCols();
	int		NumOfGCols=GetNumOfCols();

	for (int f=1;f<NumOfFCols;f++)
		{
		Colour const *	ThisCol;
		ThisCol=&P[f];
		
		int		MinIndex=-1;
		int		MinDist=-1;

		for (int g=1;g<NumOfGCols && MinDist;g++)
			{
			int	Distance=(*this)[g].DistanceUnroot(*ThisCol);

			if ((MinIndex==-1) || (Distance < MinDist))
				{
				MinIndex=g;
				MinDist=Distance;
				}
			}

		if (MinIndex < 0)
			RetTab[f]=f;
		else
			RetTab[f]=MinIndex;
		}

	RetTab[0]=0;

	return(RetTab);
}

/*===========================================================================
 end */


