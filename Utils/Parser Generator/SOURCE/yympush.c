/************************************************************
yympush.c
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include <stdlib.h>
#include <string.h>
#include "myacc.h"

#ifdef YYPROTOTYPE
int YYCDECL yympush(yymparse_t YYFAR *yy, yystack_t state)
#else
int YYCDECL yympush(yy, state)
yymparse_t YYFAR *yy;
yystack_t state;
#endif
{
	yyassert(yy != NULL);

	yy->yymtop++;		/* increment first */
	if (yy->yymtop == yy->yymstack_size) {
		do {
			if (yy->yymstackgrow) {
				if (yy->yymstack_size != 0) {
					int size = yy->yymstack_size * 2;
					if (size / 2 == yy->yymstack_size) {		/* overflow check */
						if (yymsetstacksize(yy, size)) {
							break;
						}
					}
				}
				else {
					if (yymsetstacksize(yy, 100)) {
						break;
					}
				}
			}
			yy->yymtop--;

			/* debugging */
#ifdef YYDEBUG
			if (yydebug || yy->yymdebug) {
				char string[128];

				sprintf(string, "%p: stack overflow\n", (void *) yy);
				yymparsedebugoutput(yy, string);
			}
#endif

			(*yy->yymstackoverflow)(yy);
			return 0;
		}
		while (0);
	}
	yy->yymstackptr[yy->yymtop] = state;

	/* debugging */
#ifdef YYDEBUG
	if (yydebug || yy->yymdebug) {
		char string[128];

		sprintf(string, "%p: push state %d", (void *) yy, (int) state);
		yymparsedebugoutput(yy, string);
		if (yy->yymtop > 0) {
			sprintf(string, " covering state %d",
				(int) yy->yymstackptr[yy->yymtop - 1]);
			yymparsedebugoutput(yy, string);
		}
		yymparsedebugoutput(yy, "\n");

		/* output stack contents */
		if (yydebugstack || yy->yymdebugstack) {
			int rows;
			int i;
			int n;

			yymparsedebugoutput(yy, "\nstack");
			n = sprintf(string, "\n     +");
			for (i = 0; i < 10; i++) {
				n += sprintf(&string[n], " %5d", (int) i);
			}
			yymparsedebugoutput(yy, string);

			rows = 1;
			if (yy->yymtop >= 0) {
				rows += yy->yymtop / 10;
			}
			for (i = 0; i < rows; i++) {
				int j;
				n = sprintf(string, "\n %5d", (int) (10 * i));
				for (j = 0; j < 10; j++) {
					int index = 10 * i + j;
					if (index <= yy->yymtop) {
						n += sprintf(&string[n], " %5d", (int) yy->yymstackptr[index]);
					}
					else {
						n += sprintf(&string[n], "     -");
					}
				}
				yymparsedebugoutput(yy, string);
			}
			yymparsedebugoutput(yy, "\n\n");
		}
	}
#endif

	return 1;
}
