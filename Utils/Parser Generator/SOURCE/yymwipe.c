/************************************************************
yymwipe.c
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include <string.h>
#include "myacc.h"

#ifdef YYPROTOTYPE
void YYCDECL yymwipe(yymparse_t YYFAR *yy)
#else
void YYCDECL yymwipe(yy)
yymparse_t YYFAR *yy;
#endif
{
	yyassert(yy != NULL);
	yyassert(yymisfastparser(yy));	/* make sure it's a fast parser */

	yymdestructpop(yy, yy->yymtop + 1);
	yymdestructclearin(yy);
}
