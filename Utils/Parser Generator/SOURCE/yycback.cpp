/************************************************************
yycback.cpp
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include "clex.h"

int yylexer::yyback(const yymatch_t YYNEARFAR* p, int action) const
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
