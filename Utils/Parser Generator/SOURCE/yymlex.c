/************************************************************
yymlex.c
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <limits.h>
#include "mlex.h"

#ifdef YYPROTOTYPE
int YYCDECL yymlex(yymlex_t YYFAR *yy)
#else
int YYCDECL yymlex(yy)
yymlex_t YYFAR *yy;
#endif
{
	yyassert(yy != NULL);
	yyassert(yymisfastlexer(yy));	/* make sure it's a fast lexical analyser */
	
	while (1) {
		int oldleng;
		int state = 1 + yy->yymstart;
		if (yy->yymeol) {
			state++;
		}
	
		/* yymore */
		if (yy->yymmoreflg) {
			yy->yymmoreflg = 0;		/* clear flag */
		}
		else {
			yy->yymleng = 0;
			yy->yymoldeol = yy->yymeol;
		}
		oldleng = yy->yymleng;

		/* look for a string */
		do {
			int ch;		/* lookahead character */

			/* get input character */
			ch = (*yy->yyminput)(yy);
			yyassert(ch >= 0 && ch <= UCHAR_MAX || ch == EOF);
			if (ch == EOF) {
				break;
			}
			
			/* check for possible overflow */
			if (yy->yymleng == yy->yymtext_size) {
				do {
					if (yy->yymtextgrow) {
						if (yy->yymtext_size != 0) {
							int size = yy->yymtext_size * 2;
							if (size / 2 == yy->yymtext_size) {		/* overflow check */
								if (yymsettextsize(yy, size)) {
									break;
								}
							}
						}
						else {
							if (yymsettextsize(yy, 100)) {
								break;
							}
						}
					}
					(*yy->yymtextoverflow)(yy);
					exit(EXIT_FAILURE);
				}
				while (0);
			}

			/* handles eof condition automatically */
			while (state != 0) {
				int index = yy->yymstate[state].base + ch;
				if (index >= 0 && index < yy->yymtransitionmax) {
					if (yy->yymtransition[index].check == state) {
						state = yy->yymtransition[index].next;
						break;	/* found a transition */
					}
				}
				state = yy->yymstate[state].def;
				if (state < 0) {
					if (ch >= 0 && ch <= 0xff) {
						state = -state;
					}
					else {
						state = 0;
					}
					break;
				}
			}

			{
				int leng = yy->yymleng;		/* slightly more efficient */
				yy->yymtext[leng] = (char) ch;
				yy->yymstatebuf[leng] = state;
				leng++;
				yy->yymleng = leng;
			}
		}
		while (state != 0 && (yy->yymstate[state].def != 0 || yy->yymstate[state].base != 0));

		/* now find a match */
		if (yy->yymleng > oldleng) {
			int rejectmatch = 0;
			while (1) {
				int match = yy->yymstate[yy->yymstatebuf[yy->yymleng - 1]].match;
				if (rejectmatch != 0) {
					if (match < 0) {
						int index = -match;
						do {
							match = yy->yymmatch[index++];
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
						match = yy->yymmatch[-match];
					}
				}
				if (match > 0) {
					int rejectleng;
					int token;
					/* check for backup */
					if (yy->yymbackup[match]) {
						while (yy->yymleng > oldleng) {
							int index = yy->yymstate[yy->yymstatebuf[yy->yymleng - 1]].match;
							if (index < 0) {
								if (yymback(&yy->yymmatch[-index], -match)) {
									break;	/* found an expression */
								}
							}
							yy->yymleng--;
							(*yy->yymunput)(yy, (unsigned char) yy->yymtext[yy->yymleng]);
						}
					}
					yy->yymtext[yy->yymleng] = '\0';
#ifdef YYDEBUG
					yymdmatch(yy, match);
#endif
					yy->yymrejectflg = 0;
					rejectleng = yy->yymleng;

					if (yy->yymleng > 0) {
						yy->yymeol = (unsigned char) (yy->yymtext[yy->yymleng - 1] == '\n');
					}
					else {
						yy->yymeol = yy->yymoldeol;
					}

					/* perform user action */
					token = (*yy->yymlexaction)(yy, match);

					if (yy->yymreturnflg) {
						return token;
					}
					if (!yy->yymrejectflg) {
						break;
					}
					if (rejectleng == yy->yymleng) {
						rejectmatch = match;
					}
				}
				else if (yy->yymleng > oldleng + 1) {
					yy->yymleng--;
					(*yy->yymunput)(yy, (unsigned char) yy->yymtext[yy->yymleng]);
				}
				else {
					yy->yymeol = (unsigned char) (yy->yymtext[0] == '\n');
					(*yy->yymoutput)(yy, yy->yymtext[0]);	/* non-matched character */
					break;
				}
			}
		}
		else {
			int index;
			int match;
			yyassert(yy->yymleng == oldleng);

			/* handles <<EOF>> rules */
			index = 0;
			match = yy->yymstate[state].match;
			if (match < 0) {
				index = -match;
				match = yy->yymmatch[index++];
			}
			while (match > 0) {
				int token;
				yy->yymtext[yy->yymleng] = '\0';
#ifdef YYDEBUG
				yymdmatch(yy, match);
#endif
				yy->yymrejectflg = 0;		/* clear flag */

				/* perform user action */
				token = (*yy->yymlexaction)(yy, match);

				if (yy->yymreturnflg) {
					return token;
				}
				if (!yy->yymrejectflg) {
					break;
				}

				if (index == 0) {
					break;
				}
				match = yy->yymmatch[index++];
			}

			if ((*yy->yymwrap)(yy)) {
				yy->yymoldeol = 1;
				yy->yymeol = 1;
				yy->yymstart = 0;
				return 0;			/* eof reached */
			}
		}
	}
}
