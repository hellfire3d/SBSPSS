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
	CPFile	*cp;
	cp=CPFile::getCurrentFile();
	if(cp)
	{
		cp->error(yyerr);
	}
	else
	{
		// Hmm.. this happens when you forget '*/' on the end of a comment
	}
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
	char	*endOfMacro=name;
	char	*replacement;
	int		nextChar;
	char	c[2]="*";	// Err...
	int		extraChars=0;
	
	// Search for a macro
	strcat(name,yytext);
	yyunput(*(name+strlen(name)-1));
	name[strlen(name)-1]='\0';
	while(1)
	{
		do
		{
			nextChar=yyinput();
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
			lookupMacro(name,&result);
		}
		while(result==POSSIBLE_KNOWN_MACRO);

		if(result==UNKNOWN_MACRO)
		{
			if(endOfMacro!=name)
			{
				char	*cp;
				cp=name+strlen(name)-1;
				while(cp!=endOfMacro-1)
				{
					yyunput(*cp--);
					name[strlen(name)-1]='\0';
				}
			}
			break;
		}
		else
		{
			endOfMacro=name+strlen(name);
		}
	}


	replacement=lookupMacro(name,&result);
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
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void mylexer::comment()
{
	typedef enum CommentState	{	INCOMMENT,FOUNDASTERISK,OUTOFCOMMENT	};
	int				nextChar;
	CommentState	commentState=INCOMMENT;

	do
	{
		nextChar=yygetchar();
		if(nextChar==-1)
		{
			printf("END OF FILE FOUND INSIDE COMMENT\n");
			error();
			return;
		}
		switch(commentState)
		{
			case INCOMMENT:
				if(nextChar=='*')
				{
					commentState=FOUNDASTERISK;
				}
				break;
			case FOUNDASTERISK:
				if(nextChar=='/')
				{
					commentState=OUTOFCOMMENT;
				}
				else
				{
					commentState=INCOMMENT;
				}
				break;
			default:
				break;
		}
	}
	while(commentState!=OUTOFCOMMENT);
}


/*===========================================================================
 end */
