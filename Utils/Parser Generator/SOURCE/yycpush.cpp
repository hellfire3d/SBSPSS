/************************************************************
yycpush.cpp
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include <stdlib.h>
#include <string.h>
#include "cyacc.h"

int yyparser::yypush(yystack_t state)
{
	yytop++;		// increment first
	if (yytop == yystack_size) {
		do {
			if (yystackgrow) {
				if (yystack_size != 0) {
					int size = yystack_size * 2;
					if (size / 2 == yystack_size) {		// overflow check
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

			// debugging
#ifdef YYDEBUG
			if (::yydebug || yydebug) {
				char string[128];

				sprintf(string, "%p: stack overflow\n", (void*)this);
				yydebugoutput(string);
			}
#endif

			yystackoverflow();
			return 0;
		}
		while (0);
	}
	yystackptr[yytop] = state;

	// debugging
#ifdef YYDEBUG
	if (::yydebug || yydebug) {
		char string[128];

		sprintf(string, "%p: push state %d", (void*)this, (int)state);
		yydebugoutput(string);
		if (yytop > 0) {
			sprintf(string, " covering state %d", (int)yystackptr[yytop - 1]);
			yydebugoutput(string);
		}
		yydebugoutput("\n");

		// output stack contents
		if (::yydebugstack || yydebugstack) {
			yydebugoutput("\nstack");
			int n = sprintf(string, "\n     +");
			int i;
			for (i = 0; i < 10; i++) {
				n += sprintf(&string[n], " %5d", (int) i);
			}
			yydebugoutput(string);

			int rows = 1;
			if (yytop >= 0) {
				rows += yytop / 10;
			}
			for (i = 0; i < rows; i++) {
				n = sprintf(string, "\n %5d", (int) (10 * i));
				for (int j = 0; j < 10; j++) {
					int index = 10 * i + j;
					if (index <= yytop) {
						n += sprintf(&string[n], " %5d", (int) yystackptr[index]);
					}
					else {
						n += sprintf(&string[n], "     -");
					}
				}
				yydebugoutput(string);
			}
			yydebugoutput("\n\n");
		}
	}
#endif

	return 1;
}
