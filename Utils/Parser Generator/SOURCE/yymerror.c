/************************************************************
yymerror.c
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include <stdio.h>
#include "myacc.h"

#ifdef YYPROTOTYPE
void YYCDECL yymerror(yymparse_t YYFAR *yy, YYCONST char YYFAR *text)
#else
void YYCDECL yymerror(yy, text)
yymparse_t YYFAR *yy;
YYCONST char YYFAR *text;
#endif
{
	yyassert(yy != NULL);
	yyassert(text != NULL);
	yyassert(yy->yymerr != NULL);
	while (*text != '\0') {
		putc(*text++, yy->yymerr);
	}
	putc('\n', yy->yymerr);
}
