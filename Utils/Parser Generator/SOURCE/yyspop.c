/************************************************************
yyspop.c
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include "yacc.h"

#ifdef YYDEBUG
#ifdef YYPROTOTYPE
void YYCDECL yypop(int num)
#else
void YYCDECL yypop(num)
int num;
#endif
{
	yyassert(num >= 0);
	yytop -= num;
	yyassert(yytop >= -1);

	/* debugging */
	if (yydebug || yyparsedebug) {
		if (num > 0) {
			char string[128];

			sprintf(string, "pop %d state(s)", (int) num);
			yyparsedebugoutput(string);
			if (yytop >= 0) {
				sprintf(string, " uncovering state %d", (int) yystackptr[yytop]);
				yyparsedebugoutput(string);
			}
			yyparsedebugoutput("\n");
		}
	}
}
#endif
