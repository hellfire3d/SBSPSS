/************************************************************
yyssetin.c
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include "yacc.h"

#ifdef YYPROTOTYPE
void YYCDECL yysetin(int token)
#else
void YYCDECL yysetin(token)
int token;
#endif
{
	if (token < 0) {
		token = 0;
	}
	yychar = token;
	yylookahead = 1;
}
