/************************************************************
yymsssiz.c
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include <stdlib.h>
#include <string.h>
#include "myacc.h"

#ifdef YYPROTOTYPE
int YYCDECL yymsetstacksize(yymparse_t YYFAR *yy, int size)
#else
int YYCDECL yymsetstacksize(yy, size)
yymparse_t YYFAR *yy;
int size;
#endif
{
	yyassert(yy != NULL);
	yyassert(size >= 0);
	if (yy->yymstack_size != size) {
		yystack_t YYFAR *stackptr;
#if defined(__STDC__) || defined(__cplusplus)
		void YYFAR *attributestackptr;
#else
		char YYFAR *attributestackptr;
#endif
		size_t stackcharsize;
		size_t attributestackcharsize;

		if (size <= yy->yymtop) {
			return 0;
		}

		stackcharsize = size * sizeof(yystack_t);
		if ((int) (stackcharsize / sizeof(yystack_t)) != size) {
			return 0;		/* integer overflow */
		}
		attributestackcharsize = size * yy->yymattribute_size;
		if ((int) (attributestackcharsize / yy->yymattribute_size) != size) {
			return 0;		/* integer overflow */
		}

		/* allocate */
		if (size <= yy->yymsstack_size) {
			stackptr = yy->yymsstackptr;
			attributestackptr = yy->yymsattributestackptr;
		}
		else {
			stackptr = (yystack_t YYFAR *) malloc(stackcharsize);
			if (stackptr == NULL) {
				return 0;
			}
			attributestackptr = malloc(attributestackcharsize);
			if (attributestackptr == NULL) {
				free(stackptr);		/* clean up */
				return 0;
			}
		}

		/* copy */
		if (stackptr != yy->yymstackptr) {
			size_t charsize = yy->yymstack_size * sizeof(yystack_t);
			memcpy(stackptr, yy->yymstackptr, min(stackcharsize, charsize));
		}
		if (attributestackptr != yy->yymattributestackptr) {
			size_t charsize = yy->yymstack_size * yy->yymattribute_size;
			memcpy(attributestackptr, yy->yymattributestackptr, min(attributestackcharsize,
				charsize));
		}

		/* free */
		if (yy->yymstackptr != yy->yymsstackptr) {
			free(yy->yymstackptr);
		}
		if (yy->yymattributestackptr != yy->yymsattributestackptr) {
			free(yy->yymattributestackptr);
		}

		/* assign */
		yy->yymstackptr = stackptr;
		yy->yymattributestackptr = attributestackptr;
		yy->yymstack_size = size;
	}
	return 1;
}
