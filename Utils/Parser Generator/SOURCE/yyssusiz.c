/************************************************************
yyssusiz.c
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include <stdlib.h>
#include <string.h>
#include "lex.h"

#ifdef YYPROTOTYPE
int YYCDECL yysetunputsize(int size)
#else
int YYCDECL yysetunputsize(size)
int size;
#endif
{
	yyassert(size >= 0);
	if (yyunput_size != size) {
		int YYFAR *unputbufptr;

		size_t unputbufcharsize;

		if (size < yyunputindex) {
			return 0;
		}

		unputbufcharsize = size * sizeof(int);
		if ((int) (unputbufcharsize / sizeof(int)) != size) {
			return 0;		/* integer overflow */
		}

		/* allocate */
		if (size <= yysunput_size) {
			unputbufptr = yysunputbufptr;
		}
		else {
			unputbufptr = (int YYFAR *) malloc(unputbufcharsize);
			if (unputbufptr == NULL) {
				return 0;
			}
		}

		/* copy */
		if (unputbufptr != yyunputbufptr) {
			size_t charsize = yyunput_size * sizeof(int);
			memcpy(unputbufptr, yyunputbufptr, min(unputbufcharsize, charsize));
		}

		/* free */
		if (yyunputbufptr != yysunputbufptr) {
			free(yyunputbufptr);
		}

		/* assign */
		yyunputbufptr = unputbufptr;
		yyunput_size = size;
	}
	return 1;
}
