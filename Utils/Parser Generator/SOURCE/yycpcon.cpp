/************************************************************
yycpcon.cpp
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include "cyacc.h"

yyparser::yyparser()
{
	yystackptr = NULL;
	yysstackptr = NULL;
	yyattributestackptr = NULL;
	yysattributestackptr = NULL;
	yystack_size = 0;
	yysstack_size = 0;
	yytop = -1;
	yyattribute_size = 0;
	yyvalptr = NULL;
	yylookahead = 0;
	yychar = -1;
	yywipeflg = 1;
	yypopflg = 0;
	yyskip = 0;
	yyexitflg = 0;
	yyretireflg = 0;
	yyerrorflg = 0;
	yyexitcode = 0;
	yyretirecode = 0;
	yyerrorpop = 0;
	yylexerptr = NULL;
	yystackgrow = 1;
	yylvalptr = NULL;
	yyerr = stderr;
	yyerrorcount = 0;

#ifdef YYDEBUG
	yydebug = 0;
	yydebugstack = 0;
	yydebugflush = 0;
	yydebugout = stdout;
#endif
}

yyparser::~yyparser()
{
	yydestroy();
}
