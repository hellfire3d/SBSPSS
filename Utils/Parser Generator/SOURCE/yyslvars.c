/************************************************************
yyslvars.c
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include "lex.h"

int YYNEAR YYDCDECL yyleng = 0;
int YYNEAR YYDCDECL yylineno = 1;
int YYNEAR YYDCDECL yyunputindex = 0;
unsigned char YYNEAR YYDCDECL yymoreflg = 0;
unsigned char YYNEAR YYDCDECL yyrejectflg;
unsigned char YYNEAR YYDCDECL yyreturnflg;
unsigned char YYNEAR YYDCDECL yytextgrow = 1;
unsigned char YYNEAR YYDCDECL yyunputgrow = 1;
unsigned char YYNEAR YYDCDECL yyeol = 1;
unsigned char YYNEAR YYDCDECL yyoldeol = 1;
int YYNEAR YYDCDECL yystart = 0;

#ifndef YYNINITSTREAM
FILE YYFAR *YYNEAR YYDCDECL yyin = stdin;
FILE YYFAR *YYNEAR YYDCDECL yyout = stdout;
FILE YYFAR *YYNEAR YYDCDECL yylexerr = stderr;
#else
FILE YYFAR *YYNEAR YYDCDECL yyin = NULL;
FILE YYFAR *YYNEAR YYDCDECL yyout = NULL;
FILE YYFAR *YYNEAR YYDCDECL yylexerr = NULL;
#endif
