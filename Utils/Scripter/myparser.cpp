/*=========================================================================

	myparser.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/


/*----------------------------------------------------------------------
	Includes
	-------- */

#ifndef _PARSER_H
#include "parser.h"
#endif

#ifndef _LEXER_H
#include "lexer.h"
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
void myparser::setCurrentLexer(mylexer *_lexer)
{
	m_currentLexer=_lexer;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void myparser::setBaseNode(class CTreeNode *_baseNode)
{
	m_baseNode=_baseNode;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void myparser::yyerror(const char *_text)
{
	fprintf(yyparseerr,"PARSER ERROR: %s\n",_text);
//	fprintf(yyparseerr,"       line %d char %d\n",m_currentLexer->getCurrentLine(),m_currentLexer->getCurrentCharOnLine());
	m_currentLexer->error();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int myparser::yyparse()
{
	int	ret=YYEXIT_FAILURE;

	if(yysetup()==0&&
	   yywork()==YYEXIT_SUCCESS&&
	   m_currentLexer->getErrorCount()==0)
	{
		ret=YYEXIT_SUCCESS;
	}

	return ret;
}


/*===========================================================================
 end */
