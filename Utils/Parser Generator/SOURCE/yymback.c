/************************************************************
yymback.c
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include "mlex.h"

#ifdef YYPROTOTYPE
int YYCDECL yymback(YYCONST yymatch_t YYNEARFAR *p, int action)
#else
int YYCDECL yymback(p, action)
YYCONST yymatch_t YYNEARFAR *p;
int action;
#endif
{
	yyassert(p != NULL);
	yyassert(action < 0);
	while (*p != 0) {
		if (*p++ == action) {
			return 1;
		}
	}
	return 0;
}
