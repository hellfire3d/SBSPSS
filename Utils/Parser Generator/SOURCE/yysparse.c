/************************************************************
yysparse.c
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include "yacc.h"

#ifdef YYPROTOTYPE
int YYCDECL yyparse(void)
#else
int YYCDECL yyparse()
#endif
{
	int n = yysetup();
	if (n != 0) {
		return n;
	}
	return yywork();
}
