/************************************************************
yymcwipe.c
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include <string.h>
#include "myacc.h"

#ifdef YYPROTOTYPE
void YYCDECL yymcwipe(yymparse_t YYFAR *yy)
#else
void YYCDECL yymcwipe(yy)
yymparse_t YYFAR *yy;
#endif
{
	yyassert(yy != NULL);
	yyassert(yymiscompactparser(yy));	/* make sure it's a compact parser */

	yymdestructpop(yy, yy->yymtop + 1);
	yymcdestructclearin(yy);
}
