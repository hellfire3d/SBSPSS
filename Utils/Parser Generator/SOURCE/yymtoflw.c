/************************************************************
yymtoflw.c
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include "mlex.h"

#ifdef YYPROTOTYPE
void YYCDECL yymtextoverflow(yymlex_t YYFAR *yy)
#else
void YYCDECL yymtextoverflow(yy)
yymlex_t YYFAR *yy;
#endif
{
	yyassert(yy != NULL);
	yyassert(yy->yymerr != NULL);
	fprintf(yy->yymerr, "lex text buffer overflow (%d)\n", (int) yy->yymtext_size);
}
