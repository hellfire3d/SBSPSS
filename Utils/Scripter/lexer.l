%{
/****************************************************************************
lexer.l
ParserWizard generated Lex file.

Date: 07 December 2000
****************************************************************************/

#include "parser.h"
#include "var.h"
#include "prepro.h"
#include <stdlib.h>


%}

/////////////////////////////////////////////////////////////////////////////
// declarations section

// lexical analyser name
%name mylexer

// class definition
{
	int	openInputFile(char *_filename);
	int closeInputFile();

	void			setCurrentParser(class myparser *_parser)	{m_currentParser=_parser;}
	class myparser	*getCurrentParser()							{return m_currentParser;}

	void			error();
	int				getErrorCount();

	// Overridden lexer functions
	int				yygetchar();

private:
	void			unexpectedChar();


	class myparser	*m_currentParser;
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

^#.*						{if(preprocessorCmd(yytext+1)!=(int)true)error();}
#							{printf("# commands must be at start of line!\n");error();}

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
[0-9]+						{yylval.value=atoi(yytext);return VALUE;}
//	\"[^\"]*\"					{printf("s:%s\n",yytext);return STRING;}

\/\/.*						{}
[ \t]+						{}
\n							{}

.							{unexpectedChar();}

%%


/////////////////////////////////////////////////////////////////////////////
// programs section

