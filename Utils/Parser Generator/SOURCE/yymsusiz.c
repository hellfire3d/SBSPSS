/************************************************************
yymsusiz.c
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include <stdlib.h>
#include <string.h>
#include "mlex.h"

#ifdef YYPROTOTYPE
int YYCDECL yymsetunputsize(yymlex_t YYFAR *yy, int size)
#else
int YYCDECL yymsetunputsize(yy, size)
yymlex_t YYFAR *yy;
int size;
#endif
{
	yyassert(yy != NULL);
	yyassert(size >= 0);
	if (yy->yymunput_size != size) {
		int YYFAR *unputbufptr;

		size_t unputbufcharsize;

		if (size < yy->yymunputindex) {
			return 0;
		}

		unputbufcharsize = size * sizeof(int);
		if ((int) (unputbufcharsize / sizeof(int)) != size) {
			return 0;		/* integer overflow */
		}

		/* allocate */
		if (size <= yy->yymsunput_size) {
			unputbufptr = yy->yymsunputbufptr;
		}
		else {
			unputbufptr = (int YYFAR *) malloc(unputbufcharsize);
			if (unputbufptr == NULL) {
				return 0;
			}
		}

		/* copy */
		if (unputbufptr != yy->yymunputbufptr) {
			size_t charsize = yy->yymunput_size * sizeof(int);
			memcpy(unputbufptr, yy->yymunputbufptr, min(unputbufcharsize, charsize));
		}

		/* free */
		if (yy->yymunputbufptr != yy->yymsunputbufptr) {
			free(yy->yymunputbufptr);
		}

		/* assign */
		yy->yymunputbufptr = unputbufptr;
		yy->yymunput_size = size;
	}
	return 1;
}
