/************************************************************
yysunput.c
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <limits.h>
#include "lex.h"

#ifdef YYPROTOTYPE
void YYCDECL yyunput(int ch)
#else
void YYCDECL yyunput(ch)
int ch;
#endif
{
	yyassert(ch >= 0 && ch <= UCHAR_MAX || ch == EOF);
	yyassert(yyunputindex >= 0 && yyunputindex <= yyunput_size);
	
	/* check unput buffer size */
	if (yyunputindex == yyunput_size) {
		do {
			if (yyunputgrow) {
				if (yyunput_size != 0) {
					int size = yyunput_size * 2;
					if (size / 2 == yyunput_size) {		/* overflow check */
						if (yysetunputsize(size)) {
							break;
						}
					}
				}
				else {
					if (yysetunputsize(100)) {
						break;
					}
				}
			}
			yyunputoverflow();
			exit(EXIT_FAILURE);
		}
		while (0);
	}

	yyunputbufptr[yyunputindex++] = ch;

	/* check line number */
	if (ch == '\n') {
		yylineno--;
	}

	/* debugging */
#ifdef YYDEBUG
	if (yydebug || yylexdebug) {
		yylexdebugoutput("unput: \'");
		yydebugoutput(ch);
		yylexdebugoutput("\'\n");
	}
#endif
}
