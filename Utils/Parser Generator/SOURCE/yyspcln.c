/************************************************************
yyspcln.c
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include <stdlib.h>
#include "yacc.h"

#ifdef YYPROTOTYPE
void YYCDECL yyparsecleanup(void)
#else
void YYCDECL yyparsecleanup()
#endif
{
	if (yystackptr != yysstackptr) {
		free(yystackptr);
		yystackptr = yysstackptr;
	}
	if (yyattributestackptr != yysattributestackptr) {
		free(yyattributestackptr);
		yyattributestackptr = yysattributestackptr;
	}
	yystack_size = yysstack_size;

	yytop = -1;
}
