/************************************************************
yymlcln.c
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include <stdlib.h>
#include <malloc.h>
#include "mlex.h"

#ifdef YYPROTOTYPE
void YYCDECL yymlexcleanup(yymlex_t YYFAR *yy)
#else
void YYCDECL yymlexcleanup(yy)
yymlex_t YYFAR *yy;
#endif
{
	yyassert(yy != NULL);
	if (yy->yymtext != yy->yymstext) {
		free(yy->yymtext);
		yy->yymtext = yy->yymstext;
	}
	if (yy->yymstatebuf != yy->yymsstatebuf) {
		free(yy->yymstatebuf);
		yy->yymstatebuf = yy->yymsstatebuf;
	}
	if (yy->yymunputbufptr != yy->yymsunputbufptr) {
		free(yy->yymunputbufptr);
		yy->yymunputbufptr = yy->yymsunputbufptr;
	}
	yy->yymtext_size = yy->yymstext_size;
	yy->yymunput_size = yy->yymsunput_size;

	if (yy->yymtext != NULL) {
		*yy->yymtext = '\0';
	}
	yy->yymleng = 0;
	yy->yymunputindex = 0;
}
