/************************************************************
yymgtok.c
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include "myacc.h"
#include "mlex.h"

#ifdef YYPROTOTYPE
int YYCDECL yymgettoken(yymparse_t YYFAR *yy)
#else
int YYCDECL yymgettoken(yy)
yymparse_t YYFAR *yy;
#endif
{
	yymlex_t YYFAR *p;
	yyassert(yy != NULL);

	p = (yymlex_t YYFAR *) yy->yymdata;
	yyassert(p != NULL);
	return yymlex(p);
}
