/************************************************************
yyminput.c
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include <stdio.h>
#include <ctype.h>
#include "mlex.h"

#ifdef YYPROTOTYPE
int YYCDECL yyminput(yymlex_t YYFAR *yy)
#else
int YYCDECL yyminput(yy)
yymlex_t YYFAR *yy;
#endif
{
	int ch;
	yyassert(yy != NULL);

	if (yy->yymunputindex > 0) {
		ch = yy->yymunputbufptr[--yy->yymunputindex];
	}
	else {
		ch = yy->yymgetchar(yy);
	}
	if (ch == '\n') {
		yy->yymlineno++;
	}
	
	/* debugging */
#ifdef YYDEBUG
	if (yydebug || yy->yymdebug) {
		char string[128];

		sprintf(string, "%p: input: \'", (void *) yy);
		yymlexdebugoutput(yy, string);
		yymdebugoutput(yy, ch);
		yymlexdebugoutput(yy, "\'\n");
	}
#endif
	
	return ch;
}
