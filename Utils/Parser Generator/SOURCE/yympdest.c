/************************************************************
yympdest.c
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include "myacc.h"
#include <stdlib.h>

#ifdef YYPROTOTYPE
void YYCDECL yymdestroyparse(yymparse_t YYFAR *yy)
#else
void YYCDECL yymdestroyparse(yy)
yymparse_t YYFAR *yy;
#endif
{
	yyassert(yy != NULL);

	yymparsecleanup(yy);
	free(yy->yymsstackptr);
	free(yy->yymsattributestackptr);

	free(yy->yymlvalptr);
	free(yy->yymvalptr);
}
