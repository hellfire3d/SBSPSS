/************************************************************
yysstsiz.c
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include <stdlib.h>
#include <string.h>
#include "lex.h"

#ifdef YYPROTOTYPE
int YYCDECL yysettextsize(int size)
#else
int YYCDECL yysettextsize(size)
int size;
#endif
{
	yyassert(size >= 0);
	if (yytext_size != size) {
		char YYFAR *text;
		int YYFAR *statebuf;

		size_t textcharsize;
		size_t statebufcharsize;

		if (size < yyleng) {
			return 0;
		}

		textcharsize = size + 1;	/* include the '\0' */
		if (textcharsize <= (size_t) size) {
			return 0;		/* integer overflow */
		}
		statebufcharsize = size * sizeof(int);
		if ((int) (statebufcharsize / sizeof(int)) != size) {
			return 0;		/* integer overflow */
		}

		/* allocate */
		if (size <= yystext_size) {
			text = yystext;
			statebuf = yysstatebuf;
		}
		else {
			text = (char YYFAR *) malloc(textcharsize);
			if (text == NULL) {
				return 0;
			}
			statebuf = (int YYFAR *) malloc(statebufcharsize);
			if (statebuf == NULL) {
				free(text);			/* clean up */
				return 0;
			}
		}

		/* copy */
		if (text != yytext) {
			size_t charsize = yytext_size + 1;
			memcpy(text, yytext, min(textcharsize, charsize));
		}
		if (statebuf != yystatebuf) {
			size_t charsize = yytext_size * sizeof(int);
			memcpy(statebuf, yystatebuf, min(statebufcharsize, charsize));
		}

		/* free */
		if (yytext != yystext) {
			free(yytext);
		}
		if (yystatebuf != yysstatebuf) {
			free(yystatebuf);
		}

		/* assign */
		yytext = text;
		yystatebuf = statebuf;
		yytext_size = size;
	}
	return 1;
}
