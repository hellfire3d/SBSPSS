/************************************************************
yysdeci.c
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include <string.h>
#include "yacc.h"

#ifdef YYPROTOTYPE
void YYCDECL yydestructclearin(void)
#else
void YYCDECL yydestructclearin()
#endif
{
	if (yylookahead) {
		/* clean up any token attributes */
		if (yytokendestptr != NULL) {
			int index = yytokendestbase + yychar;
			if (index >= 0 && index < yytokendest_size) {
				int action = yytokendestptr[index];
				if (action != -1) {
					/* user actions in here */
					memcpy(yyvalptr, yylvalptr, yyattribute_size);

					yyparseaction(action);

					memcpy(yylvalptr, yyvalptr, yyattribute_size);
				}
			}
		}
		yylookahead = 0;
	}
}
