/************************************************************
yysoutpt.c
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include <stdio.h>
#include <limits.h>
#include "lex.h"

#ifdef YYPROTOTYPE
void YYCDECL yyoutput(int ch)
#else
void YYCDECL yyoutput(ch)
int ch;
#endif
{
	/* debugging */
#ifdef YYDEBUG
	if (yydebug || yylexdebug) {
		yylexdebugoutput("output: \'");
		yydebugoutput(ch);
		yylexdebugoutput("\'\n");
	}
#endif

	yyassert(yyout != NULL);

#ifdef __BORLANDC__
	putc((char) ch, yyout);
#else
	putc(ch, yyout);
#endif
}
