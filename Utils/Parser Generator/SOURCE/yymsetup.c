/************************************************************
yymsetup.c
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include "myacc.h"

#ifdef YYPROTOTYPE
int YYCDECL yymsetup(yymparse_t YYFAR *yy)
#else
int YYCDECL yymsetup(yy)
yymparse_t YYFAR *yy;
#endif
{
	yyassert(yy != NULL);

	/* initialise variables */
	yy->yymtop = -1;
	yy->yymlookahead = 0;
	yy->yymskip = 0;
	yy->yymerrorcount = 0;
	yy->yymchar = -1;
	yy->yympopflg = 0;

	/* push initial state onto the stack */
	if (!yympush(yy, 0)) {
#ifdef YYDEBUG
		yymdabort(yy);
#endif
		return 1;
	}
	return 0;
}
