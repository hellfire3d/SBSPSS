/************************************************************
yyccdeci.cpp
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include <string.h>
#include "cyacc.h"

// the Visual C++ v1.52 compiler generates an error in NT if this isn't present!
#ifdef _MSC_VER
#if defined(M_I86HM) && defined(NDEBUG)
#pragma function(memcpy)
#endif
#endif

void yycparser::yydestructclearin()
{
	if (yylookahead) {
		// clean up any token attributes
		if (yyctokendestptr != NULL) {
			const yyctokendest_t YYNEARFAR* tokendestptr = yyctokendestptr;
			while (tokendestptr->token != 0) {
				if (tokendestptr->token == yychar) {
					// user actions in here
					memcpy(yyvalptr, yylvalptr, yyattribute_size);

					yyaction(tokendestptr->action);

					memcpy(yylvalptr, yyvalptr, yyattribute_size);								
					break;
				}
				tokendestptr++;
			}
		}
		yylookahead = 0;
	}
}
