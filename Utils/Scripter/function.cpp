/*=========================================================================

	function.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/


/*----------------------------------------------------------------------
	Includes
	-------- */

#include "function.h"

#ifndef _LEXER_H
#include "lexer.h"
#endif

#ifndef _PARSER_H
#include "parser.h"
#endif


/*	Std Lib
	------- */

#include <stdio.h>
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
static FunctionDef s_functionNames[]=
{
	{ 0,	"print",	1,	},
};
static int		s_functionCount=sizeof(s_functionNames)/sizeof(FunctionDef);

extern mylexer s_lexer;
extern myparser s_parser;


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
extern FunctionDef *lookupFunctionName(char *_name)
{
	int			i;
	FunctionDef	*fp;

	fp=s_functionNames;
	for(i=0;i<s_functionCount;i++)
	{
		if(strcmp(fp->m_name,_name)==0)
		{
			return fp;
		}
		fp++;
	}

	printf("Unknown function name _%s\n",_name);
	s_lexer.error();
	return NULL;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:	Argument count for the requestion function
  ---------------------------------------------------------------------- */
extern int getFunctionArgCount(int _functionNumber)
{
	return s_functionNames[_functionNumber].m_argCount;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:	A treenode that contains all of the functions arguments
  ---------------------------------------------------------------------- */
extern CTreeNode *getFunctionArgs(int _argCount)
{
	int	i;
	int	tokenType;

	if(_argCount)
	{
		CTreeNode	*base;

//		base=new CTreeNode(STMT_LIST
		for(i=0;i<_argCount;i++)
		{
			tokenType=s_parser.yygettoken();
			switch(tokenType)
			{
				case VARIABLE:
					break;
				case VALUE:
					break;
				default:
					printf("UNEXPECTED TOKEN '%s' FOR ARGUMENT %d\n",s_lexer.yytext,i+1);
					s_lexer.error();
					break;
			}
		}

		return new CTreeNode(EMPTY_STMT);
	}
	else
	{
		return new CTreeNode(EMPTY_STMT);
	}
}


/*===========================================================================
 end */
