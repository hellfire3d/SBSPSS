/*=========================================================================

	FILEIO.CPP

	Author:  Tim Swann @ CLIMAX
	Created:
	Project:
	Purpose:

	Copyright (c) 1998 Climax Development Ltd

===========================================================================*/

/*----------------------------------------------------------------------
	Includes
	-------- */

#include "AsciiExp.h"
#include "ExpFileIO.h"


/*	Std Lib
	------- */

/*	Glib
	---- */

/*	Local
	----- */

/*	Graphics
	-------- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

/*----------------------------------------------------------------------
	Positional Vars
	--------------- */

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
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */

void FIO_Stripfileinfo(char * Name, char * fname, char * ext, char * dir, char * drive)
{
	char tdrive[_MAX_DRIVE];
	char tdir[_MAX_DIR];
	char tfname[_MAX_FNAME];
	char text[_MAX_EXT];


	_splitpath(Name, tdrive, tdir, tfname, text);

	if (fname)
		strcpy(fname, tfname);

	if (ext)
		strcpy(ext, text);

	if (dir)
		strcpy(dir, tdir);

	if (drive)
		strcpy(drive, tdrive);

}


/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */

bool FIO_FileExists(const char * Name)
{
	int hFile;

	/* Open input file */
	hFile = _open(Name, _O_RDONLY | _O_BINARY);

	if(hFile != -1)
	{
		_close(hFile);							// file close
		return(TRUE);
	}
	else
	{
		return(FALSE);
	}
}


/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */

void * FIO_Read(char const * Name, int Offset)
{
	int		hFile;
	long	filelen;
	void *	pAddr;
	long	pos;


	/* Open input file */
	hFile = _open(Name, _O_RDONLY | _O_BINARY);

	if(hFile != -1)
	{
		filelen = FIO_FileLen(Name);		// length of file
		pAddr = malloc(filelen);			// allocate mainmemory

		pos = _lseek( hFile, Offset, SEEK_SET );
		if( pos == -1 )	assert(!"File seek");
		filelen -= pos;

		_read(hFile, pAddr, filelen);		// read in file
		_close(hFile);						// file close
	}
	else
	{
		assert(0);
	}
	return pAddr;
}


/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */

bool FIO_ReadAtAddr(char const * Name, unsigned char * Dest, int Len, int Offset)
{
	int		hFile;
	long	filelen;
	long	pos;


	/* Open input file */
	hFile = _open(Name, _O_RDONLY | _O_BINARY);

	if(hFile != -1)
	{
		filelen = FIO_FileLen(Name);			// length of file

		if (Len && (Len < filelen))	filelen = Len;

		pos = _lseek( hFile, Offset, SEEK_SET );
		if( pos == -1 )	assert(!"File seek");

		_read(hFile, Dest, filelen);			// read in file
		_close(hFile);							// file close
	}
	else
	{
		assert(0);
	}
	return TRUE;
}


/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */

int FIO_FileLen(const char * Name)
{
	int hFile;
	int filelen = 0;


	/* Open input file */
	hFile = _open(Name, _O_RDONLY | _O_BINARY);

	if(hFile != -1)
	{
		filelen = _filelength(hFile);			// length of file
		_close(hFile);							// file close
	}
	return filelen;
}


/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */

bool FIO_Save(const char * Name,unsigned char * Addr,int Len)
{
	HANDLE			hFile;
	UINT			fResult;
	DWORD			nWrited;


	/* Open output file */
	hFile = CreateFile(	Name,						/* output filename */
						GENERIC_WRITE,				/* mode = write */
						0,							/* share mode = not share */
						NULL,						/* security attribute = cancel */
						CREATE_ALWAYS,				/* create file always */
						FILE_ATTRIBUTE_NORMAL,		/* file attribute = normal */
						NULL						/* template file = non */
						);
	if(hFile)
	{
		fResult = WriteFile(hFile,			/* IN  : file handle */
							Addr,			/* IN  : pointer to write data */
							(DWORD)Len,		/* IN  : number of data in bytes */
							&nWrited,		/* OUT : number of data just writed */
							NULL			/* IN  : pointer to OVERLAPPED structure */
						  	);
		CloseHandle(hFile);							/* close file */

		if(fResult == FALSE)
			assert(!("SaveFile: ERROR Can not write file %s",Name));
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}

  
  /*===========================================================================
 end */
