/************************************************************
yyssskip.c
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include <stdio.h>
#include "yacc.h"

#ifdef YYDEBUG
#ifdef YYPROTOTYPE
void YYCDECL yysetskip(int skip)
#else
void YYCDECL yysetskip(skip)
int skip;
#endif
{
	yyassert(skip >= 0);

	/* debugging */
	if (yydebug || yyparsedebug) {
		if (skip > 0) {
			if (yyskip == 0) {
				yyparsedebugoutput("entering error recovery\n");
			}
		}
		else {
			if (yyskip > 0) {
				yyparsedebugoutput("leaving error recovery\n");
			}
		}
	}

	yyskip = skip;
}
#endif
