/*=========================================================================

	pfile.h

	Author:		PKG
	Created:
	Project:	Spongebob
	Purpose:

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef __PFILE_H__
#define	__PFILE_H__

/*----------------------------------------------------------------------
	Includes
	-------- */

/*	Std Lib
	------- */

#include <stdio.h>


/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

class CPFile
{
public:
				CPFile();

	int			open(char *_filename);
	int			close();

	static class CPFile		*getCurrentFile()	{return s_stack;}

	int			readChar();

	void		error(FILE *_fh);
	int			getErrorCount()					{return m_errorCount;}

private:
	FILE		*m_fh;
	char		*m_filename;
	int			m_charCount;
	int			m_lineCount;
	int			m_currentCharOnLine;
	int			m_errorCount;

	class CPFile			*m_next;
	static class CPFile		*s_stack;
};


/*----------------------------------------------------------------------
	Globals
	------- */

/*----------------------------------------------------------------------
	Functions
	--------- */

extern int openPFile(char *_filename);
extern int closePFile();


/*---------------------------------------------------------------------- */

#endif	/* __PFILE_H__ */

/*===========================================================================
 end */
