/************************************************************
yycsetup.cpp
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include "cyacc.h"

int yyparser::yysetup()
{
	// initialise variables
	yytop = -1;
	yylookahead = 0;
	yyskip = 0;
	yyerrorcount = 0;
	yychar = -1;
	yypopflg = 0;

	// push initial state onto the stack
	if (!yypush(0)) {
#ifdef YYDEBUG
		yydabort();
#endif
		return 1;
	}
	return 0;
}
