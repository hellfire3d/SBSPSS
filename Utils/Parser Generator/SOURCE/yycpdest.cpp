/************************************************************
yycpdest.cpp
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include "cyacc.h"
#include <stdlib.h>

void yyparser::yydestroy()
{
	yycleanup();
	free(yysstackptr);
	free(yysattributestackptr);

	free(yylvalptr);
	free(yyvalptr);
}
