#include <clex.h>

#line 1 "C:\\spongebob\\Utils\\scripter\\lexer.l"

/****************************************************************************
lexer.l
ParserWizard generated Lex file.

Date: 07 December 2000
****************************************************************************/

#include "parser.h"
#include "var.h"
#include "prepro.h"
#include <stdlib.h>



#line 20 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
// repeated because of possible precompiled header
#include <clex.h>

#include "lexer.h"

/////////////////////////////////////////////////////////////////////////////
// constructor

YYLEXNAME::YYLEXNAME()
{
	yytables();
#line 51 "C:\\spongebob\\Utils\\scripter\\lexer.l"

	// place any extra initialisation code here

#line 36 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
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
#line 63 "C:\\spongebob\\Utils\\scripter\\lexer.l"

// extract yylval for use later on in actions
YYSTYPE& yylval = *(YYSTYPE*)yyparserptr->yylvalptr;

#line 87 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
	yyreturnflg = 1;
	switch (action) {
	case 1:
		{
#line 70 "C:\\spongebob\\Utils\\scripter\\lexer.l"
if(preprocessorCmd(yytext+1)!=true)error();
#line 94 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 2:
		{
#line 71 "C:\\spongebob\\Utils\\scripter\\lexer.l"
printf("# commands must be at start of line! ( line %d )\n",getCurrentLine());error();
#line 101 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 3:
		{
#line 73 "C:\\spongebob\\Utils\\scripter\\lexer.l"
return STOP;
#line 108 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 4:
		{
#line 74 "C:\\spongebob\\Utils\\scripter\\lexer.l"
return IF;
#line 115 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 5:
		{
#line 75 "C:\\spongebob\\Utils\\scripter\\lexer.l"
return ELSE;
#line 122 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 6:
		{
#line 76 "C:\\spongebob\\Utils\\scripter\\lexer.l"
return PAUSE;
#line 129 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 7:
		{
#line 77 "C:\\spongebob\\Utils\\scripter\\lexer.l"
return PRINT;
#line 136 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 8:
		{
#line 78 "C:\\spongebob\\Utils\\scripter\\lexer.l"
return ASSIGN;
#line 143 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 9:
		{
#line 79 "C:\\spongebob\\Utils\\scripter\\lexer.l"
return EQUAL;
#line 150 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 10:
		{
#line 80 "C:\\spongebob\\Utils\\scripter\\lexer.l"
return NOTEQUAL;
#line 157 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 11:
		{
#line 81 "C:\\spongebob\\Utils\\scripter\\lexer.l"
return PLUS;
#line 164 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 12:
		{
#line 82 "C:\\spongebob\\Utils\\scripter\\lexer.l"
return END_STMT;
#line 171 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 13:
		{
#line 83 "C:\\spongebob\\Utils\\scripter\\lexer.l"
return OPEN_PAR;
#line 178 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 14:
		{
#line 84 "C:\\spongebob\\Utils\\scripter\\lexer.l"
return CLOSE_PAR;
#line 185 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 15:
		{
#line 85 "C:\\spongebob\\Utils\\scripter\\lexer.l"
return BEGIN_CS;
#line 192 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 16:
		{
#line 86 "C:\\spongebob\\Utils\\scripter\\lexer.l"
return END_CS;
#line 199 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 17:
		{
#line 88 "C:\\spongebob\\Utils\\scripter\\lexer.l"
yylval.variableIdx=lookupVarName(yytext+1);return VARIABLE;
#line 206 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 18:
		{
#line 89 "C:\\spongebob\\Utils\\scripter\\lexer.l"
yylval.value=atoi(yytext);return VALUE;
#line 213 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 19:
		{
#line 92 "C:\\spongebob\\Utils\\scripter\\lexer.l"

#line 220 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 20:
		{
#line 93 "C:\\spongebob\\Utils\\scripter\\lexer.l"

#line 227 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 21:
		{
#line 94 "C:\\spongebob\\Utils\\scripter\\lexer.l"

#line 234 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
		}
		break;
	case 22:
		{
#line 96 "C:\\spongebob\\Utils\\scripter\\lexer.l"
printf("UNEXPECTED CHAR: '%s' in line %d ( char %d )\n",yytext,getCurrentLine(),getCurrentCharOnLine());error();
#line 241 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"
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
#line 98 "C:\\spongebob\\Utils\\scripter\\lexer.l"



/////////////////////////////////////////////////////////////////////////////
// programs section


#line 265 "C:\\spongebob\\Utils\\scripter\\lexer.cpp"

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
		{ 19, 19 },
		{ 0, 30 },
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
		{ 0, 0 },
		{ 0, 0 },
		{ 19, 19 },
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
		{ 30, 18 },
		{ 29, 29 },
		{ 29, 29 },
		{ 29, 29 },
		{ 29, 29 },
		{ 29, 29 },
		{ 29, 29 },
		{ 29, 29 },
		{ 29, 29 },
		{ 29, 29 },
		{ 29, 29 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 27, 8 },
		{ 28, 9 },
		{ 0, 0 },
		{ 0, 0 },
		{ 29, 29 },
		{ 29, 29 },
		{ 29, 29 },
		{ 29, 29 },
		{ 29, 29 },
		{ 29, 29 },
		{ 29, 29 },
		{ 29, 29 },
		{ 29, 29 },
		{ 29, 29 },
		{ 29, 29 },
		{ 29, 29 },
		{ 29, 29 },
		{ 29, 29 },
		{ 29, 29 },
		{ 29, 29 },
		{ 29, 29 },
		{ 29, 29 },
		{ 29, 29 },
		{ 29, 29 },
		{ 29, 29 },
		{ 29, 29 },
		{ 29, 29 },
		{ 29, 29 },
		{ 29, 29 },
		{ 29, 29 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 29, 29 },
		{ 0, 0 },
		{ 29, 29 },
		{ 29, 29 },
		{ 29, 29 },
		{ 29, 29 },
		{ 29, 29 },
		{ 29, 29 },
		{ 29, 29 },
		{ 29, 29 },
		{ 29, 29 },
		{ 29, 29 },
		{ 29, 29 },
		{ 29, 29 },
		{ 29, 29 },
		{ 29, 29 },
		{ 29, 29 },
		{ 29, 29 },
		{ 29, 29 },
		{ 29, 29 },
		{ 29, 29 },
		{ 29, 29 },
		{ 29, 29 },
		{ 29, 29 },
		{ 29, 29 },
		{ 29, 29 },
		{ 29, 29 },
		{ 29, 29 },
		{ 0, 16 },
		{ 0, 16 },
		{ 0, 16 },
		{ 0, 16 },
		{ 0, 16 },
		{ 0, 16 },
		{ 0, 16 },
		{ 0, 16 },
		{ 0, 16 },
		{ 0, 16 },
		{ 19, 2 },
		{ 20, 2 },
		{ 33, 25 },
		{ 40, 38 },
		{ 22, 4 },
		{ 25, 7 },
		{ 32, 24 },
		{ 37, 33 },
		{ 35, 31 },
		{ 31, 22 },
		{ 38, 34 },
		{ 24, 6 },
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
		{ 26, 7 },
		{ 19, 2 },
		{ 9, 2 },
		{ 34, 26 },
		{ 41, 2 },
		{ 16, 2 },
		{ 23, 5 },
		{ 36, 32 },
		{ 39, 37 },
		{ 12, 2 },
		{ 13, 2 },
		{ 0, 0 },
		{ 10, 2 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 18, 2 },
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
		{ 0, 0 },
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
		{ -21, 125, 0 },
		{ 0, 0, 2 },
		{ 38, 22, 22 },
		{ 0, 60, 22 },
		{ 0, 37, 22 },
		{ 0, 42, 22 },
		{ 9, 1, 8 },
		{ 0, 2, 22 },
		{ 0, 0, 11 },
		{ 0, 0, 12 },
		{ 0, 0, 13 },
		{ 0, 0, 14 },
		{ 0, 0, 15 },
		{ 0, 0, 16 },
		{ 29, 76, 22 },
		{ 0, 98, 18 },
		{ 0, 1, 22 },
		{ 0, 1, 20 },
		{ 0, 0, 21 },
		{ 0, 0, 22 },
		{ 0, 32, 0 },
		{ 0, 0, 4 },
		{ 33, 25, 0 },
		{ 0, 19, 0 },
		{ 0, 54, 0 },
		{ 0, 0, 9 },
		{ 0, 0, 10 },
		{ 38, 1, 17 },
		{ -30, 1, 19 },
		{ 0, 30, 0 },
		{ 37, 62, 0 },
		{ 0, 26, 0 },
		{ 0, 34, 0 },
		{ 0, 0, 3 },
		{ 0, 0, 5 },
		{ 0, 63, 0 },
		{ 0, 21, 0 },
		{ 0, 0, 6 },
		{ 0, 0, 7 },
		{ -41, 2, 1 }
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
		0
	};
	yybackup = backup;
}
