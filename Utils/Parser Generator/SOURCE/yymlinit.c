/************************************************************
yymlinit.c
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include "mlex.h"

#ifdef YYPROTOTYPE
void YYCDECL yymlexinit(yymlex_t YYFAR *yy)
#else
void YYCDECL yymlexinit(yy)
yymlex_t YYFAR *yy;
#endif
{
	yyassert(yy != NULL);

	yy->yymin = stdin;
	yy->yymout = stdout;
	yy->yymerr = stderr;
#ifdef YYDEBUG
	yy->yymdebugout = stdout;
#endif
}
