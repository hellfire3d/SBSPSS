/*=========================================================================

	mylexer.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/


/*----------------------------------------------------------------------
	Includes
	-------- */

#ifndef _LEXER_H
#include "lexer.h"
#endif

#ifndef _PARSER_H
#include "parser.h"
#endif

#ifndef __PFILE_H__
#include "pfile.h"
#endif


/*	Std Lib
	------- */

#include <yacc.h>


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

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int	mylexer::openInputFile(char *_filename)
{
	return openPFile(_filename);	
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int mylexer::closeInputFile()
{
	return closePFile();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void mylexer::error()
{
	fprintf(yyerr,"ERROR AT LINE %d, COLUMN %d\n",getCurrentLine(),getCurrentCharOnLine());
	m_errorCount++;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int mylexer::yygetchar()
{
	char	c;
	int		ret;
	FILE	*fh;


	fh=getPFileFh();
	if(fh)
	{
		if(fread(&c,sizeof(c),1,fh)==1)
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
			if(ferror(fh))
			{
				printf("FATAL: Read error!\n");
			}
			else
			{				
				closePFile();
				return yygetchar();
			}
		}

		// Force compilation to stop after finding errors ( hmm.. )
		if(m_errorCount)
		{
			printf("Stopping compilation due to errors!\n");
			ret=-1;
		}
	}
	else
	{
		ret=-1;
	}
	
	return ret;
}


/*===========================================================================
 end */
