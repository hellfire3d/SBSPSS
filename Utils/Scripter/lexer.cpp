#include <clex.h>

#line 1 "C:\\spongebob\\Utils\\scripter\\lexer.l"

/****************************************************************************
lexer.l
ParserWizard generated Lex file.

Date: 07 December 2000
****************************************************************************/

#include "parser.h"
#include "var.h"
#include "function.h"
#include "prepro.h"
#include <stdlib.h>



#line 21 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
// repeated because of possible precompiled header
#include <clex.h>

#include "lexer.h"

/////////////////////////////////////////////////////////////////////////////
// constructor

YYLEXNAME::YYLEXNAME()
{
	yytables();
#line 46 "C:\\spongebob\\Utils\\scripter\\lexer.l"

	// place any extra initialisation code here

#line 37 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
}

#ifndef YYTEXT_SIZE
#define YYTEXT_SIZE 100
#endif
#ifndef YYUNPUT_SIZE
#define YYUNPUT_SIZE YYTEXT_SIZE
#endif

// backwards compatability with lex
#ifdef input
int YYLEXNAME::yyinput()
{
	return input();
}
#else
#define input yyinput
#endif

#ifdef output
void YYLEXNAME::yyoutput(int ch)
{
	output(ch);
}
#else
#define output yyoutput
#endif

#ifdef unput
void YYLEXNAME::yyunput(int ch)
{
	unput(ch);
}
#else
#define unput yyunput
#endif

#ifndef YYNBORLANDWARN
#ifdef __BORLANDC__
#pragma warn -rch		// <warning: unreachable code> off
#endif
#endif

int YYLEXNAME::yyaction(int action)
{
#line 58 "C:\\spongebob\\Utils\\scripter\\lexer.l"

// extract yylval for use later on in actions
YYSTYPE& yylval = *(YYSTYPE*)yyparserptr->yylvalptr;

#line 88 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
	yyreturnflg = 1;
	switch (action) {
	case 1:
		{
#line 65 "C:\\spongebob\\Utils\\scripter\\lexer.l"
if(preprocessorCmd(yytext+1)!=(int)true)error();
#line 95 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 2:
		{
#line 66 "C:\\spongebob\\Utils\\scripter\\lexer.l"
printf("# commands must be at start of line!\n");error();
#line 102 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 3:
		{
#line 68 "C:\\spongebob\\Utils\\scripter\\lexer.l"
return STOP;
#line 109 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 4:
		{
#line 69 "C:\\spongebob\\Utils\\scripter\\lexer.l"
return IF;
#line 116 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 5:
		{
#line 70 "C:\\spongebob\\Utils\\scripter\\lexer.l"
return ELSE;
#line 123 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 6:
		{
#line 71 "C:\\spongebob\\Utils\\scripter\\lexer.l"
return PAUSE;
#line 130 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 7:
		{
#line 72 "C:\\spongebob\\Utils\\scripter\\lexer.l"
return PRINT;
#line 137 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 8:
		{
#line 73 "C:\\spongebob\\Utils\\scripter\\lexer.l"
return ASSIGN;
#line 144 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 9:
		{
#line 74 "C:\\spongebob\\Utils\\scripter\\lexer.l"
return EQUAL;
#line 151 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 10:
		{
#line 75 "C:\\spongebob\\Utils\\scripter\\lexer.l"
return NOTEQUAL;
#line 158 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 11:
		{
#line 76 "C:\\spongebob\\Utils\\scripter\\lexer.l"
return PLUS;
#line 165 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 12:
		{
#line 77 "C:\\spongebob\\Utils\\scripter\\lexer.l"
return END_STMT;
#line 172 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 13:
		{
#line 78 "C:\\spongebob\\Utils\\scripter\\lexer.l"
return OPEN_PAR;
#line 179 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 14:
		{
#line 79 "C:\\spongebob\\Utils\\scripter\\lexer.l"
return CLOSE_PAR;
#line 186 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 15:
		{
#line 80 "C:\\spongebob\\Utils\\scripter\\lexer.l"
return BEGIN_CS;
#line 193 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 16:
		{
#line 81 "C:\\spongebob\\Utils\\scripter\\lexer.l"
return END_CS;
#line 200 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 17:
		{
#line 82 "C:\\spongebob\\Utils\\scripter\\lexer.l"
return COMMA;
#line 207 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 18:
		{
#line 85 "C:\\spongebob\\Utils\\scripter\\lexer.l"
yylval.variableIdx=lookupVarName(yytext+1);return VARIABLE;
#line 214 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 19:
		{
#line 86 "C:\\spongebob\\Utils\\scripter\\lexer.l"
yylval.value=atoi(yytext);return VALUE;
#line 221 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 20:
		{
#line 89 "C:\\spongebob\\Utils\\scripter\\lexer.l"
yylval.functionNumber=lookupFunctionName(yytext+1);return FUNCTION;
#line 228 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 21:
		{
#line 91 "C:\\spongebob\\Utils\\scripter\\lexer.l"

#line 235 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 22:
		{
#line 92 "C:\\spongebob\\Utils\\scripter\\lexer.l"

#line 242 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 23:
		{
#line 93 "C:\\spongebob\\Utils\\scripter\\lexer.l"

#line 249 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 24:
		{
#line 95 "C:\\spongebob\\Utils\\scripter\\lexer.l"
unexpectedChar();
#line 256 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	default:
		yyassert(0);
		break;
	}
	yyreturnflg = 0;
	return 0;
}

#ifndef YYNBORLANDWARN
#ifdef __BORLANDC__
#pragma warn .rch		// <warning: unreachable code> to the old state
#endif
#endif
#line 97 "C:\\spongebob\\Utils\\scripter\\lexer.l"



/////////////////////////////////////////////////////////////////////////////
// programs section


#line 280 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"

void YYLEXNAME::yytables()
{
	yystext_size = YYTEXT_SIZE;
	yysunput_size = YYUNPUT_SIZE;

	static const yymatch_t YYNEARFAR YYBASED_CODE match[] = {
		0
	};
	yymatch = match;

	yytransitionmax = 336;
	static const yytransition_t YYNEARFAR YYBASED_CODE transition[] = {
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 21, 21 },
		{ 0, 33 },
		{ 0, 44 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 21, 21 },
		{ 0, 0 },
		{ 0, 0 },
		{ 3, 1 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 33, 20 },
		{ 31, 31 },
		{ 31, 31 },
		{ 31, 31 },
		{ 31, 31 },
		{ 31, 31 },
		{ 31, 31 },
		{ 31, 31 },
		{ 31, 31 },
		{ 31, 31 },
		{ 31, 31 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 29, 8 },
		{ 30, 9 },
		{ 0, 0 },
		{ 0, 0 },
		{ 31, 31 },
		{ 31, 31 },
		{ 31, 31 },
		{ 31, 31 },
		{ 31, 31 },
		{ 31, 31 },
		{ 31, 31 },
		{ 31, 31 },
		{ 31, 31 },
		{ 31, 31 },
		{ 31, 31 },
		{ 31, 31 },
		{ 31, 31 },
		{ 31, 31 },
		{ 31, 31 },
		{ 31, 31 },
		{ 31, 31 },
		{ 31, 31 },
		{ 31, 31 },
		{ 31, 31 },
		{ 31, 31 },
		{ 31, 31 },
		{ 31, 31 },
		{ 31, 31 },
		{ 31, 31 },
		{ 31, 31 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 31, 31 },
		{ 0, 0 },
		{ 31, 31 },
		{ 31, 31 },
		{ 31, 31 },
		{ 31, 31 },
		{ 31, 31 },
		{ 31, 31 },
		{ 31, 31 },
		{ 31, 31 },
		{ 31, 31 },
		{ 31, 31 },
		{ 31, 31 },
		{ 31, 31 },
		{ 31, 31 },
		{ 31, 31 },
		{ 31, 31 },
		{ 31, 31 },
		{ 31, 31 },
		{ 31, 31 },
		{ 31, 31 },
		{ 31, 31 },
		{ 31, 31 },
		{ 31, 31 },
		{ 31, 31 },
		{ 31, 31 },
		{ 31, 31 },
		{ 31, 31 },
		{ 32, 32 },
		{ 32, 32 },
		{ 32, 32 },
		{ 32, 32 },
		{ 32, 32 },
		{ 32, 32 },
		{ 32, 32 },
		{ 32, 32 },
		{ 32, 32 },
		{ 32, 32 },
		{ 36, 27 },
		{ 43, 41 },
		{ 24, 4 },
		{ 35, 26 },
		{ 40, 36 },
		{ 38, 34 },
		{ 34, 24 },
		{ 32, 32 },
		{ 32, 32 },
		{ 32, 32 },
		{ 32, 32 },
		{ 32, 32 },
		{ 32, 32 },
		{ 32, 32 },
		{ 32, 32 },
		{ 32, 32 },
		{ 32, 32 },
		{ 32, 32 },
		{ 32, 32 },
		{ 32, 32 },
		{ 32, 32 },
		{ 32, 32 },
		{ 32, 32 },
		{ 32, 32 },
		{ 32, 32 },
		{ 32, 32 },
		{ 32, 32 },
		{ 32, 32 },
		{ 32, 32 },
		{ 32, 32 },
		{ 32, 32 },
		{ 32, 32 },
		{ 32, 32 },
		{ 41, 37 },
		{ 26, 6 },
		{ 37, 28 },
		{ 25, 5 },
		{ 32, 32 },
		{ 39, 35 },
		{ 32, 32 },
		{ 32, 32 },
		{ 32, 32 },
		{ 32, 32 },
		{ 32, 32 },
		{ 32, 32 },
		{ 32, 32 },
		{ 32, 32 },
		{ 32, 32 },
		{ 32, 32 },
		{ 32, 32 },
		{ 32, 32 },
		{ 32, 32 },
		{ 32, 32 },
		{ 32, 32 },
		{ 32, 32 },
		{ 32, 32 },
		{ 32, 32 },
		{ 32, 32 },
		{ 32, 32 },
		{ 32, 32 },
		{ 32, 32 },
		{ 32, 32 },
		{ 32, 32 },
		{ 32, 32 },
		{ 32, 32 },
		{ 0, 17 },
		{ 0, 17 },
		{ 0, 17 },
		{ 0, 17 },
		{ 0, 17 },
		{ 0, 17 },
		{ 0, 17 },
		{ 0, 17 },
		{ 0, 17 },
		{ 0, 17 },
		{ 0, 19 },
		{ 0, 19 },
		{ 0, 19 },
		{ 0, 19 },
		{ 0, 19 },
		{ 0, 19 },
		{ 0, 19 },
		{ 0, 19 },
		{ 0, 19 },
		{ 0, 19 },
		{ 21, 2 },
		{ 22, 2 },
		{ 27, 7 },
		{ 18, 18 },
		{ 18, 18 },
		{ 18, 18 },
		{ 18, 18 },
		{ 18, 18 },
		{ 18, 18 },
		{ 18, 18 },
		{ 18, 18 },
		{ 18, 18 },
		{ 18, 18 },
		{ 42, 40 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 28, 7 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 21, 2 },
		{ 9, 2 },
		{ 0, 0 },
		{ 44, 2 },
		{ 17, 2 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 12, 2 },
		{ 13, 2 },
		{ 0, 0 },
		{ 10, 2 },
		{ 16, 2 },
		{ 0, 0 },
		{ 0, 0 },
		{ 20, 2 },
		{ 18, 2 },
		{ 18, 2 },
		{ 18, 2 },
		{ 18, 2 },
		{ 18, 2 },
		{ 18, 2 },
		{ 18, 2 },
		{ 18, 2 },
		{ 18, 2 },
		{ 18, 2 },
		{ 0, 0 },
		{ 11, 2 },
		{ 0, 0 },
		{ 8, 2 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 19, 2 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 6, 2 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 5, 2 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 7, 2 },
		{ 0, 0 },
		{ 0, 0 },
		{ 4, 2 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 14, 2 },
		{ 0, 0 },
		{ 15, 2 }
	};
	yytransition = transition;

	static const yystate_t YYNEARFAR YYBASED_CODE state[] = {
		{ 0, 0, 0 },
		{ 2, 1, 0 },
		{ -23, 210, 0 },
		{ 0, 0, 2 },
		{ 41, 20, 24 },
		{ 0, 68, 24 },
		{ 0, 60, 24 },
		{ 0, 124, 24 },
		{ 9, 1, 8 },
		{ 0, 2, 24 },
		{ 0, 0, 11 },
		{ 0, 0, 12 },
		{ 0, 0, 13 },
		{ 0, 0, 14 },
		{ 0, 0, 15 },
		{ 0, 0, 16 },
		{ 0, 0, 17 },
		{ 31, 151, 24 },
		{ 0, 174, 19 },
		{ 32, 161, 24 },
		{ 0, 1, 24 },
		{ 0, 1, 22 },
		{ 0, 0, 23 },
		{ 0, 0, 24 },
		{ 0, 29, 0 },
		{ 0, 0, 4 },
		{ 36, 22, 0 },
		{ 0, 17, 0 },
		{ 0, 64, 0 },
		{ 0, 0, 9 },
		{ 0, 0, 10 },
		{ 41, 1, 18 },
		{ 41, 76, 20 },
		{ -33, 1, 21 },
		{ 0, 27, 0 },
		{ 40, 71, 0 },
		{ 0, 23, 0 },
		{ 0, 57, 0 },
		{ 0, 0, 3 },
		{ 0, 0, 5 },
		{ 0, 131, 0 },
		{ 0, 19, 0 },
		{ 0, 0, 6 },
		{ 0, 0, 7 },
		{ -44, 2, 1 }
	};
	yystate = state;

	static const yybackup_t YYNEARFAR YYBASED_CODE backup[] = {
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0
	};
	yybackup = backup;
}
