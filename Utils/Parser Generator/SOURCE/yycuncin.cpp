/************************************************************
yycuncin.cpp
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include "cyacc.h"

int yyparser::yyunclearin()
{
	if (!yylookahead && yychar != -1) {
		yylookahead = 1;
		return 1;
	}
	return 0;
}
