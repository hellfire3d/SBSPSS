/************************************************************
yydebug.c
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include "yacc.h"
#include "lex.h"
#include "myacc.h"
#include "mlex.h"

#ifdef YYDEBUG
int YYNEAR YYDCDECL yydebug = 0;
int YYNEAR YYDCDECL yydebugstack = 0;	/* parsers only */
int YYNEAR YYDCDECL yydebugflush = 0;
#endif
