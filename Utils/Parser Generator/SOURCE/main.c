/************************************************************
main.c
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include "yacc.h"

#ifdef YYPROTOTYPE
int main(void)
#else
int main()
#endif
{
	return yyparse();
}
