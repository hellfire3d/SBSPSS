/************************************************************
yyspinit.c
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include "yacc.h"

#ifdef YYPROTOTYPE
void YYCDECL yyparseinit(void)
#else
void YYCDECL yyparseinit()
#endif
{
	yyparseerr = stderr;
#ifdef YYDEBUG
	yyparsedebugout = stdout;
#endif
}
