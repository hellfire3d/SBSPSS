/************************************************************
yymdeci.c
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include <string.h>
#include "myacc.h"

#ifdef YYPROTOTYPE
void YYCDECL yymdestructclearin(yymparse_t YYFAR *yy)
#else
void YYCDECL yymdestructclearin(yy)
yymparse_t YYFAR *yy;
#endif
{
	yyassert(yy != NULL);
	yyassert(yymisfastparser(yy));	/* make sure it's a fast parser */

	if (yy->yymlookahead) {
		/* clean up any token attributes */
		if (yy->yymtokendestptr != NULL) {
			int index = yy->yymtokendestbase + yy->yymchar;
			if (index >= 0 && index < yy->yymtokendest_size) {
				int action = yy->yymtokendestptr[index];
				if (action != -1) {
					/* user actions in here */
					memcpy(yy->yymvalptr, yy->yymlvalptr, yy->yymattribute_size);

					(*yy->yymparseaction)(yy, action);

					memcpy(yy->yymlvalptr, yy->yymvalptr, yy->yymattribute_size);
				}
			}
		}
		yy->yymlookahead = 0;
	}
}
