/*=========================================================================

	IFF.HPP

	Author:  Carl Muller
	Created: 25 Sep 1990
	Purpose: IFF write stuff, a bit manky

	Copyright (c) 1990 - 1997 Carl Muller

===========================================================================*/

#ifndef __CP_GLIB_IFF_HPP__
#define __CP_GLIB_IFF_HPP__

/*----------------------------------------------------------------------
	Includes
	-------- */

/*	Std Lib
	------- */
#include <stdlib.h>
#include <stdio.h>


/*	Glib
	---- */
#include "gtypes.h"


/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */
const MAXIFFNESTING = 16;

enum Masking
{
	mskNone = 0,
	mskHasMask = 1,
	mskHasTransparentColor = 2,
	mskLasso = 3
};

enum Compression
{
	cmpNone = 0,
	cmpByteRun1 = 1
};

/*----------------------------------------------------------------------
	Class defintions
	---------------- */
class GLIB_API IFF_FILE
{
	private :
		FILE *	file;
		int		stackptr;
		long	stack[MAXIFFNESTING];

	public :
		FILE *	write(char const *filename);
		void	close(void);

		long	tell(void);
		void	form(char name[5]);
		void	endform(void);
		void	chunk(char name[5]);
		void	endchunk(void);

		void	writeword(U16 it) { fputc(it >> 8, file); fputc(it & 255, file); }
		void	writelong(ULONG it) { fputc((UCHAR)((it >> 24)&0xff), file);fputc((UCHAR)((it >> 16)&0xff), file);fputc((UCHAR)((it >> 8)&0xff), file); fputc((UCHAR)(it & 0xff), file); }
		U16		readword(void) { U16 ch=fgetc(file); return (ch<<8)+(U16)fgetc(file); }
		void	writechar(U16 it) { fputc(it, file); }
		U16		readchar(void) { return fgetc(file); }

		void	push(long it) { stack[stackptr++] = it; }
		long	pop(void) { return stack[--stackptr]; }
};



/*----------------------------------------------------------------------
	Inline Functions
	---------------- */
inline U16 getword(FILE *file)
{
	U16 ch = fgetc(file);
	return (ch << 8) + (U16) (fgetc(file));
}

inline void putword(U16 theword, FILE *file)
{
	fputc(theword >> 8, file);
	fputc(theword & 255, file);
}

inline long getlong(FILE *file)
{
	long ch;
	ch = ((long) fgetc(file)) << 24;
	ch |= ((long) fgetc(file)) << 16;
	ch |= ((long) fgetc(file)) << 8;
	ch |= ((long) fgetc(file));
	return ch;
}

inline void get4chars(char *name, FILE *file)
{
	name[0] = fgetc(file);
	name[1] = fgetc(file);
	name[2] = fgetc(file);
	name[3] = fgetc(file);
	name[4] = 0;
}


/*---------------------------------------------------------------------- */

#endif	/* __CP_GLIB_IFF_HPP__ */

/*===========================================================================
 end */
