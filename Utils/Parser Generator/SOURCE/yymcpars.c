/************************************************************
yymcpars.c
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include "myacc.h"

#ifdef YYPROTOTYPE
int YYCDECL yymcparse(yymparse_t YYFAR *yy)
#else
int YYCDECL yymcparse(yy)
yymparse_t YYFAR *yy;
#endif
{
	int n;
	yyassert(yy != NULL);
	yyassert(yymiscompactparser(yy));	/* make sure it's a compact parser */

	n = yymsetup(yy);
	if (n != 0) {
		return n;
	}
	return yymcwork(yy);
}
