/************************************************************
yycerror.cpp
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include <stdio.h>
#include "cyacc.h"

void yyparser::yyerror(const char YYFAR* text)
{
	yyassert(text != NULL);
	yyassert(yyerr != NULL);
	while (*text != '\0') {
		putc(*text++, yyerr);
	}
	putc('\n', yyerr);
}
