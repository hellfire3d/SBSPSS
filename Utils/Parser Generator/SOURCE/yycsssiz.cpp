/************************************************************
yycsssiz.cpp
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include <stdlib.h>
#include <string.h>
#include "cyacc.h"

#ifndef min
#define min(x, y) ((x) <= (y) ? (x) : (y))
#endif

// the Visual C++ v1.52 compiler generates an error in NT if this isn't present!
#ifdef _MSC_VER
#if defined(M_I86HM) && defined(NDEBUG)
#pragma function(memcpy)
#endif
#endif

int yyparser::yysetstacksize(int size)
{
	yyassert(size >= 0);
	if (yystack_size != size) {
		if (size <= yytop) {
			return 0;
		}

		size_t stackcharsize = size * sizeof(yystack_t);
		if ((int)(stackcharsize / sizeof(yystack_t)) != size) {
			return 0;		// integer overflow
		}
		size_t attributestackcharsize = size * yyattribute_size;
		if ((int)(attributestackcharsize / yyattribute_size) != size) {
			return 0;		// integer overflow
		}

		// allocate
		yystack_t YYFAR* stackptr;
		void YYFAR* attributestackptr;
		if (size <= yysstack_size) {
			stackptr = yysstackptr;
			attributestackptr = yysattributestackptr;
		}
		else {
			stackptr = (yystack_t YYFAR*)malloc(stackcharsize);
			if (stackptr == NULL) {
				return 0;
			}
			attributestackptr = malloc(attributestackcharsize);
			if (attributestackptr == NULL) {
				free(stackptr);		// clean up
				return 0;
			}
		}

		// copy
		if (stackptr != yystackptr) {
			size_t charsize = yystack_size * sizeof(yystack_t);
			memcpy(stackptr, yystackptr, min(stackcharsize, charsize));
		}
		if (attributestackptr != yyattributestackptr) {
			size_t charsize = yystack_size * yyattribute_size;
			memcpy(attributestackptr, yyattributestackptr, min(attributestackcharsize,
				charsize));
		}

		// free
		if (yystackptr != yysstackptr) {
			free(yystackptr);
		}
		if (yyattributestackptr != yysattributestackptr) {
			free(yyattributestackptr);
		}

		// assign
		yystackptr = stackptr;
		yyattributestackptr = attributestackptr;
		yystack_size = size;
	}
	return 1;
}
