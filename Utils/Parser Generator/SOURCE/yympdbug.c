/************************************************************
yympdbug.c
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include <stdio.h>
#ifdef _WIN32
#include <windows.h>
#include <winbase.h>
#endif
#include "myacc.h"

#ifdef YYDEBUG

#ifdef YYPROTOTYPE
YYCONST char *YYCDECL yymtokenstring(yymparse_t YYFAR *yy, int token)
#else
YYCONST char *YYCDECL yymtokenstring(yy, token)
yymparse_t YYFAR *yy;
int token;
#endif
{
	YYCONST yysymbol_t YYNEARFAR *symbol;
	yyassert(yy != NULL);
	yyassert(token >= 0);

	symbol = yy->yymsymbol;
	while (symbol->name != NULL) {
		if (symbol->token == token) {
			return symbol->name;
		}
		symbol++;
	}
	return "illegal-token";
}

#ifdef YYPROTOTYPE
void YYCDECL yymdgettoken(yymparse_t YYFAR *yy, int token)
#else
void YYCDECL yymdgettoken(yy, token)
yymparse_t YYFAR *yy;
int token;
#endif
{
	yyassert(yy != NULL);
	yyassert(token >= 0);
	if (yydebug || yy->yymdebug) {
		char string[128];
		YYCONST char *tokenstring = yymtokenstring(yy, token);
		yyassert(tokenstring != NULL);

		sprintf(string, "%p: get token ", (void *) yy);
		yymparsedebugoutput(yy, string);
		yymparsedebugoutput(yy, tokenstring);
		sprintf(string, " (%d)\n", (int) token);
		yymparsedebugoutput(yy, string);
	}
}

#ifdef YYPROTOTYPE
void YYCDECL yymdshift(yymparse_t YYFAR *yy, int token)
#else
void YYCDECL yymdshift(yy, token)
yymparse_t YYFAR *yy;
int token;
#endif
{
	yyassert(yy != NULL);
	yyassert(token >= 0);
	if (yydebug || yy->yymdebug) {
		char string[128];
		YYCONST char *tokenstring = yymtokenstring(yy, token);
		yyassert(tokenstring != NULL);

		sprintf(string, "%p: shift token ", (void *) yy);
		yymparsedebugoutput(yy, string);
		yymparsedebugoutput(yy, tokenstring);
		sprintf(string, " (%d)\n", (int) token);
		yymparsedebugoutput(yy, string);
	}
}

#ifdef YYPROTOTYPE
void YYCDECL yymdreduce(yymparse_t YYFAR *yy, int rule)
#else
void YYCDECL yymdreduce(yy, rule)
yymparse_t YYFAR *yy;
int rule;
#endif
{
	yyassert(yy != NULL);
	yyassert(rule >= 0);
	if (yydebug || yy->yymdebug) {
		char string[128];

		sprintf(string, "%p: reduce rule ", (void *) yy);
		yymparsedebugoutput(yy, string);
		yymparsedebugoutput(yy, yy->yymrule[rule]);
		sprintf(string, " (%d)\n", (int) rule);
		yymparsedebugoutput(yy, string);
	}
}

#ifdef YYPROTOTYPE
void YYCDECL yymdsyntaxerror(yymparse_t YYFAR *yy)
#else
void YYCDECL yymdsyntaxerror(yy)
yymparse_t YYFAR *yy;
#endif
{
	yyassert(yy != NULL);
	if (yydebug || yy->yymdebug) {
		char string[128];

		sprintf(string, "%p: syntax error\n", (void *) yy);
		yymparsedebugoutput(yy, string);
	}
}

#ifdef YYPROTOTYPE
void YYCDECL yymdaccept(yymparse_t YYFAR *yy)
#else
void YYCDECL yymdaccept(yy)
yymparse_t YYFAR *yy;
#endif
{
	yyassert(yy != NULL);
	if (yydebug || yy->yymdebug) {
		char string[128];

		sprintf(string, "%p: accept\n", (void *) yy);
		yymparsedebugoutput(yy, string);
	}
}

#ifdef YYPROTOTYPE
void YYCDECL yymdabort(yymparse_t YYFAR *yy)
#else
void YYCDECL yymdabort(yy)
yymparse_t YYFAR *yy;
#endif
{
	yyassert(yy != NULL);
	if (yydebug || yy->yymdebug) {
		char string[128];

		sprintf(string, "%p: abort\n", (void *) yy);
		yymparsedebugoutput(yy, string);
	}
}

#ifdef YYPROTOTYPE
void YYCDECL yymddiscard(yymparse_t YYFAR *yy, int token)
#else
void YYCDECL yymddiscard(yy, token)
yymparse_t YYFAR *yy;
int token;
#endif
{
	yyassert(yy != NULL);
	yyassert(token >= 0);
	if (yydebug || yy->yymdebug) {
		char string[128];
		YYCONST char *tokenstring = yymtokenstring(yy, token);
		yyassert(tokenstring != NULL);

		sprintf(string, "%p: discard token ", (void *) yy);
		yymparsedebugoutput(yy, string);
		yymparsedebugoutput(yy, tokenstring);
		sprintf(string, " (%d)\n", (int) token);
		yymparsedebugoutput(yy, string);
	}
}

#ifdef YYPROTOTYPE
void YYCDECL yymdexit(yymparse_t YYFAR *yy, int exitcode)
#else
void YYCDECL yymdexit(yy, exitcode)
yymparse_t YYFAR *yy;
int exitcode;
#endif
{
	yyassert(yy != NULL);
	if (yydebug || yy->yymdebug) {
		char string[128];

		sprintf(string, "%p: exit with code %d\n", (void *) yy, (int) exitcode);
		yymparsedebugoutput(yy, string);
	}
}

#ifdef YYPROTOTYPE
void YYCDECL yymdthrowerror(yymparse_t YYFAR *yy, int errorpop)
#else
void YYCDECL yymdthrowerror(yy, errorpop)
yymparse_t YYFAR *yy;
int errorpop;
#endif
{
	yyassert(yy != NULL);
	if (yydebug || yy->yymdebug) {
		char string[128];

		sprintf(string, "%p: throw error and pop %d error handling state(s)\n",
			(void *) yy, (int) errorpop);
		yymparsedebugoutput(yy, string);
	}
}

#ifdef YYPROTOTYPE
void YYCDECL yymdretire(yymparse_t YYFAR *yy, int retirecode)
#else
void YYCDECL yymdretire(yy, retirecode)
yymparse_t YYFAR *yy;
int retirecode;
#endif
{
	yyassert(yy != NULL);
	if (yydebug || yy->yymdebug) {
		char string[128];

		sprintf(string, "%p: retire with code %d\n", (void *) yy, (int) retirecode);
		yymparsedebugoutput(yy, string);
	}
}

#ifdef YYPROTOTYPE
void YYCDECL yymdattemptrecovery(yymparse_t YYFAR *yy)
#else
void YYCDECL yymdattemptrecovery(yy)
yymparse_t YYFAR *yy;
#endif
{
	yyassert(yy != NULL);
	if (yydebug || yy->yymdebug) {
		char string[128];

		sprintf(string, "%p: attempting error recovery\n", (void *) yy);
		yymparsedebugoutput(yy, string);
	}
}

#ifdef YYPROTOTYPE
void YYCDECL yymparsedebugoutput(yymparse_t YYFAR *yy, YYCONST char *string)
#else
void YYCDECL yymparsedebugoutput(yy, string)
yymparse_t YYFAR *yy;
YYCONST char *string;
#endif
{
	yyassert(yy != NULL);
	yyassert(string != NULL);

#ifdef _WIN32
	if (yy->yymdebugout != NULL) {
#else
		yyassert(yy->yymdebugout != NULL);
#endif
		while (*string != '\0') {
			putc(*string++, yy->yymdebugout);
		}

		if (yydebugflush || yy->yymdebugflush) {
			fflush(yy->yymdebugout);
		}
#ifdef _WIN32
	}
	else {
		OutputDebugString(string);
	}
#endif
}

#endif
