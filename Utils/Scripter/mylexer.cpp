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

#ifndef __PREPRO_H__
#include "prepro.h"
#endif


/*	Std Lib
	------- */

#include <yacc.h>
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
	CPFile::getCurrentFile()->error(yyerr);
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int mylexer::getErrorCount()
{
	CPFile	*cpf;
	cpf=CPFile::getCurrentFile();
	if(cpf)
	{
		return cpf->getErrorCount();
	}
	else
	{
		return 0;
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int mylexer::yygetchar()
{
	CPFile	*cpf;
	int		ret;


	cpf=CPFile::getCurrentFile();
	if(cpf)
	{
		ret=cpf->readChar();

	}
	else
	{
		ret=-1;
	}

	// Force compilation to stop after finding errors ( hmm.. )
	cpf=CPFile::getCurrentFile();
	if(cpf)
	{
		if(cpf->getErrorCount())
		{
			printf("Stopping compilation due to errors!\n");
			ret=-1;
		}
	}

	return ret;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void mylexer::unexpectedChar()
{
	int		result;
	char	name[256]="\0";
	char	*replacement;
	int		nextChar;
	char	c[2]="*";	// Err...

	strcat(name,yytext);
	do
	{
		replacement=lookupMacro(name,&result);
		if(replacement)
		{
			break;
		}
		nextChar=yygetchar();
		if(nextChar!=-1)
		{
			if(strlen(name)>=255)
			{
				printf("OVERFLOW WHILE LOOKING UP MACRO\n");
				error();
				return;
			}
			c[0]=(char)nextChar;
			strcat(name,c);
		}
		else
		{
			printf("END OF FILE WHILE LOOKING FOR MACRO\n");
			error();
			return;
		}
	}
	while(result==POSSIBLE_KNOWN_MACRO);

	if(result==KNOWN_MACRO)
	{
		// Err.. shove the string into the input buffer ( is this good? )
		char	*ptr;
		ptr=replacement+strlen(replacement);
		while(ptr!=replacement)
		{
			yyunput(*--ptr);
		}
	}
	else
	{
		printf("UNEXPECTED STRING: '%s'\n",name);
		error();
	}
}

/*===========================================================================
 end */
