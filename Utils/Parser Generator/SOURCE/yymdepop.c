/************************************************************
yymdepop.c
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include <string.h>
#include "myacc.h"

#ifdef YYPROTOTYPE
void YYCDECL yymdestructpop(yymparse_t YYFAR *yy, int num)
#else
void YYCDECL yymdestructpop(yy, num)
yymparse_t YYFAR *yy;
int num;
#endif
{
	yyassert(yy != NULL);
	yyassert(num >= 0);
	yyassert(yy->yymtop - num >= -1);

	if (yy->yymdestructorptr != NULL) {
		while (num > 0) {
			yystack_t state = yympeek(yy);
			int action = yy->yymdestructorptr[state];
			if (action != -1) {
				/* user actions in here */
				memcpy(yy->yymvalptr, &((char YYFAR *) yy->yymattributestackptr)
					[yy->yymtop * yy->yymattribute_size], yy->yymattribute_size);

				(*yy->yymparseaction)(yy, action);

				memcpy(&((char YYFAR *) yy->yymattributestackptr)
					[yy->yymtop * yy->yymattribute_size], yy->yymvalptr,
					yy->yymattribute_size);
			}
			yympop(yy, 1);
			num--;
		}
	}
	else {
		yympop(yy, num);
	}
}
