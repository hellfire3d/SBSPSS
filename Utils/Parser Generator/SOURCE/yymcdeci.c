/************************************************************
yymcdeci.c
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include <string.h>
#include "myacc.h"

#ifdef YYPROTOTYPE
void YYCDECL yymcdestructclearin(yymparse_t YYFAR *yy)
#else
void YYCDECL yymcdestructclearin(yy)
yymparse_t YYFAR *yy;
#endif
{
	yyassert(yy != NULL);
	yyassert(yymiscompactparser(yy));	/* make sure it's a compact parser */

	if (yy->yymlookahead) {
		/* clean up any token attributes */
		if (yy->yymctokendestptr != NULL) {
			YYCONST yyctokendest_t YYNEARFAR *tokendestptr = yy->yymctokendestptr;
			while (tokendestptr->token != 0) {
				if (tokendestptr->token == yy->yymchar) {
					/* user actions in here */
					memcpy(yy->yymvalptr, yy->yymlvalptr, yy->yymattribute_size);

					(*yy->yymparseaction)(yy, tokendestptr->action);

					memcpy(yy->yymlvalptr, yy->yymvalptr, yy->yymattribute_size);								
					break;
				}
				tokendestptr++;
			}
		}
		yy->yymlookahead = 0;
	}
}
