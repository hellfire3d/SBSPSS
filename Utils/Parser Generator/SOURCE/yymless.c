/************************************************************
yymless.c
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include "mlex.h"

#ifdef YYPROTOTYPE
void YYCDECL yymless(yymlex_t YYFAR *yy, int length)
#else
void YYCDECL yymless(yy, length)
yymlex_t YYFAR *yy;
int length;
#endif
{
	yyassert(yy != NULL);
	yyassert(length >= 0 && length <= yy->yymleng);
	while (yy->yymleng > length) {
		(*yy->yymunput)(yy, (unsigned char) yy->yymtext[--yy->yymleng]);
	}
	if (yy->yymleng > 0) {
		yy->yymeol = (unsigned char) (yy->yymtext[yy->yymleng - 1] == '\n');
	}
	else {
		yy->yymeol = yy->yymoldeol;
	}
}
