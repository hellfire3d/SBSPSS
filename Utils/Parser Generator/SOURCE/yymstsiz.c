/************************************************************
yymstsiz.c
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include <stdlib.h>
#include <string.h>
#include "mlex.h"

#ifdef YYPROTOTYPE
int YYCDECL yymsettextsize(yymlex_t YYFAR *yy, int size)
#else
int YYCDECL yymsettextsize(yy, size)
yymlex_t YYFAR *yy;
int size;
#endif
{
	yyassert(yy != NULL);
	yyassert(size >= 0);
	if (yy->yymtext_size != size) {
		char YYFAR *text;
		int YYFAR *statebuf;

		size_t textcharsize;
		size_t statebufcharsize;

		if (size < yy->yymleng) {
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
		if (size <= yy->yymstext_size) {
			text = yy->yymstext;
			statebuf = yy->yymsstatebuf;
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
		if (text != yy->yymtext) {
			size_t charsize = yy->yymtext_size + 1;
			memcpy(text, yy->yymtext, min(textcharsize, charsize));
		}
		if (statebuf != yy->yymstatebuf) {
			size_t charsize = yy->yymtext_size * sizeof(int);
			memcpy(statebuf, yy->yymstatebuf, min(statebufcharsize, charsize));
		}

		/* free */
		if (yy->yymtext != yy->yymstext) {
			free(yy->yymtext);
		}
		if (yy->yymstatebuf != yy->yymsstatebuf) {
			free(yy->yymstatebuf);
		}

		/* assign */
		yy->yymtext = text;
		yy->yymstatebuf = statebuf;
		yy->yymtext_size = size;
	}
	return 1;
}
