/************************************************************
yymsskip.c
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include <stdio.h>
#include "myacc.h"

#ifdef YYDEBUG
#ifdef YYPROTOTYPE
void YYCDECL yymsetskip(yymparse_t YYFAR *yy, int skip)
#else
void YYCDECL yymsetskip(yy, skip)
yymparse_t YYFAR *yy;
int skip;
#endif
{
	yyassert(yy != NULL);
	yyassert(skip >= 0);

	/* debugging */
	if (yydebug || yy->yymdebug) {
		if (skip > 0) {
			if (yy->yymskip == 0) {
				char string[128];

				sprintf(string, "%p: entering error recovery\n", (void *) yy);
				yymparsedebugoutput(yy, string);
			}
		}
		else {
			if (yy->yymskip > 0) {
				char string[128];

				sprintf(string, "%p: leaving error recovery\n", (void *) yy);
				yymparsedebugoutput(yy, string);
			}
		}
	}

	yy->yymskip = skip;
}
#endif
