/************************************************************
yymuoflw.c
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include "mlex.h"

#ifdef YYPROTOTYPE
void YYCDECL yymunputoverflow(yymlex_t YYFAR *yy)
#else
void YYCDECL yymunputoverflow(yy)
yymlex_t YYFAR *yy;
#endif
{
	yyassert(yy != NULL);
	yyassert(yy->yymerr != NULL);
	fprintf(yy->yymerr, "lex unput buffer overflow (%d)\n", (int) yy->yymunput_size);
}
