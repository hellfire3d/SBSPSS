/************************************************************
yycldest.cpp
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include "clex.h"
#include <stdlib.h>

void yylexer::yydestroy()
{
	yycleanup();
	free(yystext);
	free(yysstatebuf);
	free(yysunputbufptr);
}
