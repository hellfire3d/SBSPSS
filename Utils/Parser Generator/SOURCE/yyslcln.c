/************************************************************
yyslcln.c
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include <stdlib.h>
#include <malloc.h>
#include "lex.h"

#ifdef YYPROTOTYPE
void YYCDECL yylexcleanup(void)
#else
void YYCDECL yylexcleanup()
#endif
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
