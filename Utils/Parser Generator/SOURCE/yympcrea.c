/************************************************************
yympcrea.c
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include "myacc.h"
#include <stdlib.h>

#ifdef YYPROTOTYPE
int YYCDECL yymcreateparse(yymparse_t YYFAR *yy, YYCONST yymparse_t YYFAR *src)
#else
int YYCDECL yymcreateparse(yy, src)
yymparse_t YYFAR *yy;
YYCONST yymparse_t YYFAR *src;
#endif
{
	yyassert(yy != NULL);
	yyassert(src != NULL);

	*yy = *src;

	/* stack */
	if (yy->yymsstack_size != 0) {
		size_t stackcharsize;
		size_t attributestackcharsize;

		stackcharsize = yy->yymsstack_size * sizeof(yystack_t);
		if ((int) (stackcharsize / sizeof(yystack_t)) != yy->yymsstack_size) {
			return 0;		/* integer overflow */
		}
		attributestackcharsize = yy->yymsstack_size * yy->yymattribute_size;
		if ((int) (attributestackcharsize / yy->yymattribute_size) != yy->yymsstack_size) {
			return 0;		/* integer overflow */
		}
		
		yy->yymsstackptr = (yystack_t YYFAR *) malloc(stackcharsize);
		if (yy->yymsstackptr == NULL) {
			return 0;
		}
		yy->yymsattributestackptr = malloc(attributestackcharsize);
		if (yy->yymsattributestackptr == NULL) {
			free(yy->yymsstackptr);
			return 0;
		}
	}
	else {
		yy->yymsstackptr = NULL;
		yy->yymsattributestackptr = NULL;
	}
	yy->yymstack_size = yy->yymsstack_size;
	yy->yymstackptr = yy->yymsstackptr;
	yy->yymattributestackptr = yy->yymsattributestackptr;

	/* yylval */
	yy->yymlvalptr = malloc(yy->yymattribute_size);
	if (yy->yymlvalptr == NULL) {
		free(yy->yymsstackptr);
		free(yy->yymsattributestackptr);
		return 0;
	}

	/* yyval ($$) */
	yy->yymvalptr = malloc(yy->yymattribute_size);
	if (yy->yymvalptr == NULL) {
		free(yy->yymsstackptr);
		free(yy->yymsattributestackptr);
		free(yy->yymlvalptr);
		return 0;
	}

	return 1;
}
