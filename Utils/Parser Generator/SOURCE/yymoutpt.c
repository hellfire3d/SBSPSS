/************************************************************
yymoutpt.c
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include <stdio.h>
#include <limits.h>
#include "mlex.h"

#ifdef YYPROTOTYPE
void YYCDECL yymoutput(yymlex_t YYFAR *yy, int ch)
#else
void YYCDECL yymoutput(yy, ch)
yymlex_t YYFAR *yy;
int ch;
#endif
{
	yyassert(yy != NULL);

	/* debugging */
#ifdef YYDEBUG
	if (yydebug || yy->yymdebug) {
		char string[128];

		sprintf(string, "%p: output: \'", (void *) yy);
		yymlexdebugoutput(yy, string);
		yymdebugoutput(yy, ch);
		yymlexdebugoutput(yy, "\'\n");
	}
#endif

	yyassert(yy->yymout != NULL);

#ifdef __BORLANDC__
	putc((char) ch, yy->yymout);
#else
	putc(ch, yy->yymout);
#endif
}
