/************************************************************
yysreset.c
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include "lex.h"

#ifdef YYPROTOTYPE
void YYCDECL yyreset(void)
#else
void YYCDECL yyreset()
#endif
{
	yyleng = 0;
	yylineno = 1;
	yyunputindex = 0;
	yymoreflg = 0;
	yyrejectflg = 0;
	yyeol = 1;
	yyoldeol = 1;
	yystart = 0;
}
