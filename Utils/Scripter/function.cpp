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

struct FunctionDef
{
	char	*m_name;
	int		m_argCount;
};


/*----------------------------------------------------------------------
	Function Prototypes
	------------------- */

/*----------------------------------------------------------------------
	Vars
	---- */
static FunctionDef s_functionNames[]=
{
	{	"setCharacterAnimation",		2	},		// characterId,animationId
	{	"setText",						2	},		// characterId,textId
	{	"setResponseOptions",			1	},		// optionsId
	{	"getResponse",					0	},		// 
	{	"getAmmoCount",					1	},		// ammoId
	{	"setAmmoCount",					2	},		// ammoId,amount
	{	"isHoldingWeapon",				1	},		// weaponId
	{	"giveWeapon",					1	},		// weaponId
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
extern int lookupFunctionName(char *_name)
{
	int			i;
	FunctionDef	*fp;

	fp=s_functionNames;
	for(i=0;i<s_functionCount;i++)
	{
		if(strcmp(fp->m_name,_name)==0)
		{
			return i;
		}
		fp++;
	}

	printf("Unknown function name _%s\n",_name);
	s_lexer.error();
	return -1;
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
extern CTreeNode *getFunctionArgs(int _functionNumber)
{
	int	argCount,i;
	int	tokenType;

	argCount=s_functionNames[_functionNumber].m_argCount;
	if(argCount)
	{
		CTreeNode	*base;

		base=NULL;
		for(i=0;i<argCount;i++)
		{
			tokenType=s_parser.yygettoken();
			switch(tokenType)
			{
				case VARIABLE:
					{
					int var;
					var=*((int*)s_parser.yylvalptr);
					if(base)
						base=new CTreeNode(STMT_LIST,base,new CTreeNode(VARIABLE_EXPR,(int)var));
					else
						base=new CTreeNode(STMT_LIST,new CTreeNode(VARIABLE_EXPR,(int)var));
					break;
					}
				case VALUE:
					{
					signed short value;
					value=*((signed short*)s_parser.yylvalptr);
					if(base)
						base=new CTreeNode(STMT_LIST,base,new CTreeNode(VALUE_EXPR,(int)value));
					else
						base=new CTreeNode(STMT_LIST,new CTreeNode(VALUE_EXPR,(int)value));
					break;
					}
				default:
					printf("UNEXPECTED TOKEN '%s' FOR ARGUMENT %d\n",s_lexer.yytext,i+1);
					s_lexer.error();
					break;
			}
			if(i<argCount-1)
			{
				if(s_parser.yygettoken()!=COMMA)
				{
					printf("EXPETCING ',' BUT FOUND '%s'\n",s_lexer.yytext);
					s_lexer.error();
					break;

				}
			}
		}

		return base;
	}

	return new CTreeNode(EMPTY_STMT);
}


/*===========================================================================
 end */
