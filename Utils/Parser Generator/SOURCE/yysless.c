/************************************************************
yysless.c
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include "lex.h"

#ifdef YYPROTOTYPE
void YYCDECL yyless(int length)
#else
void YYCDECL yyless(length)
int length;
#endif
{
	yyassert(length >= 0 && length <= yyleng);
	while (yyleng > length) {
		yyunput((unsigned char) yytext[--yyleng]);
	}
	if (yyleng > 0) {
		yyeol = (unsigned char) (yytext[yyleng - 1] == '\n');
	}
	else {
		yyeol = yyoldeol;
	}
}
