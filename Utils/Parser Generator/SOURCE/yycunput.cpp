/************************************************************
yycunput.cpp
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <limits.h>
#include "clex.h"

void yylexer::yyunput(int ch)
{
	yyassert(ch >= 0 && ch <= UCHAR_MAX || ch == EOF);
	yyassert(yyunputindex >= 0 && yyunputindex <= yyunput_size);
	
	// check unput buffer size
	if (yyunputindex == yyunput_size) {
		do {
			if (yyunputgrow) {
				if (yyunput_size != 0) {
					int size = yyunput_size * 2;
					if (size / 2 == yyunput_size) {		// overflow check
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

	// check line number
	if (ch == '\n') {
		yylineno--;
	}

	// debugging
#ifdef YYDEBUG
	if (::yydebug || yydebug) {
		char string[128];

		sprintf(string, "%p: unput: \'", (void*)this);
		yydebugoutput(string);
		yydebugoutput(ch);
		yydebugoutput("\'\n");
	}
#endif
}
