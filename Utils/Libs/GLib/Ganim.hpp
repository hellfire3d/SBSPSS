/*=========================================================================

	GANIM.HPP	

	Author:  Gary Liddon @ Fareham
	Created:
	Project: 
	Purpose:

	Copyright (c) 1997 Gary Liddon

===========================================================================*/

#ifndef __GANIM_HPP__
#define __GANIM_HPP__

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
#include "gstring.hpp"

/*	Local
	----- */
#include "frame.hpp"

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Class defintions
	---------------- */
class GLIB_API GAnimFilter;

class GLIB_API GAnim : public GObject
{
public:
	GAnim(void);
	~GAnim(void);

	bool Load(GAnimFilter & Af,char const * Name=NULL);
	bool Save(GAnimFilter & Af,char const * Name=NULL);
	Frame & GetNewFrame(void);

	int AddPalette(Palette & Pal);

	int NumOfFrames(void) {return(TheFrames.size());}

	Palette & GetPal(int PalId)
		{
		if (PalId >= ThePals.size())
			Error(ERR_FATAL,"Bounds error pals %d",PalId);
		return(ThePals[PalId]);
		}

	Frame & operator[](int Index);

protected:
	typedef std::vector<Palette>	PalVec;
	typedef PalVec::iterator		PalVecIt;

	typedef std::vector<Frame>		FrameVec;
	typedef FrameVec::iterator		FrameVecIt;

	PalVec		ThePals;
	FrameVec	TheFrames;

};



class GLIB_API GAnimFilter : public GObject
{
public:
	GAnimFilter(){;}
	GAnimFilter(char const * FName);

	virtual bool Load(GAnim & Anm)=0;
	virtual bool Save(GAnim & Anm)=0;

	virtual	GAnimFilter * Create(char const *)=0;
	void SetName(char const * NewName) {FileName=NewName;}
	char const * GetName(void) const {return(FileName);}

protected:
	GString	FileName;

};

class GLIB_API FilterDetails
{
public:
	FilterDetails();

	FilterDetails(GAnimFilter & nAnmFilter,char const * nExt);

	static GAnimFilter * GetFilter(char const * Fname);

	bool operator<(FilterDetails const &) const;
	bool operator==(FilterDetails const &) const;

protected:
	GString				Ext;
	GAnimFilter	*		AnmFilter;

	static std::vector<FilterDetails>	AllDetails;
};

/*---------------------------------------------------------------------- */

#endif	/* __GANIM_HPP__ */

/*===========================================================================
 end */


