/************************************************************
yysecho.c
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include <stdio.h>
#include "lex.h"

#ifdef YYPROTOTYPE
void YYCDECL yyecho(void)
#else
void YYCDECL yyecho()
#endif
{
	int i;
	for (i = 0; i < yyleng; i++) {
		yyoutput(yytext[i]);
	}
}
