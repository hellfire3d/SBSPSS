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
	int	ret;
	
	if((m_fh=fopen(_filename,"rt"))==NULL)
	{
		printf("FATAL: Couldn't open %sfor reading\n",_filename);
		ret=false;
	}
	else
	{
		m_filename=(char*)malloc(strlen(_filename)+1);
		strcpy(m_filename,_filename);
		m_next=s_stack;
		s_stack=this;
		ret=true;
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
	printf(" Processed %d char(s) on %d line(s) in file %s\n",m_charCount,m_lineCount,m_filename);
	if(m_errorCount)
	{
		printf(" Found %d error(s) :(\n",m_errorCount);
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

	return true;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:		Returns ASCII code, or -1 for eof, as per yygetchar()
	Returns:
  ---------------------------------------------------------------------- */
int CPFile::readChar()
{
	int		ret;
	char	c;

	if(fread(&c,sizeof(c),1,m_fh)==1)
	{
		m_charCount++;
		m_currentCharOnLine++;
		if(c=='\n')
		{
			m_lineCount++;
			m_currentCharOnLine=0;
		}
		ret=c;
	}
	else
	{
		ret=-1;
		if(ferror(m_fh))
		{
			printf("FATAL: Read error!\n");
		}
		else
		{				
			closePFile();
			if(getCurrentFile())
			{
				return getCurrentFile()->readChar();
			}
		}
	}

	return ret;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:		Returns ASCII code, or -1 for eof, as per yygetchar()
	Returns:
  ---------------------------------------------------------------------- */
void CPFile::error(FILE *_fh)
{
	fprintf(_fh,"ERROR IN %s LINE %d, COLUMN %d\n",m_filename,m_lineCount+1,m_currentCharOnLine);
	m_errorCount++;
}

/*===========================================================================
 end */
