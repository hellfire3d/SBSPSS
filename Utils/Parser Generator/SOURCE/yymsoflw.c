/************************************************************
yymsoflw.c
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include <stdlib.h>
#include "myacc.h"

#ifdef YYPROTOTYPE
void YYCDECL yymstackoverflow(yymparse_t YYFAR *yy)
#else
void YYCDECL yymstackoverflow(yy)
yymparse_t YYFAR *yy;
#endif
{
	yyassert(yy != NULL);
	(*yy->yymerror)(yy, "yacc stack overflow");
}
