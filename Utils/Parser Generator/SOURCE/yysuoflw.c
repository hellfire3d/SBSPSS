/************************************************************
yysuoflw.c
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include "lex.h"

#ifdef YYPROTOTYPE
void YYCDECL yyunputoverflow(void)
#else
void YYCDECL yyunputoverflow()
#endif
{
	yyassert(yylexerr != NULL);
	fprintf(yylexerr, "lex unput buffer overflow (%d)\n", (int) yyunput_size);
}
