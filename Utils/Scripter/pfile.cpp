/*=========================================================================

	pfile.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/


/*----------------------------------------------------------------------
	Includes
	-------- */

#include "pfile.h"


/*	Std Lib
	------- */

#include <stdlib.h>
#include <string.h>


/*	Data
	---- */

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

	FILE		*getFh()						{return m_fh;}

	static class CPFile		*getCurrentFile()	{return s_stack;}


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
	Function Prototypes
	------------------- */

/*----------------------------------------------------------------------
	Vars
	---- */

CPFile		*CPFile::s_stack=NULL;


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
extern int openPFile(char *_filename)
{
	CPFile	*pf;
	pf=new CPFile;
	return pf->open(_filename);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
extern int closePFile()
{
	CPFile	*pf;
	int ret;
	pf=CPFile::getCurrentFile();
	ret=pf->close();
	delete pf;
	return ret;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
extern FILE	*getPFileFh()
{
	CPFile*pf;

	pf=CPFile::getCurrentFile();
	if(pf)
		return pf->getFh();
	else
		return NULL;
}






/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
CPFile::CPFile()
{
	m_fh=NULL;
	m_filename=NULL;
	m_charCount=0;
	m_lineCount=0;
	m_currentCharOnLine=0;
	m_errorCount=0;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int CPFile::open(char *_filename)
{
	int	ret=1;
	
	printf("Opening %s..\n",_filename);
	if((m_fh=fopen(_filename,"rt"))==NULL)
	{
		printf("FATAL: Couldn't open file for reading\n");
		ret=0;
	}
	else
	{
		m_filename=(char*)malloc(strlen(_filename)+1);
		strcpy(m_filename,_filename);
		m_next=s_stack;
		s_stack=this;
	}
	return ret;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int CPFile::close()
{
	printf("Processed %d char(s), %d line(s) in file %s\n",m_charCount,m_lineCount,m_filename);
	if(m_errorCount)
	{
		printf("Found %d error(s) :(\n",m_errorCount);
	}
	fclose(m_fh);
	if(m_next)
	{
		s_stack=m_next;
	}
	else
	{
		s_stack=NULL;
	}
	return 1;
}


/*===========================================================================
 end */
