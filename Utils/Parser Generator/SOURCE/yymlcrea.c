/************************************************************
yymlcrea.c
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include "mlex.h"
#include <stdlib.h>

#ifdef YYPROTOTYPE
int YYCDECL yymcreatelex(yymlex_t YYFAR *yy, YYCONST yymlex_t YYFAR *src)
#else
int YYCDECL yymcreatelex(yy, src)
yymlex_t YYFAR *yy;
YYCONST yymlex_t YYFAR *src;
#endif
{
	size_t textcharsize;
	size_t statebufcharsize;
	size_t unputbufcharsize;
	yyassert(yy != NULL);
	yyassert(src != NULL);

	*yy = *src;

	/* get sizes first */
	textcharsize = yy->yymstext_size + 1;	/* include the '\0' */
	if (textcharsize <= (size_t) yy->yymstext_size) {
		return 0;		/* integer overflow */
	}
	if (yy->yymstext_size != 0) {
		statebufcharsize = yy->yymstext_size * sizeof(int);
		if ((int) (statebufcharsize / sizeof(int)) != yy->yymstext_size) {
			return 0;		/* integer overflow */
		}
	}
	else {
		statebufcharsize = 0;
	}
	if (yy->yymsunput_size != 0) {
		unputbufcharsize = yy->yymsunput_size * sizeof(int);
		if ((int) (unputbufcharsize / sizeof(int)) != yy->yymsunput_size) {
			return 0;		/* integer overflow */
		}
	}
	else {
		unputbufcharsize = 0;
	}

	/* allocate the memory if necessary */
	yy->yymstext = (char YYFAR *) malloc(textcharsize);
	if (yy->yymstext == NULL) {
		return 0;
	}
	if (statebufcharsize != 0) {
		yy->yymsstatebuf = (int YYFAR *) malloc(statebufcharsize);
		if (yy->yymsstatebuf == NULL) {
			free(yy->yymstext);
			return 0;
		}
	}
	else {
		yy->yymsstatebuf = NULL;
	}
	if (unputbufcharsize != 0) {
		yy->yymsunputbufptr = (int YYFAR *) malloc(unputbufcharsize);
		if (yy->yymsunputbufptr == NULL) {
			free(yy->yymstext);
			free(yy->yymsstatebuf);
			return 0;
		}
	}
	else {
		yy->yymsunputbufptr = NULL;
	}

	/* assign any other variables */
	yy->yymtext_size = yy->yymstext_size;
	yy->yymtext = yy->yymstext;
	yy->yymstatebuf = yy->yymsstatebuf;
	yy->yymunput_size = yy->yymsunput_size;
	yy->yymunputbufptr = yy->yymsunputbufptr;

	/* make sure we are ready to go */
	yymreset(yy);

	return 1;
}
