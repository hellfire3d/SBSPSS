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
#line 47 "C:\\spongebob\\Utils\\scripter\\lexer.l"

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
#line 59 "C:\\spongebob\\Utils\\scripter\\lexer.l"

// extract yylval for use later on in actions
YYSTYPE& yylval = *(YYSTYPE*)yyparserptr->yylvalptr;

#line 88 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
	yyreturnflg = 1;
	switch (action) {
	case 1:
		{
#line 66 "C:\\spongebob\\Utils\\scripter\\lexer.l"
if(preprocessorCmd(yytext+1)!=(int)true)error();
#line 95 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 2:
		{
#line 67 "C:\\spongebob\\Utils\\scripter\\lexer.l"
printf("# commands must be at start of line!\n");error();
#line 102 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 3:
		{
#line 69 "C:\\spongebob\\Utils\\scripter\\lexer.l"
return STOP;
#line 109 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 4:
		{
#line 70 "C:\\spongebob\\Utils\\scripter\\lexer.l"
return IF;
#line 116 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 5:
		{
#line 71 "C:\\spongebob\\Utils\\scripter\\lexer.l"
return ELSE;
#line 123 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 6:
		{
#line 72 "C:\\spongebob\\Utils\\scripter\\lexer.l"
return WHILE;
#line 130 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 7:
		{
#line 73 "C:\\spongebob\\Utils\\scripter\\lexer.l"
return DO;
#line 137 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 8:
		{
#line 74 "C:\\spongebob\\Utils\\scripter\\lexer.l"
return PAUSE;
#line 144 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 9:
		{
#line 75 "C:\\spongebob\\Utils\\scripter\\lexer.l"
return PRINT;
#line 151 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 10:
		{
#line 76 "C:\\spongebob\\Utils\\scripter\\lexer.l"
return ASSIGN;
#line 158 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 11:
		{
#line 77 "C:\\spongebob\\Utils\\scripter\\lexer.l"
return EQUAL;
#line 165 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 12:
		{
#line 78 "C:\\spongebob\\Utils\\scripter\\lexer.l"
return NOTEQUAL;
#line 172 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 13:
		{
#line 79 "C:\\spongebob\\Utils\\scripter\\lexer.l"
return LESSTHAN;
#line 179 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 14:
		{
#line 80 "C:\\spongebob\\Utils\\scripter\\lexer.l"
return GREATERTHAN;
#line 186 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 15:
		{
#line 81 "C:\\spongebob\\Utils\\scripter\\lexer.l"
return PLUS;
#line 193 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 16:
		{
#line 82 "C:\\spongebob\\Utils\\scripter\\lexer.l"
return SUBTRACT;
#line 200 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 17:
		{
#line 83 "C:\\spongebob\\Utils\\scripter\\lexer.l"
return MULTIPLY;
#line 207 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 18:
		{
#line 84 "C:\\spongebob\\Utils\\scripter\\lexer.l"
return DIVIDE;
#line 214 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 19:
		{
#line 85 "C:\\spongebob\\Utils\\scripter\\lexer.l"
return END_STMT;
#line 221 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 20:
		{
#line 86 "C:\\spongebob\\Utils\\scripter\\lexer.l"
return OPEN_PAR;
#line 228 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 21:
		{
#line 87 "C:\\spongebob\\Utils\\scripter\\lexer.l"
return CLOSE_PAR;
#line 235 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 22:
		{
#line 88 "C:\\spongebob\\Utils\\scripter\\lexer.l"
return BEGIN_CS;
#line 242 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 23:
		{
#line 89 "C:\\spongebob\\Utils\\scripter\\lexer.l"
return END_CS;
#line 249 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 24:
		{
#line 90 "C:\\spongebob\\Utils\\scripter\\lexer.l"
return COMMA;
#line 256 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 25:
		{
#line 93 "C:\\spongebob\\Utils\\scripter\\lexer.l"
yylval.variableIdx=lookupVarName(yytext+1);return VARIABLE;
#line 263 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 26:
		{
#line 94 "C:\\spongebob\\Utils\\scripter\\lexer.l"
yylval.value=atoi(yytext);return VALUE;
#line 270 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 27:
		{
#line 97 "C:\\spongebob\\Utils\\scripter\\lexer.l"
yylval.functionNumber=lookupFunctionName(yytext+1);return FUNCTION;
#line 277 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 28:
		{
#line 99 "C:\\spongebob\\Utils\\scripter\\lexer.l"

#line 284 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 29:
		{
#line 100 "C:\\spongebob\\Utils\\scripter\\lexer.l"
comment();
#line 291 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 30:
		{
#line 101 "C:\\spongebob\\Utils\\scripter\\lexer.l"

#line 298 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 31:
		{
#line 102 "C:\\spongebob\\Utils\\scripter\\lexer.l"

#line 305 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 32:
		{
#line 104 "C:\\spongebob\\Utils\\scripter\\lexer.l"
unexpectedChar();
#line 312 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
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
#line 106 "C:\\spongebob\\Utils\\scripter\\lexer.l"



/////////////////////////////////////////////////////////////////////////////
// programs section


#line 336 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"

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
		{ 27, 27 },
		{ 0, 39 },
		{ 0, 56 },
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
		{ 27, 27 },
		{ 0, 0 },
		{ 0, 0 },
		{ 3, 1 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 40, 17 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 39, 17 },
		{ 41, 41 },
		{ 41, 41 },
		{ 41, 41 },
		{ 41, 41 },
		{ 41, 41 },
		{ 41, 41 },
		{ 41, 41 },
		{ 41, 41 },
		{ 41, 41 },
		{ 41, 41 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 38, 11 },
		{ 37, 10 },
		{ 0, 0 },
		{ 0, 0 },
		{ 41, 41 },
		{ 41, 41 },
		{ 41, 41 },
		{ 41, 41 },
		{ 41, 41 },
		{ 41, 41 },
		{ 41, 41 },
		{ 41, 41 },
		{ 41, 41 },
		{ 41, 41 },
		{ 41, 41 },
		{ 41, 41 },
		{ 41, 41 },
		{ 41, 41 },
		{ 41, 41 },
		{ 41, 41 },
		{ 41, 41 },
		{ 41, 41 },
		{ 41, 41 },
		{ 41, 41 },
		{ 41, 41 },
		{ 41, 41 },
		{ 41, 41 },
		{ 41, 41 },
		{ 41, 41 },
		{ 41, 41 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 41, 41 },
		{ 0, 0 },
		{ 41, 41 },
		{ 41, 41 },
		{ 41, 41 },
		{ 41, 41 },
		{ 41, 41 },
		{ 41, 41 },
		{ 41, 41 },
		{ 41, 41 },
		{ 41, 41 },
		{ 41, 41 },
		{ 41, 41 },
		{ 41, 41 },
		{ 41, 41 },
		{ 41, 41 },
		{ 41, 41 },
		{ 41, 41 },
		{ 41, 41 },
		{ 41, 41 },
		{ 41, 41 },
		{ 41, 41 },
		{ 41, 41 },
		{ 41, 41 },
		{ 41, 41 },
		{ 41, 41 },
		{ 41, 41 },
		{ 41, 41 },
		{ 42, 42 },
		{ 42, 42 },
		{ 42, 42 },
		{ 42, 42 },
		{ 42, 42 },
		{ 42, 42 },
		{ 42, 42 },
		{ 42, 42 },
		{ 42, 42 },
		{ 42, 42 },
		{ 46, 35 },
		{ 55, 52 },
		{ 30, 4 },
		{ 44, 32 },
		{ 51, 46 },
		{ 48, 43 },
		{ 34, 8 },
		{ 42, 42 },
		{ 42, 42 },
		{ 42, 42 },
		{ 42, 42 },
		{ 42, 42 },
		{ 42, 42 },
		{ 42, 42 },
		{ 42, 42 },
		{ 42, 42 },
		{ 42, 42 },
		{ 42, 42 },
		{ 42, 42 },
		{ 42, 42 },
		{ 42, 42 },
		{ 42, 42 },
		{ 42, 42 },
		{ 42, 42 },
		{ 42, 42 },
		{ 42, 42 },
		{ 42, 42 },
		{ 42, 42 },
		{ 42, 42 },
		{ 42, 42 },
		{ 42, 42 },
		{ 42, 42 },
		{ 42, 42 },
		{ 43, 30 },
		{ 52, 47 },
		{ 32, 6 },
		{ 50, 45 },
		{ 42, 42 },
		{ 45, 33 },
		{ 42, 42 },
		{ 42, 42 },
		{ 42, 42 },
		{ 42, 42 },
		{ 42, 42 },
		{ 42, 42 },
		{ 42, 42 },
		{ 42, 42 },
		{ 42, 42 },
		{ 42, 42 },
		{ 42, 42 },
		{ 42, 42 },
		{ 42, 42 },
		{ 42, 42 },
		{ 42, 42 },
		{ 42, 42 },
		{ 42, 42 },
		{ 42, 42 },
		{ 42, 42 },
		{ 42, 42 },
		{ 42, 42 },
		{ 42, 42 },
		{ 42, 42 },
		{ 42, 42 },
		{ 42, 42 },
		{ 42, 42 },
		{ 0, 24 },
		{ 0, 24 },
		{ 0, 24 },
		{ 0, 24 },
		{ 0, 24 },
		{ 0, 24 },
		{ 0, 24 },
		{ 0, 24 },
		{ 0, 24 },
		{ 0, 24 },
		{ 0, 26 },
		{ 0, 26 },
		{ 0, 26 },
		{ 0, 26 },
		{ 0, 26 },
		{ 0, 26 },
		{ 0, 26 },
		{ 0, 26 },
		{ 0, 26 },
		{ 0, 26 },
		{ 27, 2 },
		{ 28, 2 },
		{ 35, 9 },
		{ 25, 25 },
		{ 25, 25 },
		{ 25, 25 },
		{ 25, 25 },
		{ 25, 25 },
		{ 25, 25 },
		{ 25, 25 },
		{ 25, 25 },
		{ 25, 25 },
		{ 25, 25 },
		{ 47, 36 },
		{ 33, 7 },
		{ 31, 5 },
		{ 54, 51 },
		{ 49, 44 },
		{ 53, 50 },
		{ 36, 9 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 27, 2 },
		{ 11, 2 },
		{ 0, 0 },
		{ 56, 2 },
		{ 24, 2 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 19, 2 },
		{ 20, 2 },
		{ 16, 2 },
		{ 14, 2 },
		{ 23, 2 },
		{ 15, 2 },
		{ 0, 0 },
		{ 17, 2 },
		{ 25, 2 },
		{ 25, 2 },
		{ 25, 2 },
		{ 25, 2 },
		{ 25, 2 },
		{ 25, 2 },
		{ 25, 2 },
		{ 25, 2 },
		{ 25, 2 },
		{ 25, 2 },
		{ 0, 0 },
		{ 18, 2 },
		{ 12, 2 },
		{ 10, 2 },
		{ 13, 2 },
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
		{ 26, 2 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 8, 2 },
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
		{ 9, 2 },
		{ 0, 0 },
		{ 0, 0 },
		{ 4, 2 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 7, 2 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 21, 2 },
		{ 0, 0 },
		{ 22, 2 }
	};
	yytransition = transition;

	static const yystate_t YYNEARFAR YYBASED_CODE state[] = {
		{ 0, 0, 0 },
		{ 2, 1, 0 },
		{ -29, 210, 0 },
		{ 0, 0, 2 },
		{ 52, 20, 32 },
		{ 0, 132, 32 },
		{ 45, 61, 32 },
		{ 0, 129, 32 },
		{ 30, 29, 32 },
		{ 0, 124, 32 },
		{ 11, 2, 10 },
		{ 0, 1, 32 },
		{ 0, 0, 13 },
		{ 0, 0, 14 },
		{ 0, 0, 15 },
		{ 0, 0, 16 },
		{ 0, 0, 17 },
		{ 0, 1, 18 },
		{ 0, 0, 19 },
		{ 0, 0, 20 },
		{ 0, 0, 21 },
		{ 0, 0, 22 },
		{ 0, 0, 23 },
		{ 0, 0, 24 },
		{ 41, 151, 32 },
		{ 0, 174, 26 },
		{ 42, 161, 32 },
		{ 0, 1, 30 },
		{ 0, 0, 31 },
		{ 0, 0, 32 },
		{ 0, 56, 0 },
		{ 0, 0, 4 },
		{ 46, 22, 0 },
		{ 36, 67, 0 },
		{ 0, 0, 7 },
		{ 0, 17, 0 },
		{ 0, 127, 0 },
		{ 0, 0, 11 },
		{ 0, 0, 12 },
		{ -39, 1, 28 },
		{ 0, 0, 29 },
		{ 52, 1, 25 },
		{ 52, 76, 27 },
		{ 0, 27, 0 },
		{ 51, 135, 0 },
		{ 0, 62, 0 },
		{ 0, 23, 0 },
		{ 0, 58, 0 },
		{ 0, 0, 3 },
		{ 0, 0, 5 },
		{ 51, 136, 0 },
		{ 0, 134, 0 },
		{ 0, 19, 0 },
		{ 0, 0, 6 },
		{ 0, 0, 8 },
		{ 0, 0, 9 },
		{ -56, 2, 1 }
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
