/************************************************************
yyssoflw.c
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include <stdlib.h>
#include "yacc.h"

#ifdef YYPROTOTYPE
void YYCDECL yystackoverflow(void)
#else
void YYCDECL yystackoverflow()
#endif
{
	yyerror("yacc stack overflow");
}
