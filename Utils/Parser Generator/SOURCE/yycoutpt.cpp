/************************************************************
yycoutpt.cpp
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include <stdio.h>
#include <limits.h>
#include "clex.h"

void yylexer::yyoutput(int ch)
{
	// debugging
#ifdef YYDEBUG
	if (::yydebug || yydebug) {
		char string[128];

		sprintf(string, "%p: output: \'", (void*)this);
		yydebugoutput(string);
		yydebugoutput(ch);
		yydebugoutput("\'\n");
	}
#endif

	yyassert(yyout != NULL);

#ifdef __BORLANDC__
	putc((char)ch, yyout);
#else
	putc(ch, yyout);
#endif
}
