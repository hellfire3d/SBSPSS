/*=========================================================================

	FILEIO.H	

	Author:  Tim Swann @ CLIMAX
	Created:
	Project:
	Purpose:

	Copyright (c) 1998 Climax Development Ltd

===========================================================================*/

#ifndef __FILEIO__
#define __FILEIO__

/*----------------------------------------------------------------------
	Includes
	-------- */

/*	Std Lib
	------- */

/*	Glib
	---- */

/*	Local
	----- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

/*----------------------------------------------------------------------
	Globals
	------- */

/*	Vars
	---- */

/*	Data
	---- */

/*	Functions
	--------- */

void *	FIO_Read(char const * Name, int Offset = 0);
bool	FIO_ReadAtAddr(char const * Name,unsigned char * Dest,int Len = 0, int Offset = 0);
int		FIO_FileLen(const char * Name);
bool	FIO_FileExists(const char * Name);
bool	FIO_Save(const char * Name,unsigned char * Addr,int Len);
void	FIO_Stripfileinfo(char * Name, char * fname = NULL, char * ext = NULL, char * dir = NULL, char * drive = NULL);

/*---------------------------------------------------------------------- */

#endif	/* __FILEIO__ */

/*===========================================================================
 end */
