/************************************************************
yyclcrea.cpp
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include "clex.h"
#include <stdlib.h>

int yylexer::yycreate(yyparser YYFAR* parserptr)
{
	yyparserptr = parserptr;

	size_t textcharsize;
	size_t statebufcharsize;
	size_t unputbufcharsize;

	// get sizes first
	textcharsize = yystext_size + 1;	// include the '\0'
	if (textcharsize <= (size_t)yystext_size) {
		return 0;		// integer overflow
	}
	if (yystext_size != 0) {
		statebufcharsize = yystext_size * sizeof(int);
		if ((int)(statebufcharsize / sizeof(int)) != yystext_size) {
			return 0;		// integer overflow
		}
	}
	else {
		statebufcharsize = 0;
	}
	if (yysunput_size != 0) {
		unputbufcharsize = yysunput_size * sizeof(int);
		if ((int)(unputbufcharsize / sizeof(int)) != yysunput_size) {
			return 0;		// integer overflow
		}
	}
	else {
		unputbufcharsize = 0;
	}

	// allocate the memory if necessary
	yystext = (char YYFAR*)malloc(textcharsize);
	if (yystext == NULL) {
		return 0;
	}
	if (statebufcharsize != 0) {
		yysstatebuf = (int YYFAR*)malloc(statebufcharsize);
		if (yysstatebuf == NULL) {
			free(yystext);
			return 0;
		}
	}
	else {
		yysstatebuf = NULL;
	}
	if (unputbufcharsize != 0) {
		yysunputbufptr = (int YYFAR*)malloc(unputbufcharsize);
		if (yysunputbufptr == NULL) {
			free(yystext);
			free(yysstatebuf);
			return 0;
		}
	}
	else {
		yysunputbufptr = NULL;
	}

	// assign any other variables
	yytext_size = yystext_size;
	yytext = yystext;
	yystatebuf = yysstatebuf;
	yyunput_size = yysunput_size;
	yyunputbufptr = yysunputbufptr;

	// makes sure we are ready to go
	yyreset();

	return 1;
}
