#include <clex.h>

#line 1 "C:\\spongebob\\Utils\\scripter\\lexer.l"

/****************************************************************************
lexer.l
ParserWizard generated Lex file.

Date: 07 December 2000
****************************************************************************/

#include "parser.h"
#include "var.h"
#include <stdlib.h>



#line 19 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
// repeated because of possible precompiled header
#include <clex.h>

#include "lexer.h"

/////////////////////////////////////////////////////////////////////////////
// constructor

YYLEXNAME::YYLEXNAME()
{
	yytables();
#line 28 "C:\\spongebob\\Utils\\scripter\\lexer.l"

	// place any extra initialisation code here

#line 35 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
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
#line 40 "C:\\spongebob\\Utils\\scripter\\lexer.l"

// extract yylval for use later on in actions
YYSTYPE& yylval = *(YYSTYPE*)yyparserptr->yylvalptr;

#line 86 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
	yyreturnflg = 1;
	switch (action) {
	case 1:
		{
#line 47 "C:\\spongebob\\Utils\\scripter\\lexer.l"
/*preprocess(yytext);*/
#line 93 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 2:
		{
#line 49 "C:\\spongebob\\Utils\\scripter\\lexer.l"
return STOP;
#line 100 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 3:
		{
#line 50 "C:\\spongebob\\Utils\\scripter\\lexer.l"
return IF;
#line 107 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 4:
		{
#line 51 "C:\\spongebob\\Utils\\scripter\\lexer.l"
return ELSE;
#line 114 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 5:
		{
#line 52 "C:\\spongebob\\Utils\\scripter\\lexer.l"
return PAUSE;
#line 121 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 6:
		{
#line 53 "C:\\spongebob\\Utils\\scripter\\lexer.l"
return PRINT;
#line 128 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 7:
		{
#line 54 "C:\\spongebob\\Utils\\scripter\\lexer.l"
return ASSIGN;
#line 135 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 8:
		{
#line 55 "C:\\spongebob\\Utils\\scripter\\lexer.l"
return EQUAL;
#line 142 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 9:
		{
#line 56 "C:\\spongebob\\Utils\\scripter\\lexer.l"
return NOTEQUAL;
#line 149 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 10:
		{
#line 57 "C:\\spongebob\\Utils\\scripter\\lexer.l"
return PLUS;
#line 156 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 11:
		{
#line 58 "C:\\spongebob\\Utils\\scripter\\lexer.l"
return END_STMT;
#line 163 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 12:
		{
#line 59 "C:\\spongebob\\Utils\\scripter\\lexer.l"
return OPEN_PAR;
#line 170 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 13:
		{
#line 60 "C:\\spongebob\\Utils\\scripter\\lexer.l"
return CLOSE_PAR;
#line 177 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 14:
		{
#line 61 "C:\\spongebob\\Utils\\scripter\\lexer.l"
return BEGIN_CS;
#line 184 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 15:
		{
#line 62 "C:\\spongebob\\Utils\\scripter\\lexer.l"
return END_CS;
#line 191 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 16:
		{
#line 63 "C:\\spongebob\\Utils\\scripter\\lexer.l"
yylval.variableIdx=lookupVarName(yytext+1);return VARIABLE;
#line 198 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 17:
		{
#line 64 "C:\\spongebob\\Utils\\scripter\\lexer.l"

#line 205 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 18:
		{
#line 65 "C:\\spongebob\\Utils\\scripter\\lexer.l"
yylval.value=atoi(yytext);return VALUE;
#line 212 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 19:
		{
#line 66 "C:\\spongebob\\Utils\\scripter\\lexer.l"

#line 219 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 20:
		{
#line 67 "C:\\spongebob\\Utils\\scripter\\lexer.l"
/*s_linesProcessed++;*/
#line 226 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 21:
		{
#line 68 "C:\\spongebob\\Utils\\scripter\\lexer.l"
printf("?\n");/*printf("line:%d\n",yylineno);printf("-%s-\n",yytext);yyerror("Unexpected character in source");*/
#line 233 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
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
#line 71 "C:\\spongebob\\Utils\\scripter\\lexer.l"



/////////////////////////////////////////////////////////////////////////////
// programs section


#line 257 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"

void YYLEXNAME::yytables()
{
	yystext_size = YYTEXT_SIZE;
	yysunput_size = YYUNPUT_SIZE;

	static const yymatch_t YYNEARFAR YYBASED_CODE match[] = {
		0
	};
	yymatch = match;

	yytransitionmax = 251;
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
		{ 18, 1 },
		{ 40, 40 },
		{ 18, 18 },
		{ 0, 29 },
		{ 0, 41 },
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
		{ 18, 1 },
		{ 40, 40 },
		{ 18, 18 },
		{ 20, 1 },
		{ 41, 40 },
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
		{ 29, 16 },
		{ 28, 28 },
		{ 28, 28 },
		{ 28, 28 },
		{ 28, 28 },
		{ 28, 28 },
		{ 28, 28 },
		{ 28, 28 },
		{ 28, 28 },
		{ 28, 28 },
		{ 28, 28 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 26, 7 },
		{ 27, 8 },
		{ 0, 0 },
		{ 0, 0 },
		{ 28, 28 },
		{ 28, 28 },
		{ 28, 28 },
		{ 28, 28 },
		{ 28, 28 },
		{ 28, 28 },
		{ 28, 28 },
		{ 28, 28 },
		{ 28, 28 },
		{ 28, 28 },
		{ 28, 28 },
		{ 28, 28 },
		{ 28, 28 },
		{ 28, 28 },
		{ 28, 28 },
		{ 28, 28 },
		{ 28, 28 },
		{ 28, 28 },
		{ 28, 28 },
		{ 28, 28 },
		{ 28, 28 },
		{ 28, 28 },
		{ 28, 28 },
		{ 28, 28 },
		{ 28, 28 },
		{ 28, 28 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 28, 28 },
		{ 0, 0 },
		{ 28, 28 },
		{ 28, 28 },
		{ 28, 28 },
		{ 28, 28 },
		{ 28, 28 },
		{ 28, 28 },
		{ 28, 28 },
		{ 28, 28 },
		{ 28, 28 },
		{ 28, 28 },
		{ 28, 28 },
		{ 28, 28 },
		{ 28, 28 },
		{ 28, 28 },
		{ 28, 28 },
		{ 28, 28 },
		{ 28, 28 },
		{ 28, 28 },
		{ 28, 28 },
		{ 28, 28 },
		{ 28, 28 },
		{ 28, 28 },
		{ 28, 28 },
		{ 28, 28 },
		{ 28, 28 },
		{ 28, 28 },
		{ 0, 15 },
		{ 0, 15 },
		{ 0, 15 },
		{ 0, 15 },
		{ 0, 15 },
		{ 0, 15 },
		{ 0, 15 },
		{ 0, 15 },
		{ 0, 15 },
		{ 0, 15 },
		{ 40, 2 },
		{ 19, 2 },
		{ 32, 24 },
		{ 39, 37 },
		{ 21, 3 },
		{ 24, 6 },
		{ 31, 23 },
		{ 36, 32 },
		{ 34, 30 },
		{ 30, 21 },
		{ 37, 33 },
		{ 23, 5 },
		{ 17, 17 },
		{ 17, 17 },
		{ 17, 17 },
		{ 17, 17 },
		{ 17, 17 },
		{ 17, 17 },
		{ 17, 17 },
		{ 17, 17 },
		{ 17, 17 },
		{ 17, 17 },
		{ 25, 6 },
		{ 40, 2 },
		{ 8, 2 },
		{ 33, 25 },
		{ 41, 2 },
		{ 15, 2 },
		{ 22, 4 },
		{ 35, 31 },
		{ 38, 36 },
		{ 11, 2 },
		{ 12, 2 },
		{ 0, 0 },
		{ 9, 2 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 16, 2 },
		{ 17, 2 },
		{ 17, 2 },
		{ 17, 2 },
		{ 17, 2 },
		{ 17, 2 },
		{ 17, 2 },
		{ 17, 2 },
		{ 17, 2 },
		{ 17, 2 },
		{ 17, 2 },
		{ 0, 0 },
		{ 10, 2 },
		{ 0, 0 },
		{ 7, 2 },
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
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 5, 2 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 4, 2 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 6, 2 },
		{ 0, 0 },
		{ 0, 0 },
		{ 3, 2 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 13, 2 },
		{ 0, 0 },
		{ 14, 2 }
	};
	yytransition = transition;

	static const yystate_t YYNEARFAR YYBASED_CODE state[] = {
		{ 0, 0, 0 },
		{ 2, 1, 0 },
		{ -20, 125, 0 },
		{ 37, 22, 21 },
		{ 0, 60, 21 },
		{ 0, 37, 21 },
		{ 0, 42, 21 },
		{ 8, 1, 7 },
		{ 0, 2, 21 },
		{ 0, 0, 10 },
		{ 0, 0, 11 },
		{ 0, 0, 12 },
		{ 0, 0, 13 },
		{ 0, 0, 14 },
		{ 0, 0, 15 },
		{ 28, 76, 21 },
		{ 0, 1, 21 },
		{ 0, 98, 18 },
		{ 0, 3, 19 },
		{ 0, 0, 20 },
		{ 0, 0, 21 },
		{ 0, 32, 0 },
		{ 0, 0, 3 },
		{ 32, 25, 0 },
		{ 0, 19, 0 },
		{ 0, 54, 0 },
		{ 0, 0, 8 },
		{ 0, 0, 9 },
		{ 37, 1, 16 },
		{ -29, 3, 17 },
		{ 0, 30, 0 },
		{ 36, 62, 0 },
		{ 0, 26, 0 },
		{ 0, 34, 0 },
		{ 0, 0, 2 },
		{ 0, 0, 4 },
		{ 0, 63, 0 },
		{ 0, 21, 0 },
		{ 0, 0, 5 },
		{ 0, 0, 6 },
		{ 18, 2, 19 },
		{ -41, 4, 1 }
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
		0
	};
	yybackup = backup;
}
