/************************************************************
yycsusiz.cpp
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include <stdlib.h>
#include <string.h>
#include "clex.h"

#ifndef min
#define min(x, y) ((x) <= (y) ? (x) : (y))
#endif

// the Visual C++ v1.52 compiler generates an error in NT if this isn't present!
#ifdef _MSC_VER
#if defined(M_I86HM) && defined(NDEBUG)
#pragma function(memcpy)
#endif
#endif

int yylexer::yysetunputsize(int size)
{
	yyassert(size >= 0);
	if (yyunput_size != size) {
		if (size < yyunputindex) {
			return 0;
		}

		size_t unputbufcharsize = size * sizeof(int);
		if ((int)(unputbufcharsize / sizeof(int)) != size) {
			return 0;		// integer overflow
		}

		// allocate
		int YYFAR* unputbufptr;
		if (size <= yysunput_size) {
			unputbufptr = yysunputbufptr;
		}
		else {
			unputbufptr = (int YYFAR*)malloc(unputbufcharsize);
			if (unputbufptr == NULL) {
				return 0;
			}
		}

		// copy
		if (unputbufptr != yyunputbufptr) {
			size_t charsize = yyunput_size * sizeof(int);
			memcpy(unputbufptr, yyunputbufptr, min(unputbufcharsize, charsize));
		}

		// free
		if (yyunputbufptr != yysunputbufptr) {
			free(yyunputbufptr);
		}

		// assign
		yyunputbufptr = unputbufptr;
		yyunput_size = size;
	}
	return 1;
}
