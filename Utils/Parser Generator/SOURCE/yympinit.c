/************************************************************
yympinit.c
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include "myacc.h"

#ifdef YYPROTOTYPE
void YYCDECL yymparseinit(yymparse_t YYFAR *yy)
#else
void YYCDECL yymparseinit(yy)
yymparse_t YYFAR *yy;
#endif
{
	yyassert(yy != NULL);

	yy->yymerr = stderr;
#ifdef YYDEBUG
	yy->yymdebugout = stdout;
#endif
}
