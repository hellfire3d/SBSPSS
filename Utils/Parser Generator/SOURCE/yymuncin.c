/************************************************************
yymuncin.c
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include "myacc.h"

#ifdef YYPROTOTYPE
int YYCDECL yymunclearin(yymparse_t YYFAR *yy)
#else
int YYCDECL yymunclearin(yy)
yymparse_t YYFAR *yy;
#endif
{
	yyassert(yy != NULL);
	if (!yy->yymlookahead && yy->yymchar != -1) {
		yy->yymlookahead = 1;
		return 1;
	}
	return 0;
}
