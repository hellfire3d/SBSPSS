/************************************************************
yycinput.cpp
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include <stdio.h>
#include <ctype.h>
#include "clex.h"

int yylexer::yyinput()
{
	int ch;
	if (yyunputindex > 0) {
		ch = yyunputbufptr[--yyunputindex];
	}
	else {
		ch = yygetchar();
	}
	if (ch == '\n') {
		yylineno++;
	}

#ifdef YYDEBUG
	if (::yydebug || yydebug) {
		char string[128];

		sprintf(string, "%p: input: \'", (void*)this);
		yydebugoutput(string);
		yydebugoutput(ch);
		yydebugoutput("\'\n");
	}
#endif

	return ch;
}
