/************************************************************
yymsetin.c
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include "myacc.h"

#ifdef YYPROTOTYPE
void YYCDECL yymsetin(yymparse_t YYFAR *yy, int token)
#else
void YYCDECL yymsetin(yy, token)
yymparse_t YYFAR *yy;
int token;
#endif
{
	yyassert(yy != NULL);
	if (token < 0) {
		token = 0;
	}
	yy->yymchar = token;
	yy->yymlookahead = 1;
}
