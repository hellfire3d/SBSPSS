/************************************************************
yymecho.c
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include <stdio.h>
#include "mlex.h"

#ifdef YYPROTOTYPE
void YYCDECL yymecho(yymlex_t YYFAR *yy)
#else
void YYCDECL yymecho(yy)
yymlex_t YYFAR *yy;
#endif
{
	int i;
	yyassert(yy != NULL);

	for (i = 0; i < yy->yymleng; i++) {
		(*yy->yymoutput)(yy, yy->yymtext[i]);
	}
}
