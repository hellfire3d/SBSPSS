/************************************************************
yycflex.cpp
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <limits.h>
#include "clex.h"

int yyflexer::yylex()
{
	while (1) {
		int state = 1 + yystart;
		if (yyeol) {
			state++;
		}
	
		// yymore
		if (yymoreflg) {
			yymoreflg = 0;		// clear flag
		}
		else {
			yyleng = 0;
			yyoldeol = yyeol;
		}
		int oldleng = yyleng;

		// look for a string
		do {
			// get input character (look ahead character)
			int ch = yyinput();
			yyassert(ch >= 0 && ch <= UCHAR_MAX || ch == EOF);
			if (ch == EOF) {
				break;
			}
			
			// check for possible overflow
			if (yyleng == yytext_size) {
				do {
					if (yytextgrow) {
						if (yytext_size != 0) {
							int size = yytext_size * 2;
							if (size / 2 == yytext_size) {		// overflow check
								if (yysettextsize(size)) {
									break;
								}
							}
						}
						else {
							if (yysettextsize(100)) {
								break;
							}
						}
					}
					yytextoverflow();
					exit(EXIT_FAILURE);
				}
				while (0);
			}

			// look for a transition
			do {
				int index = yystate[state].base + ch;
				if (index >= 0 && index < yytransitionmax) {
					if (yytransition[index].check == state) {
						state = yytransition[index].next;
						break;	// found a transition
					}
				}
				state = yystate[state].def;
				if (state < 0) {
					if (ch >= 0 && ch <= 0xff) {
						state = -state;
					}
					else {
						state = 0;
					}
					break;		// default transition
				}
			}
			while (state != 0);

			int leng = yyleng;		// slightly more efficient
			yytext[leng] = (char)ch;
			yystatebuf[leng] = state;
			leng++;
			yyleng = leng;
		}
		while (state != 0 && (yystate[state].def != 0 || yystate[state].base != 0));

		// now find a match
		if (yyleng > oldleng) {
			int rejectmatch = 0;
			while (1) {
				int match = yystate[yystatebuf[yyleng - 1]].match;
				if (rejectmatch != 0) {
					if (match < 0) {
						int index = -match;
						do {
							match = yymatch[index++];
						}
						while (match > 0 && match <= rejectmatch);
					}
					else {
						if (match == rejectmatch) {
							match = 0;
						}
					}
					rejectmatch = 0;
				}
				else {
					if (match < 0) {
						match = yymatch[-match];
					}
				}
				if (match > 0) {
					// check for backup
					if (yybackup[match]) {
						while (yyleng > oldleng) {
							int index = yystate[yystatebuf[yyleng - 1]].match;
							if (index < 0) {
								if (yyback(&yymatch[-index], -match)) {
									break;	// found an expression
								}
							}
							yyleng--;
							yyunput((unsigned char)yytext[yyleng]);
						}
					}
					yytext[yyleng] = '\0';
#ifdef YYDEBUG
					yydmatch(match);
#endif
					yyrejectflg = 0;		// clear flag
					int rejectleng = yyleng;

					if (yyleng > 0) {
						yyeol = (unsigned char)(yytext[yyleng - 1] == '\n');
					}
					else {
						yyeol = yyoldeol;
					}

					// perform user action
					int token = yyaction(match);

					if (yyreturnflg) {
						return token;
					}
					if (!yyrejectflg) {
						break;
					}
					if (rejectleng == yyleng) {
						rejectmatch = match;
					}
				}
				else if (yyleng > oldleng + 1) {
					yyleng--;
					yyunput((unsigned char)yytext[yyleng]);
				}
				else {
					yyeol = (unsigned char)(yytext[0] == '\n');
					yyoutput(yytext[0]);	// non-matched character
					break;
				}
			}
		}
		else {
			yyassert(yyleng == oldleng);

			// handles <<EOF>> rules
			int index = 0;
			int match = yystate[state].match;
			if (match < 0) {
				index = -match;
				match = yymatch[index++];
			}
			while (match > 0) {
				yytext[yyleng] = '\0';
#ifdef YYDEBUG
				yydmatch(match);
#endif
				yyrejectflg = 0;		// clear flag

				// perform user action
				int token = yyaction(match);

				if (yyreturnflg) {
					return token;
				}
				if (!yyrejectflg) {
					break;
				}

				if (index == 0) {
					break;
				}
				match = yymatch[index++];
			}

			if (yywrap()) {
				yyoldeol = 1;
				yyeol = 1;
				yystart = 0;
				return 0;			// eof reached
			}
		}
	}
}
