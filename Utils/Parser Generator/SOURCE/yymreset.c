/************************************************************
yymreset.c
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include "mlex.h"

#ifdef YYPROTOTYPE
void YYCDECL yymreset(yymlex_t YYFAR *yy)
#else
void YYCDECL yymreset(yy)
yymlex_t YYFAR *yy;
#endif
{
	yyassert(yy != NULL);
	yy->yymleng = 0;
	yy->yymlineno = 1;
	yy->yymunputindex = 0;
	yy->yymmoreflg = 0;
	yy->yymrejectflg = 0;
	yy->yymeol = 1;
	yy->yymoldeol = 1;
	yy->yymstart = 0;
}
