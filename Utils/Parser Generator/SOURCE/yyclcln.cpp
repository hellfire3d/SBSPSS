/************************************************************
yyclcln.cpp
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include <stdlib.h>
#include <malloc.h>
#include "clex.h"

void yylexer::yycleanup()
{
	if (yytext != yystext) {
		free(yytext);
		yytext = yystext;
	}
	if (yystatebuf != yysstatebuf) {
		free(yystatebuf);
		yystatebuf = yysstatebuf;
	}
	if (yyunputbufptr != yysunputbufptr) {
		free(yyunputbufptr);
		yyunputbufptr = yysunputbufptr;
	}
	yytext_size = yystext_size;
	yyunput_size = yysunput_size;

	if (yytext != NULL) {
		*yytext = '\0';
	}
	yyleng = 0;
	yyunputindex = 0;
}
