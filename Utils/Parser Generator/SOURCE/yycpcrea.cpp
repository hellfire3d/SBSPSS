/************************************************************
yycpcrea.cpp
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include "cyacc.h"
#include <stdlib.h>

int yyparser::yycreate(yylexer YYFAR* lexerptr)
{
	yylexerptr = lexerptr;

	// stack
	if (yysstack_size != 0) {
		size_t stackcharsize;
		size_t attributestackcharsize;

		stackcharsize = yysstack_size * sizeof(yystack_t);
		if ((int)(stackcharsize / sizeof(yystack_t)) != yysstack_size) {
			return 0;		// integer overflow
		}
		attributestackcharsize = yysstack_size * yyattribute_size;
		if ((int)(attributestackcharsize / yyattribute_size) != yysstack_size) {
			return 0;		// integer overflow
		}
		
		yysstackptr = (yystack_t YYFAR*)malloc(stackcharsize);
		if (yysstackptr == NULL) {
			return 0;
		}
		yysattributestackptr = malloc(attributestackcharsize);
		if (yysattributestackptr == NULL) {
			free(yysstackptr);
			return 0;
		}
	}
	else {
		yysstackptr = NULL;
		yysattributestackptr = NULL;
	}
	yystack_size = yysstack_size;
	yystackptr = yysstackptr;
	yyattributestackptr = yysattributestackptr;

	// yylval
	yylvalptr = malloc(yyattribute_size);
	if (yylvalptr == NULL) {
		free(yysstackptr);
		free(yysattributestackptr);
		return 0;
	}

	// yyval ($$)
	yyvalptr = malloc(yyattribute_size);
	if (yyvalptr == NULL) {
		free(yysstackptr);
		free(yysattributestackptr);
		free(yylvalptr);
		return 0;
	}

	return 1;
}
