/************************************************************
yymunput.c
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <limits.h>
#include "mlex.h"

#ifdef YYPROTOTYPE
void YYCDECL yymunput(yymlex_t YYFAR *yy, int ch)
#else
void YYCDECL yymunput(yy, ch)
yymlex_t YYFAR *yy;
int ch;
#endif
{
	yyassert(yy != NULL);
	yyassert(ch >= 0 && ch <= UCHAR_MAX || ch == EOF);
	yyassert(yy->yymunputindex >= 0 && yy->yymunputindex <= yy->yymunput_size);
	
	/* check unput buffer size */
	if (yy->yymunputindex == yy->yymunput_size) {
		do {
			if (yy->yymunputgrow) {
				if (yy->yymunput_size != 0) {
					int size = yy->yymunput_size * 2;
					if (size / 2 == yy->yymunput_size) {		/* overflow check */
						if (yymsetunputsize(yy, size)) {
							break;
						}
					}
				}
				else {
					if (yymsetunputsize(yy, 100)) {
						break;
					}
				}
			}
			(*yy->yymunputoverflow)(yy);
			exit(EXIT_FAILURE);
		}
		while (0);
	}

	yy->yymunputbufptr[yy->yymunputindex++] = ch;

	/* check line number */
	if (ch == '\n') {
		yy->yymlineno--;
	}

	/* debugging */
#ifdef YYDEBUG
	if (yydebug || yy->yymdebug) {
		char string[128];

		sprintf(string, "%p: unput: \'", (void *) yy);
		yymlexdebugoutput(yy, string);
		yymdebugoutput(yy, ch);
		yymlexdebugoutput(yy, "\'\n");
	}
#endif
}
