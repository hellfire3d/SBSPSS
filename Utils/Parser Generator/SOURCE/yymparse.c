/************************************************************
yymparse.c
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include <string.h>
#include "myacc.h"

#ifdef YYPROTOTYPE
int YYCDECL yymparse(yymparse_t YYFAR *yy)
#else
int YYCDECL yymparse(yy)
yymparse_t YYFAR *yy;
#endif
{
	int n;
	yyassert(yy != NULL);
	yyassert(yymisfastparser(yy));	/* make sure it's a fast parser */

	n = yymsetup(yy);
	if (n != 0) {
		return n;
	}
	return yymwork(yy);
}
