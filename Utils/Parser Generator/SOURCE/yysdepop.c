/************************************************************
yysdepop.c
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include <string.h>
#include "yacc.h"

#ifdef YYPROTOTYPE
void YYCDECL yydestructpop(int num)
#else
void YYCDECL yydestructpop(num)
int num;
#endif
{
	yyassert(num >= 0);
	yyassert(yytop - num >= -1);

	if (yydestructorptr != NULL) {
		while (num > 0) {
			yystack_t state = yypeek();
			int action = yydestructorptr[state];
			if (action != -1) {
				/* user actions in here */
				memcpy(yyvalptr, &((char YYFAR *) yyattributestackptr)
					[yytop * yyattribute_size], yyattribute_size);

				yyparseaction(action);

				memcpy(&((char YYFAR *) yyattributestackptr)
					[yytop * yyattribute_size], yyvalptr, yyattribute_size);
			}
			yypop(1);
			num--;
		}
	}
	else {
		yypop(num);
	}
}
