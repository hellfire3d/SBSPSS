/************************************************************
yycldbug.cpp
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include <stdio.h>
#ifdef _WIN32
#include <windows.h>
#include <winbase.h>
#endif
#include "clex.h"

#ifdef YYDEBUG

void yylexer::yydmatch(int expr) const
{
	if (::yydebug || yydebug) {
		char string[128];

		sprintf(string, "%p: match: \"", (void*)this);
		yydebugoutput(string);
		for (int i = 0; i < yyleng; i++) {
			yydebugoutput(yytext[i]);
		}
		sprintf(string, "\", %d\n", (int)expr);
		yydebugoutput(string);
	}
}

void yylexer::yydebugoutput(const char *string) const
{
	yyassert(string != NULL);

#ifdef _WIN32
	if (yydebugout != NULL) {
#else
		yyassert(yydebugout != NULL);
#endif
		while (*string != '\0') {
			putc(*string++, yydebugout);
		}

		if (::yydebugflush || yydebugflush) {
			fflush(yydebugout);
		}
#ifdef _WIN32
	}
	else {
		OutputDebugString(string);
	}
#endif
}

#endif
