/************************************************************
yyssetup.c
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include "yacc.h"

#ifdef YYPROTOTYPE
int YYCDECL yysetup(void)
#else
int YYCDECL yysetup()
#endif
{
	/* initialise variables */
	yytop = -1;
	yylookahead = 0;
	yyskip = 0;
	yyerrorcount = 0;
	yychar = -1;
	yypopflg = 0;

	/* push initial state onto the stack */
	if (!yypush(0)) {
#ifdef YYDEBUG
		yydabort();
#endif
		return 1;
	}
	return 0;
}
