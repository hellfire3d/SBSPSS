/************************************************************
yycdoutp.cpp
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include <string.h>
#include <ctype.h>
#include "clex.h"

#ifdef YYDEBUG
void yylexer::yydebugoutput(int ch) const
{
	char string[32];
	switch (ch) {
	case EOF:
		strcpy(string, "EOF");
		break;
	case '\n':
		strcpy(string, "\\n");
		break;
	case '\t':
		strcpy(string, "\\t");
		break;
	case '\v':
		strcpy(string, "\\v");
		break;
	case '\b':
		strcpy(string, "\\b");
		break;
	case '\r':
		strcpy(string, "\\r");
		break;
	case '\f':
		strcpy(string, "\\f");
		break;
	case '\a':
		strcpy(string, "\\a");
		break;
	case '\\':
		strcpy(string, "\\");
		break;
	case '\?':
		strcpy(string, "\\\?");
		break;
	case '\'':
		strcpy(string, "\\\'");
		break;
	case '\"':
		strcpy(string, "\\\"");
		break;
	default:
		if (isascii(ch) && isgraph(ch)) {
			string[0] = (char)ch;
			string[1] = '\0';
		}
		else {
			sprintf(string, "\\%03o", (unsigned int)(unsigned char)ch);
		}
		break;
	}
	yydebugoutput(string);
}

#endif
