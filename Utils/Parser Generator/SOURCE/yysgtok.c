/************************************************************
yysgtok.c
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include "yacc.h"

#ifdef YYPROTOTYPE
int YYCDECL yygettoken(void)
#else
int YYCDECL yygettoken()
#endif
{
	return yylex();
}
