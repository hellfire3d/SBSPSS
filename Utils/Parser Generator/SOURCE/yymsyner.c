/************************************************************
yymsyner.c
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include <string.h>
#include "myacc.h"

#ifdef YYPROTOTYPE
void YYCDECL yymsyntaxerror(yymparse_t YYFAR *yy)
#else
void YYCDECL yymsyntaxerror(yy)
yymparse_t YYFAR *yy;
#endif
{
	yyassert(yy != NULL);
	(*yy->yymerror)(yy, "syntax error");
}
