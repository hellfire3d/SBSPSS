/************************************************************
yysclex.c
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <limits.h>
#include "lex.h"

#ifdef YYPROTOTYPE
int YYCDECL yyclex(void)
#else
int YYCDECL yyclex()
#endif
{
	while (1) {
		int oldleng;
		int state = 1 + yystart;
		if (yyeol) {
			state++;
		}
	
		/* yymore */
		if (yymoreflg) {
			yymoreflg = 0;		/* clear flag */
		}
		else {
			yyleng = 0;
			yyoldeol = yyeol;
		}
		oldleng = yyleng;

		/* look for a string */
		do {
			int index;
			int ch;		/* lookahead character */
			
			/* get input character */
			ch = yyinput();
			yyassert(ch >= 0 && ch <= UCHAR_MAX || ch == EOF);
			if (ch == EOF) {
				break;
			}
			
			/* check for possible overflow */
			if (yyleng == yytext_size) {
				do {
					if (yytextgrow) {
						if (yytext_size != 0) {
							int size = yytext_size * 2;
							if (size / 2 == yytext_size) {		/* overflow check */
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

			/* look for a transition */
			index = yystate[state].base;
			while (1) {
				if (yyctransition[index].next == 0) {
					state = yystate[state].def;
					if (state <= 0) {
						if (state < 0) {
							if (ch >= 0 && ch <= 0xff) {
								state = -state;
							}
							else {
								state = 0;
							}
						}
						break;
					}
				}
				if (ch >= yyctransition[index].first &&
					ch <= yyctransition[index].last) {
					state = yyctransition[index].next;
					break;
				}
				index++;
			}

			{
				int leng = yyleng;		/* slightly more efficient */
				yytext[leng] = (char) ch;
				yystatebuf[leng] = state;
				leng++;
				yyleng = leng;
			}
		}
		while (state != 0 && (yystate[state].def != 0 || yystate[state].base != 0));

		/* now find a match */
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
					int rejectleng;
					int token;
					/* check for backup */
					if (yybackup[match]) {
						while (yyleng > oldleng) {
							int index = yystate[yystatebuf[yyleng - 1]].match;
							if (index < 0) {
								if (yyback(&yymatch[-index], -match)) {
									break;	/* found an expression */
								}
							}
							yyleng--;
							yyunput((unsigned char) yytext[yyleng]);
						}
					}
					yytext[yyleng] = '\0';
#ifdef YYDEBUG
					yydmatch(match);
#endif
					yyrejectflg = 0;		/* clear flag */
					rejectleng = yyleng;

					if (yyleng > 0) {
						yyeol = (unsigned char) (yytext[yyleng - 1] == '\n');
					}
					else {
						yyeol = yyoldeol;
					}

					/* perform user action */
					token = yylexaction(match);

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
					yyunput((unsigned char) yytext[yyleng]);
				}
				else {
					yyeol = (unsigned char) (yytext[0] == '\n');
					yyoutput(yytext[0]);	/* non-matched character */
					break;
				}
			}
		}
		else {
			int index;
			int match;
			yyassert(yyleng == oldleng);

			/* handles <<EOF>> rules */
			index = 0;
			match = yystate[state].match;
			if (match < 0) {
				index = -match;
				match = yymatch[index++];
			}
			while (match > 0) {
				int token;
				yytext[yyleng] = '\0';
#ifdef YYDEBUG
				yydmatch(match);
#endif
				yyrejectflg = 0;		/* clear flag */

				/* perform user action */
				token = yylexaction(match);

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
				return 0;			/* eof reached */
			}
		}
	}
}
