/************************************************************
yymdisc.c
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include "myacc.h"

#ifdef __BORLANDC__
#pragma argsused
#endif

#ifdef YYPROTOTYPE
void YYCDECL yymdiscard(yymparse_t YYFAR *yy, int token)
#else
void YYCDECL yymdiscard(yy, token)
yymparse_t YYFAR *yy;
int token;
#endif
{
	yyassert(yy != NULL);
	yyassert(token > 0);
	/* do nothing */
}
