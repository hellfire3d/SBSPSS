/************************************************************
yymldbug.c
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include <stdio.h>
#ifdef _WIN32
#include <windows.h>
#include <winbase.h>
#endif
#include "mlex.h"

#ifdef YYDEBUG

#ifdef YYPROTOTYPE
void YYCDECL yymdmatch(yymlex_t YYFAR *yy, int expr)
#else
void YYCDECL yymdmatch(yy, expr)
yymlex_t YYFAR *yy;
int expr;
#endif
{
	yyassert(yy != NULL);
	if (yydebug || yy->yymdebug) {
		char string[128];
		int i;

		sprintf(string, "%p: match: \"", (void *) yy);
		yymlexdebugoutput(yy, string);
		for (i = 0; i < yy->yymleng; i++) {
			yymdebugoutput(yy, yy->yymtext[i]);
		}
		sprintf(string, "\", %d\n", (int) expr);
		yymlexdebugoutput(yy, string);
	}
}

#ifdef YYPROTOTYPE
void YYCDECL yymlexdebugoutput(yymlex_t YYFAR *yy, YYCONST char *string)
#else
void YYCDECL yymlexdebugoutput(yy, string)
yymlex_t YYFAR *yy;
YYCONST char *string;
#endif
{
	yyassert(yy != NULL);
	yyassert(string != NULL);

#ifdef _WIN32
	if (yy->yymdebugout != NULL) {
#else
		yyassert(yy->yymdebugout != NULL);
#endif
		while (*string != '\0') {
			putc(*string++, yy->yymdebugout);
		}

		if (yydebugflush || yy->yymdebugflush) {
			fflush(yy->yymdebugout);
		}
#ifdef _WIN32
	}
	else {
		OutputDebugString(string);
	}
#endif
}

#endif
