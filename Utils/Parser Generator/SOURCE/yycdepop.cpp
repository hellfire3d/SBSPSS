/************************************************************
yycdepop.cpp
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

void yyparser::yydestructpop(int num)
{
	yyassert(num >= 0);
	yyassert(yytop - num >= -1);

	if (yydestructorptr != NULL) {
		while (num > 0) {
			yystack_t state = yypeek();
			int action = yydestructorptr[state];
			if (action != -1) {
				// user actions in here
				memcpy(yyvalptr, &((char YYFAR*)yyattributestackptr)
					[yytop * yyattribute_size], yyattribute_size);

				yyaction(action);

				memcpy(&((char YYFAR*)yyattributestackptr)
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
