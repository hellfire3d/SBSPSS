/************************************************************
yyscpars.c
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include <string.h>
#include "yacc.h"

#ifdef YYPROTOTYPE
int YYCDECL yycparse(void)
#else
int YYCDECL yycparse()
#endif
{
	int n = yysetup();
	if (n != 0) {
		return n;
	}
	return yycwork();
}
