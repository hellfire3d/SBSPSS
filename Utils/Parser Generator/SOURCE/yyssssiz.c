/************************************************************
yyssssiz.c
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include <stdlib.h>
#include <string.h>
#include "yacc.h"

#ifdef YYPROTOTYPE
int YYCDECL yysetstacksize(int size)
#else
int YYCDECL yysetstacksize(size)
int size;
#endif
{
	yyassert(size >= 0);
	if (yystack_size != size) {
		yystack_t YYFAR *stackptr;
#if defined(__STDC__) || defined(__cplusplus)
		void YYFAR *attributestackptr;
#else
		char YYFAR *attributestackptr;
#endif
		size_t stackcharsize;
		size_t attributestackcharsize;

		if (size <= yytop) {
			return 0;
		}

		stackcharsize = size * sizeof(yystack_t);
		if ((int) (stackcharsize / sizeof(yystack_t)) != size) {
			return 0;		/* integer overflow */
		}
		attributestackcharsize = size * yyattribute_size;
		if ((int) (attributestackcharsize / yyattribute_size) != size) {
			return 0;		/* integer overflow */
		}

		/* allocate */
		if (size <= yysstack_size) {
			stackptr = yysstackptr;
			attributestackptr = yysattributestackptr;
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
		if (stackptr != yystackptr) {
			size_t charsize = yystack_size * sizeof(yystack_t);
			memcpy(stackptr, yystackptr, min(stackcharsize, charsize));
		}
		if (attributestackptr != yyattributestackptr) {
			size_t charsize = yystack_size * yyattribute_size;
			memcpy(attributestackptr, yyattributestackptr, min(attributestackcharsize,
				charsize));
		}

		/* free */
		if (yystackptr != yysstackptr) {
			free(yystackptr);
		}
		if (yyattributestackptr != yysattributestackptr) {
			free(yyattributestackptr);
		}

		/* assign */
		yystackptr = stackptr;
		yyattributestackptr = attributestackptr;
		yystack_size = size;
	}
	return 1;
}
