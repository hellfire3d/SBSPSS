/************************************************************
yympop.c
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include "myacc.h"

#ifdef YYDEBUG
#ifdef YYPROTOTYPE
void YYCDECL yympop(yymparse_t YYFAR *yy, int num)
#else
void YYCDECL yympop(yy, num)
yymparse_t YYFAR *yy;
int num;
#endif
{
	yyassert(yy != NULL);
	yyassert(num >= 0);
	yy->yymtop -= num;
	yyassert(yy->yymtop >= -1);

	/* debugging */
	if (yydebug || yy->yymdebug) {
		if (num > 0) {
			char string[128];

			sprintf(string, "%p: pop %d state(s)", (void *) yy, (int) num);
			yymparsedebugoutput(yy, string);
			if (yy->yymtop >= 0) {
				sprintf(string, " uncovering state %d",
					(int) yy->yymstackptr[yy->yymtop]);
				yymparsedebugoutput(yy, string);
			}
			yymparsedebugoutput(yy, "\n");
		}
	}
}
#endif
