/************************************************************
yyspvars.c
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include "yacc.h"

unsigned char YYNEAR YYDCDECL yylookahead = 0;	/* whether we have lookahead token */
unsigned char YYNEAR YYDCDECL yystackgrow = 1;	/* do we allow stack to grow? */
unsigned char YYNEAR YYDCDECL yyexitflg = 0;	/* whether yyexit called */
unsigned char YYNEAR YYDCDECL yyretireflg = 0;	/* whether yyretire called */
unsigned char YYNEAR YYDCDECL yyerrorflg = 0;	/* whether yyforceerror called */
unsigned char YYNEAR YYDCDECL yypopflg = 0;		/* popping symbols during error recovery */
unsigned char YYNEAR YYDCDECL yywipeflg = 1;	/* whether to "wipe" stack on abort */
int YYNEAR YYDCDECL yytop = -1;					/* the current top of the stack */
int YYNEAR YYDCDECL yychar = -1;				/* lookahead token */
int YYNEAR YYDCDECL yyskip = 0;					/* skip erroneous characters */
int YYNEAR YYDCDECL yyerrorcount = 0;			/* syntax error count */
int YYNEAR YYDCDECL yyexitcode;					/* yyexit exit code */
int YYNEAR YYDCDECL yyretirecode;				/* yyretire exit code */
int YYNEAR YYDCDECL yyerrorpop;					/* how many error transitions to pop */

#ifndef YYNINITSTREAM
FILE YYFAR *YYNEAR YYDCDECL yyparseerr = stderr;
#else
FILE YYFAR *YYNEAR YYDCDECL yyparseerr = NULL;
#endif
