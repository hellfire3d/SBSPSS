/************************************************************
yycwipe.cpp
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include <string.h>
#include "cyacc.h"

void yyparser::yywipe()
{
	yydestructpop(yytop + 1);
	yydestructclearin();
}
