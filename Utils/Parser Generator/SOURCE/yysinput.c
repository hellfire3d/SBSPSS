/************************************************************
yysinput.c
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include <stdio.h>
#include <ctype.h>
#include "lex.h"

#ifdef YYPROTOTYPE
int YYCDECL yyinput(void)
#else
int YYCDECL yyinput()
#endif
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
	if (yydebug || yylexdebug) {
		yylexdebugoutput("input: \'");
		yydebugoutput(ch);
		yylexdebugoutput("\'\n");
	}
#endif

	return ch;
}
