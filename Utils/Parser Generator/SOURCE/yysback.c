/************************************************************
yysback.c
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include "lex.h"

#ifdef YYPROTOTYPE
int YYCDECL yyback(YYCONST yymatch_t YYNEARFAR *p, int action)
#else
int YYCDECL yyback(p, action)
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
