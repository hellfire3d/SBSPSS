/************************************************************
yycdeci.cpp
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

void yyfparser::yydestructclearin()
{
	if (yylookahead) {
		// clean up any token attributes
		if (yytokendestptr != NULL) {
			int index = yytokendestbase + yychar;
			if (index >= 0 && index < yytokendest_size) {
				int action = yytokendestptr[index];
				if (action != -1) {
					// user actions in here
					memcpy(yyvalptr, yylvalptr, yyattribute_size);

					yyaction(action);

					memcpy(yylvalptr, yyvalptr, yyattribute_size);
				}
			}
		}
		yylookahead = 0;
	}
}
