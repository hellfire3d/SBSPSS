/************************************************************
yysldbug.c
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include <stdio.h>
#ifdef _WIN32
#include <windows.h>
#include <winbase.h>
#endif
#include "lex.h"

#ifdef YYDEBUG

int YYNEAR YYDCDECL yylexdebug = 0;
int YYNEAR YYDCDECL yylexdebugflush = 0;
#ifndef YYNINITSTREAM
FILE YYFAR *YYNEAR YYDCDECL yylexdebugout = stdout;
#else
FILE YYFAR *YYNEAR YYDCDECL yylexdebugout = NULL;
#endif

#ifdef YYPROTOTYPE
void YYCDECL yydmatch(int expr)
#else
void YYCDECL yydmatch(expr)
int expr;
#endif
{
	if (yydebug || yylexdebug) {
		char string[128];
		int i;

		yylexdebugoutput("match: \"");
		for (i = 0; i < yyleng; i++) {
			yydebugoutput(yytext[i]);
		}
		sprintf(string, "\", %d\n", (int) expr);
		yylexdebugoutput(string);
	}
}

#ifdef YYPROTOTYPE
void YYCDECL yylexdebugoutput(YYCONST char *string)
#else
void YYCDECL yylexdebugoutput(string)
YYCONST char *string;
#endif
{
	yyassert(string != NULL);

#ifdef _WIN32
	if (yylexdebugout != NULL) {
#else
		yyassert(yylexdebugout != NULL);
#endif
		while (*string != '\0') {
			putc(*string++, yylexdebugout);
		}

		if (yydebugflush || yylexdebugflush) {
			fflush(yylexdebugout);
		}
#ifdef _WIN32
	}
	else {
		OutputDebugString(string);
	}
#endif
}

#endif
