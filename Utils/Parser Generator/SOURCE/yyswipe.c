/************************************************************
yyswipe.c
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include <string.h>
#include "yacc.h"

#ifdef YYPROTOTYPE
void YYCDECL yywipe(void)
#else
void YYCDECL yywipe()
#endif
{
	yydestructpop(yytop + 1);
	yydestructclearin();
}
