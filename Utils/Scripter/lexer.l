%{
/****************************************************************************
lexer.l
ParserWizard generated Lex file.

Date: 07 December 2000
****************************************************************************/

#include "parser.h"
#include "var.h"
#include <stdlib.h>


%}

/////////////////////////////////////////////////////////////////////////////
// declarations section

// lexical analyser name
%name mylexer

// class definition
{
	// place any extra class members here
}

// constructor
{
	// place any extra initialisation code here
}

// place any declarations here

%%


/////////////////////////////////////////////////////////////////////////////
// rules section

%{
// extract yylval for use later on in actions
YYSTYPE& yylval = *(YYSTYPE*)yyparserptr->yylvalptr;
%}

// place your Lex rules here

^[ \t]*#.*					{/*preprocess(yytext);*/}
//#							{yyerror("# commands must be at start of line!");}
stop						{return STOP;}
if							{return IF;}
else						{return ELSE;}
pause						{return PAUSE;}
print						{return PRINT;}
=							{return ASSIGN;}
==							{return EQUAL;}
!=							{return NOTEQUAL;}
\+							{return PLUS;}
;							{return END_STMT;}
\(							{return OPEN_PAR;}
\)							{return CLOSE_PAR;}
\{							{return BEGIN_CS;}
\}							{return END_CS;}
\$[a-zA-Z_][a-zA-Z_0-9]*	{yylval.variableIdx=lookupVarName(yytext+1);return VARIABLE;}
\/\/.*						{}
[0-9]+						{yylval.value=atoi(yytext);return VALUE;}
[ \t]+						{}
\n							{/*s_linesProcessed++;*/}
.							{printf("?\n");/*printf("line:%d\n",yylineno);printf("-%s-\n",yytext);yyerror("Unexpected character in source");*/}


%%


/////////////////////////////////////////////////////////////////////////////
// programs section

