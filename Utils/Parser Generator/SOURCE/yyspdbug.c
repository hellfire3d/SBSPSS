/************************************************************
yyspdbug.c
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include <stdio.h>
#ifdef _WIN32
#include <windows.h>
#include <winbase.h>
#endif
#include "yacc.h"

#ifdef YYDEBUG

int YYNEAR YYDCDECL yyparsedebug = 0;
int YYNEAR YYDCDECL yyparsedebugstack = 0;
int YYNEAR YYDCDECL yyparsedebugflush = 0;
#ifndef YYNINITSTREAM
FILE YYFAR *YYNEAR YYDCDECL yyparsedebugout = stdout;
#else
FILE YYFAR *YYNEAR YYDCDECL yyparsedebugout = NULL;
#endif

#ifdef YYPROTOTYPE
YYCONST char *YYCDECL yytokenstring(int token)
#else
YYCONST char *YYCDECL yytokenstring(token)
int token;
#endif
{
	YYCONST yysymbol_t YYNEARFAR *symbol;
	yyassert(token >= 0);

	symbol = yysymbol;
	while (symbol->name != NULL) {
		if (symbol->token == token) {
			return symbol->name;
		}
		symbol++;
	}
	return "illegal-token";
}

#ifdef YYPROTOTYPE
void YYCDECL yydgettoken(int token)
#else
void YYCDECL yydgettoken(token)
int token;
#endif
{
	yyassert(token >= 0);
	if (yydebug || yyparsedebug) {
		char string[128];
		YYCONST char *tokenstring = yytokenstring(token);
		yyassert(tokenstring != NULL);

		yyparsedebugoutput("get token ");
		yyparsedebugoutput(tokenstring);
		sprintf(string, " (%d)\n", (int) token);
		yyparsedebugoutput(string);
	}
}

#ifdef YYPROTOTYPE
void YYCDECL yydshift(int token)
#else
void YYCDECL yydshift(token)
int token;
#endif
{
	yyassert(token >= 0);
	if (yydebug || yyparsedebug) {
		char string[128];
		YYCONST char *tokenstring = yytokenstring(token);
		yyassert(tokenstring != NULL);

		yyparsedebugoutput("shift token ");
		yyparsedebugoutput(tokenstring);
		sprintf(string, " (%d)\n", (int) token);
		yyparsedebugoutput(string);
	}
}

#ifdef YYPROTOTYPE
void YYCDECL yydreduce(int rule)
#else
void YYCDECL yydreduce(rule)
int rule;
#endif
{
	yyassert(rule >= 0);
	if (yydebug || yyparsedebug) {
		char string[128];

		yyparsedebugoutput("reduce rule ");
		yyparsedebugoutput(yyrule[rule]);
		sprintf(string, " (%d)\n", (int) rule);
		yyparsedebugoutput(string);
	}
}

#ifdef YYPROTOTYPE
void YYCDECL yydsyntaxerror(void)
#else
void YYCDECL yydsyntaxerror()
#endif
{
	if (yydebug || yyparsedebug) {
		yyparsedebugoutput("syntax error\n");
	}
}

#ifdef YYPROTOTYPE
void YYCDECL yydaccept(void)
#else
void YYCDECL yydaccept()
#endif
{
	if (yydebug || yyparsedebug) {
		yyparsedebugoutput("accept\n");
	}
}

#ifdef YYPROTOTYPE
void YYCDECL yydabort(void)
#else
void YYCDECL yydabort()
#endif
{
	if (yydebug || yyparsedebug) {
		yyparsedebugoutput("abort\n");
	}
}

#ifdef YYPROTOTYPE
void YYCDECL yyddiscard(int token)
#else
void YYCDECL yyddiscard(token)
int token;
#endif
{
	yyassert(token >= 0);
	if (yydebug || yyparsedebug) {
		char string[128];
		YYCONST char *tokenstring = yytokenstring(token);
		yyassert(tokenstring != NULL);

		yyparsedebugoutput("discard token ");
		yyparsedebugoutput(tokenstring);
		sprintf(string, " (%d)\n", (int) token);
		yyparsedebugoutput(string);
	}
}

#ifdef YYPROTOTYPE
void YYCDECL yydexit(int exitcode)
#else
void YYCDECL yydexit(exitcode)
int exitcode;
#endif
{
	if (yydebug || yyparsedebug) {
		char string[128];

		sprintf(string, "exit with code %d\n", (int) exitcode);
		yyparsedebugoutput(string);
	}
}

#ifdef YYPROTOTYPE
void YYCDECL yydthrowerror(int errorpop)
#else
void YYCDECL yydthrowerror(errorpop)
int errorpop;
#endif
{
	if (yydebug || yyparsedebug) {
		char string[128];

		sprintf(string, "throw error and pop %d error handling state(s)\n",
			(int) errorpop);
		yyparsedebugoutput(string);
	}
}

#ifdef YYPROTOTYPE
void YYCDECL yydretire(int retirecode)
#else
void YYCDECL yydretire(retirecode)
int retirecode;
#endif
{
	if (yydebug || yyparsedebug) {
		char string[128];

		sprintf(string, "retire with code %d\n", (int) retirecode);
		yyparsedebugoutput(string);
	}
}

#ifdef YYPROTOTYPE
void YYCDECL yydattemptrecovery(void)
#else
void YYCDECL yydattemptrecovery()
#endif
{
	if (yydebug || yyparsedebug) {
		yyparsedebugoutput("attempting error recovery\n");
	}
}

#ifdef YYPROTOTYPE
void YYCDECL yyparsedebugoutput(YYCONST char *string)
#else
void YYCDECL yyparsedebugoutput(string)
YYCONST char *string;
#endif
{
	yyassert(string != NULL);

#ifdef _WIN32
	if (yyparsedebugout != NULL) {
#else
		yyassert(yyparsedebugout != NULL);
#endif
		while (*string != '\0') {
			putc(*string++, yyparsedebugout);
		}

		if (yydebugflush || yyparsedebugflush) {
			fflush(yyparsedebugout);
		}
#ifdef _WIN32
	}
	else {
		OutputDebugString(string);
	}
#endif
}

#endif
