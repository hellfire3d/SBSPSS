/************************************************************
yycsskip.cpp
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include <stdio.h>
#include "cyacc.h"

#ifdef YYDEBUG
void yyparser::yysetskip(int skip)
{
	yyassert(skip >= 0);

	// debugging
	if (::yydebug || yydebug) {
		if (skip > 0) {
			if (yyskip == 0) {
				char string[128];

				sprintf(string, "%p: entering error recovery\n", (void*)this);
				yydebugoutput(string);
			}
		}
		else {
			if (yyskip > 0) {
				char string[128];

				sprintf(string, "%p: leaving error recovery\n", (void*)this);
				yydebugoutput(string);
			}
		}
	}

	yyskip = skip;
}
#endif
