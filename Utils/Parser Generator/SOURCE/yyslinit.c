/************************************************************
yyslinit.c
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include "lex.h"

#ifdef YYPROTOTYPE
void YYCDECL yylexinit(void)
#else
void YYCDECL yylexinit()
#endif
{
	yyin = stdin;
	yyout = stdout;
	yylexerr = stderr;
#ifdef YYDEBUG
	yylexdebugout = stdout;
#endif
}
