/************************************************************
yyclcon.cpp
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include "clex.h"

yylexer::yylexer()
{
	yystart = 0;
	yyeol = 1;
	yyoldeol = 1;
	yystatebuf = NULL;
	yysstatebuf = NULL;
	yystext = NULL;
	yytext_size = 0;
	yystext_size = 0;
	yyunputbufptr = NULL;
	yysunputbufptr = NULL;
	yyunput_size = 0;
	yysunput_size = 0;
	yyunputindex = 0;
	yymoreflg = 0;
	yyrejectflg = 0;
	yyreturnflg = 0;
	yyparserptr = NULL;
	yytextgrow = 1;
	yyunputgrow = 1;
	yyin = stdin;
	yyout = stdout;
	yyerr = stderr;
	yytext = NULL;
	yyleng = 0;
	yylineno = 1;

	// debugging
#ifdef YYDEBUG
	yydebug = 0;
	yydebugflush = 0;
	yydebugout = stdout;
#endif
}

yylexer::~yylexer()
{
	yydestroy();
}
