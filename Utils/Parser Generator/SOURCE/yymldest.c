/************************************************************
yymldest.c
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include "mlex.h"
#include <stdlib.h>

#ifdef YYPROTOTYPE
void YYCDECL yymdestroylex(yymlex_t YYFAR *yy)
#else
void YYCDECL yymdestroylex(yy)
yymlex_t YYFAR *yy;
#endif
{
	yyassert( yy != NULL);

	yymlexcleanup(yy);
	free(yy->yymstext);
	free(yy->yymsstatebuf);
	free(yy->yymsunputbufptr);
}
