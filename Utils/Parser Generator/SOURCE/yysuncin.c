/************************************************************
yysuncin.c
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include "yacc.h"

#ifdef YYPROTOTYPE
int YYCDECL yyunclearin(void)
#else
int YYCDECL yyunclearin()
#endif
{
	if (!yylookahead && yychar != -1) {
		yylookahead = 1;
		return 1;
	}
	return 0;
}
