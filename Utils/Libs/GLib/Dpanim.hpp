/*=========================================================================

	DPANM.HPP	

	Author:  Gary Liddon @ Fareham
	Created:
	Project: 
	Purpose:

	Copyright (c) 1997 Gary Liddon

===========================================================================*/

#ifndef __DPANM_HPP__
#define __DPANM_HPP__

/*----------------------------------------------------------------------
	Includes
	-------- */

/*	Std Lib
	------- */
#include <fstream>
#include <vector>

/*	Glib
	---- */

/*	Local
	----- */
#include "gtypes.h"
#include "ganim.hpp"

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */
#define MAX_LARGE_PAGE					256
#define MAX_RECORDS_PER_LP				256

/*----------------------------------------------------------------------
	Class defintions
	---------------- */
class CompFrame;
class LpObj;

typedef std::vector<LpObj>		LpObjVec;

class GLIB_API DpAnimFilter : public GAnimFilter
{
public:
	DpAnimFilter();
	~DpAnimFilter(void);
	DpAnimFilter(char const * FName);

	virtual bool Load(GAnim & Anm);
	virtual bool Save(GAnim & Anm);

	virtual	GAnimFilter * Create(char const * Name)	{return (new DpAnimFilter(Name));}

protected:

	class AnimHdr
	{
	public:	
		AnimHdr();
		void InitHdr(void);


			ULONG id;					// ID == "IPF "
			UWORD maxLps;				// Maximum Number of LPs
			UWORD nLps;				// Number of LPs in file
			ULONG lRecords;			// Number of records in file
			UWORD maxRecsPerLp;		// Maximum number of records per LP
			UWORD lpfTableOffset;		// Offset to start of LP Table
			ULONG contentType;		// == "ANIM"
			UWORD width;				// Width in pixels of drawing
			UWORD height;				// Height in pixel of drawing
			UBYTE variant;
			UBYTE version;
			UBYTE hasLastDelta;		// Has a delta for first to last
			UBYTE lastDeltaValid;		// The delta is valid
			UBYTE pixelType;
			UBYTE highestBBComp;
			UBYTE otherRecordsPerFrame;
			UBYTE bitmapRecordsPerFrame;
			UBYTE recordTypes[32];
			ULONG nFrames;			// Number of frames in the file
			UWORD framesPerSecond;
			UWORD pad2[29];
	};

	struct ColCycles
	{
	ColCycles();

			UWORD count;
			UWORD rate;
			UWORD flags;
			UBYTE low, high;
	};

	struct LpTable
	{
		UWORD baseRecord;
		UWORD nRecords;
		UWORD nBytes;
	};

	struct LpTableMem
	{
			UWORD baseRecord;
			UWORD nRecords;
			UWORD nBytes;
			UWORD size[MAX_RECORDS_PER_LP];
	};


protected:
	void GetPal(Palette & MyPal);


	ULONG anm_init2(char const * filename);
	ULONG anm_init(FILE * filename);
	int	anm_read_next_frame(void);
	void anm_read_first_frame(void);

	void read_LP(UWORD LP_to_load);
	void PlayRunSkipDump(UBYTE const *src, UBYTE *dst);

	UWORD 	loaded_LP;			// Which LP is currently loaded

	UWORD 	current_Frame;		// Which frame is being displayed
	FILE *	inANM;				// Input anm file
	UBYTE *	lp_buffer;		// Input buffer for the currently loaded LP
	UBYTE *	dst_buffer;		// Location to play the frames to

	void InitVars(void);
	void MakeVars(void);
	void DeleteVars(void);

	AnimHdr 		lpfHdr;
	ColCycles		Color_Cycles;
	LpTable			* lpfTable;
	LpTableMem		* current_LP;


	void MakeAnim(void);
	void WriteHdr(std::ofstream & Out,AnimHdr & A);
	void WriteCycs(std::ofstream & Out);
	void WritePal(std::ofstream & Out,Palette const & P);
	void WriteLpTable(std::ofstream & Out,LpObjVec & LpTable);
	void WriteLps(std::ofstream & Out,LpObjVec & LpTable);
	void DiscardAnim(void);


	void Encode(Frame const & Fr,CompFrame & Cfr);
	void WriteRun(u8 Val,int Length,CompFrame &Cfr);
	void WriteDataRun(u8 const * Data,int Length,CompFrame &Cfr);
	int GetDataLength(u8 const * Src,int BytesLeft);
	int GetRunLength(u8 const * Src,int BytesLeft);
	void WriteEnd(CompFrame &Cfr);

	enum
		{
//		MAX_RUN_WRITE =		16384-10
		MAX_RUN_WRITE =		10240
		};



};

/*---------------------------------------------------------------------- */

#endif	/* __DPANM_HPP__ */

/*===========================================================================
 end */


