/************************************************************
yymwrap.c
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include "mlex.h"

#ifdef __BORLANDC__
#pragma argsused
#endif

#ifdef YYPROTOTYPE
int YYCDECL yymwrap(yymlex_t YYFAR *yy)
#else
int YYCDECL yymwrap(yy)
yymlex_t YYFAR *yy;
#endif
{
	yyassert(yy != NULL);
	return 1;
}
