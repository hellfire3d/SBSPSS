/************************************************************
yympcln.c
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include <stdlib.h>
#include "myacc.h"

#ifdef YYPROTOTYPE
void YYCDECL yymparsecleanup(yymparse_t YYFAR *yy)
#else
void YYCDECL yymparsecleanup(yy)
yymparse_t YYFAR *yy;
#endif
{
	if (yy->yymstackptr != yy->yymsstackptr) {
		free(yy->yymstackptr);
		yy->yymstackptr = yy->yymsstackptr;
	}
	if (yy->yymattributestackptr != yy->yymsattributestackptr) {
		free(yy->yymattributestackptr);
		yy->yymattributestackptr = yy->yymsattributestackptr;
	}
	yy->yymstack_size = yy->yymsstack_size;

	yy->yymtop = -1;
}
