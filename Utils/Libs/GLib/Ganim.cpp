/*=========================================================================

	GANIM.CPP	

	Author:  Gary Liddon @ Fareham
	Created:
	Project: 
	Purpose:

	Copyright (c) 1997 Gary Liddon

===========================================================================*/

/*----------------------------------------------------------------------
	Includes
	-------- */

/*	Std Lib
	------- */

/*	STL
	--- */

/*	Glib
	---- */
#include "gfname.hpp"

/*	Local
	----- */
#include "ganim.hpp"

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Function types
	-------------- */

/*----------------------------------------------------------------------
	Vars
	---- */
std::vector<FilterDetails> FilterDetails::AllDetails ;

/*----------------------------------------------------------------------
	Data
	---- */

/*----------------------------------------------------------------------
	Function:		GAnim::GAnim(void)
	Notes:			Constructor
  ---------------------------------------------------------------------- */
GAnim::GAnim(void)
{
	TheFrames.reserve(2000);
}

GAnim::~GAnim(void)
{
}


bool GAnim::Load(GAnimFilter & Af,char const * IoName)
{
	bool	RetVal;

	if (IoName)
		Af.SetName(IoName);

	RetVal=Af.Load(*this);

	return(RetVal);

}

bool GAnim::Save(GAnimFilter & Af,char const * IoName)
{
	bool	RetVal;

	if (IoName)
		Af.SetName(IoName);

	RetVal=Af.Save(*this);

	return(RetVal);
}

Frame & GAnim::GetNewFrame(void)
{
	return(operator[](TheFrames.size()));
}

int GAnim::AddPalette(Palette & P)
{
	for (int f=0;f<ThePals.size();f++)
		{
		if (P==ThePals[f])
			return(f);
		}

	ThePals.push_back(P);
	return(ThePals.size()-1);
}

/*----------------------------------------------------------------------
	Function:		GAnimFilter::GAnimFilter(char const * FName)
	Notes:			Constructor
  ---------------------------------------------------------------------- */
GAnimFilter::GAnimFilter(char const * FName)
{
	FileName=FName;
}


FilterDetails::FilterDetails(GAnimFilter & nAnmFilter,char const * nExt)
{
	Ext=nExt;
	Ext.Lower();
	AnmFilter=nAnmFilter.Create(NULL);
	AllDetails.push_back(*this);
}

GAnimFilter * FilterDetails::GetFilter(char const * Fname)
{
	GAnimFilter *	RetFilter;
	GString	ExtStr(GFName(Fname).Ext());

	ExtStr.Lower();

	RetFilter=NULL;

	for (int f=0;f<AllDetails.size() && !RetFilter;f++)
		{

		if (AllDetails[f].Ext==ExtStr)
			RetFilter=AllDetails[f].AnmFilter->Create(Fname);
		}

		return(RetFilter);
}



Frame & GAnim::operator[](int Index)
{
	if (Index >= TheFrames.capacity())
		TheFrames.reserve(TheFrames.capacity()+1000);

	if (Index >= TheFrames.size())
		TheFrames.resize(TheFrames.size()+1);

	return(TheFrames[Index]);
}

/*===========================================================================
 end */




