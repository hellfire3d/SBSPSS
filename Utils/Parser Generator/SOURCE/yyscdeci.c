/************************************************************
yyscdeci.c
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include <string.h>
#include "yacc.h"

#ifdef YYPROTOTYPE
void YYCDECL yycdestructclearin(void)
#else
void YYCDECL yycdestructclearin()
#endif
{
	if (yylookahead) {
		/* clean up any token attributes */
		if (yyctokendestptr != NULL) {
			YYCONST yyctokendest_t YYNEARFAR *tokendestptr = yyctokendestptr;
			while (tokendestptr->token != 0) {
				if (tokendestptr->token == yychar) {
					/* user actions in here */
					memcpy(yyvalptr, yylvalptr, yyattribute_size);

					yyparseaction(tokendestptr->action);

					memcpy(yylvalptr, yyvalptr, yyattribute_size);								
					break;
				}
				tokendestptr++;
			}
		}
		yylookahead = 0;
	}
}
