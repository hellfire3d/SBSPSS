/************************************************************
yysdisc.c
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include <stdlib.h>
#include "yacc.h"

#ifdef __BORLANDC__
#pragma argsused
#endif

#ifdef YYPROTOTYPE
void YYCDECL yydiscard(int token)
#else
void YYCDECL yydiscard(token)
int token;
#endif
{
	yyassert(token > 0);
	/* do nothing */
}
