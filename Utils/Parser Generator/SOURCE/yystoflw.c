/************************************************************
yystoflw.c
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include "lex.h"

#ifdef YYPROTOTYPE
void YYCDECL yytextoverflow(void)
#else
void YYCDECL yytextoverflow()
#endif
{
	yyassert(yylexerr != NULL);
	fprintf(yylexerr, "lex text buffer overflow (%d)\n", (int) yytext_size);
}
