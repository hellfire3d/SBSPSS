/************************************************************
yyspush.c
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include <stdlib.h>
#include <string.h>
#include "yacc.h"

#ifdef YYPROTOTYPE
int YYCDECL yypush(yystack_t state)
#else
int YYCDECL yypush(state)
yystack_t state;
#endif
{
	yytop++;		/* increment first */
	if (yytop == yystack_size) {
		do {
			if (yystackgrow) {
				if (yystack_size != 0) {
					int size = yystack_size * 2;
					if (size / 2 == yystack_size) {		/* overflow check */
						if (yysetstacksize(size)) {
							break;
						}
					}
				}
				else {
					if (yysetstacksize(100)) {
						break;
					}
				}
			}
			yytop--;

			/* debugging */
#ifdef YYDEBUG
			if (yydebug || yyparsedebug) {
				yyparsedebugoutput("stack overflow\n");
			}
#endif

			yystackoverflow();
			return 0;
		}
		while (0);
	}
	yystackptr[yytop] = state;

	/* debugging */
#ifdef YYDEBUG
	if (yydebug || yyparsedebug) {
		char string[128];

		sprintf(string, "push state %d", (int) state);
		yyparsedebugoutput(string);
		if (yytop > 0) {
			sprintf(string, " covering state %d", (int) yystackptr[yytop - 1]);
			yyparsedebugoutput(string);
		}
		yyparsedebugoutput("\n");

		/* output stack contents */
		if (yydebugstack || yyparsedebugstack) {
			int rows;
			int i;
			int n;

			yyparsedebugoutput("\nstack");
			n = sprintf(string, "\n     +");
			for (i = 0; i < 10; i++) {
				n += sprintf(&string[n], " %5d", (int) i);
			}
			yyparsedebugoutput(string);

			rows = 1;
			if (yytop >= 0) {
				rows += yytop / 10;
			}
			for (i = 0; i < rows; i++) {
				int j;
				n = sprintf(string, "\n %5d", (int) (10 * i));
				for (j = 0; j < 10; j++) {
					int index = 10 * i + j;
					if (index <= yytop) {
						n += sprintf(&string[n], " %5d", (int) yystackptr[index]);
					}
					else {
						n += sprintf(&string[n], "     -");
					}
				}
				yyparsedebugoutput(string);
			}
			yyparsedebugoutput("\n\n");
		}
	}
#endif

	return 1;
}
