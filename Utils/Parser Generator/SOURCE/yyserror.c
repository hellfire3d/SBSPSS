/************************************************************
yyserror.c
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include <stdio.h>
#include "yacc.h"

#ifdef YYPROTOTYPE
void YYCDECL yyerror(YYCONST char YYFAR *text)
#else
void YYCDECL yyerror(text)
YYCONST char YYFAR *text;
#endif
{
	yyassert(text != NULL);
	yyassert(yyparseerr != NULL);
	while (*text != '\0') {
		putc(*text++, yyparseerr);
	}
	putc('\n', yyparseerr);
}
