/************************************************************
yycreset.cpp
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include "clex.h"

void yylexer::yyreset()
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
