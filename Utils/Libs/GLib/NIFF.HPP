/*=========================================================================

	NIFF.HPP

	Author:  Gary Liddon @ Watford
	Created: 27th May 1991
	Purpose: Base (and i do mean base) iff class

	Copyright (c) 1991 - 1997 Gary Liddon

===========================================================================*/

#ifndef __PC_GLIB_NIFF_HPP__
#define __PC_GLIB_NIFF_HPP__

/*----------------------------------------------------------------------
	Includes
	-------- */

/*	Std Lib
	------- */
#include <stdio.h>

/*	Glib
	---- */
#include "gtypes.h"

/*	Local
	----- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */
#define NIFF_SEEK_SET SEEK_SET
#define NIFF_SEEK_CUR SEEK_CUR
#define NIFF_SEEK_END SEEK_END

#define MAKE_ID(a,b,c,d) (U32) ((U32)(d)<<24|(U32)(c)<<16|(U32)(b)<<8|(U32)(a))

#define FORM MAKE_ID('F','O','R','M')
#define PROP MAKE_ID('P','R','O','P')
#define LIST MAKE_ID('L','I','S','T')
#define CAT  MAKE_ID('C','A','T',' ')

#define ILBM MAKE_ID('I','L','B','M')
#define PBM  MAKE_ID('P','B','M',' ')
#define BMHD MAKE_ID('B','M','H','D')
#define BODY MAKE_ID('B','O','D','Y')
#define CMAP MAKE_ID('C','M','A','P')
//#define ANIM MAKE_ID('A','N','I','M')
#define DLTA MAKE_ID('D','L','T','A')

enum errors
{
	NO_ERROR,
	OPEN_ERROR,
	NOT_IFF,
	LOST_FORM,
	LOST_HUNK,
	CORRUPT_BMHD,
	LOST_BODY,
	NO_MEM,
	MANGLED_IFF,
	LOST_COLOUR,
	NOT_PBM,
	PASSED_ERR,
	ANIM_PIC_E,
	NOT_FORM,
};

/*----------------------------------------------------------------------
	Class defintions
	---------------- */
class GLIB_API niff
{
	public:
		niff(char const *name=NULL,U32 form=0L);
		niff(FILE *fp,U32 form=0L);
		~niff();

		int GetNumOfHunks(U32 FormName);
	
		BOOL open(char const *name,U32 form=0L);
		BOOL close();
	
		FILE *get_fp() {return fp;}

		char *error();
	
		BOOL next_form(U32 form=0L);
		BOOL next_hunk(U32 form=0L);
		BOOL mount_form(U32 form=0L);
		U8 *get_hunk();
		BOOL goto_hunk(U32 form);
		void goto_form_start();

		U32 rev_long(U32);
		U16 rev_word(U16 val);

		U32 GetIntelLong();
		U16 GetIntelWord();

		int seek(long offset,int orig);
		long tell();

		void Read(U8 *Buf,long bytes);
	
	protected:
		U32 full_len(U32 val);
	
		BOOL form_mounted,passed_file,file_opened;
		FILE *fp;
	
		int err_no;
	
		U32 form_name,hunk_name,splen;
		long form_start,form_end,iff_base;
};



/*---------------------------------------------------------------------- */

#endif	/* __PC_GLIB_NIFF_HPP__ */

/*===========================================================================
 end */

