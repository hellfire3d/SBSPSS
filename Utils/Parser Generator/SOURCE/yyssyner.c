/************************************************************
yyssyner.c
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include "yacc.h"

#ifdef YYPROTOTYPE
void YYCDECL yysyntaxerror(void)
#else
void YYCDECL yysyntaxerror()
#endif
{
	yyerror("syntax error");
}
