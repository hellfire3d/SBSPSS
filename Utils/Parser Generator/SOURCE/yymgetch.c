/************************************************************
yymgetch.c
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include <stdio.h>
#include <assert.h>
#include "mlex.h"

#ifdef YYPROTOTYPE
int YYCDECL yymgetchar(yymlex_t YYFAR *yy)
#else
int YYCDECL yymgetchar(yy)
yymlex_t YYFAR *yy;
#endif
{
	yyassert(yy != NULL);
	yyassert(yy->yymin != NULL);
	return getc(yy->yymin);
}
