/************************************************************
yymcwork.c
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include <string.h>
#include "myacc.h"

#ifdef YYPROTOTYPE
int YYCDECL yymcwork(yymparse_t YYFAR *yy)
#else
int YYCDECL yymcwork(yy)
yymparse_t YYFAR *yy;
#endif
{
	int errorpop = 0;
	yyassert(yy != NULL);
	yyassert(yymiscompactparser(yy));	/* make sure it's a compact parser */

	while (1) {
		unsigned char type;
		short sr;
		yystack_t state = yympeek(yy);			/* get top state */

		int index = yy->yymcstateaction[state];
		while (1) {
			if (yy->yymctokenaction[index].token == YYTK_ALL) {
				if (yy->yymctokenaction[index].type == YYAT_DEFAULT) {
					state = yy->yymctokenaction[index].sr;
					index = yy->yymcstateaction[state];
					continue;
				}
				break;
			}

			if (!yy->yymlookahead) {
				yy->yymlookahead = 1;
				yy->yymchar = (*yy->yymgettoken)(yy);
				if (yy->yymchar < 0) {
					yy->yymchar = 0;
				}
#ifdef YYDEBUG
				yymdgettoken(yy, yy->yymchar);
#endif
			}
			if (yy->yymctokenaction[index].token == yy->yymchar) {
				break;
			}
			index++;
		}
		type = yy->yymctokenaction[index].type;
		sr = yy->yymctokenaction[index].sr;

		/* action */
		switch (type) {
		case YYAT_SHIFT:
#ifdef YYDEBUG
			yymdshift(yy, yy->yymchar);
#endif
			if (yy->yymskip > 0) {
				yymsetskip(yy, yy->yymskip - 1);
			}
			if (!yympush(yy, sr)) {
#ifdef YYDEBUG
				yymdabort(yy);
#endif
				if (yy->yymwipeflg) {
					yymcwipe(yy);	/* clean up */
				}
				return 1;
			}
			memcpy(&((char YYFAR *) yy->yymattributestackptr)[yy->yymtop *
				yy->yymattribute_size], yy->yymlvalptr, yy->yymattribute_size);
			yy->yymlookahead = 0;
			continue;		/* go to top of while loop */
		case YYAT_REDUCE:
#ifdef YYDEBUG
			yymdreduce(yy, sr);
#endif
			yy->yymretireflg = 0;
			if (yy->yymreduction[sr].action != -1) {
				/* user actions in here */
				if (yy->yymreduction[sr].length > 0) {
					memcpy(yy->yymvalptr, &((char YYFAR *) yy->yymattributestackptr)
						[(yy->yymtop + 1 - yy->yymreduction[sr].length) *
						yy->yymattribute_size], yy->yymattribute_size);
				}
        
				yy->yymerrorflg = 0;
				yy->yymexitflg = 0;
				(*yy->yymparseaction)(yy, yy->yymreduction[sr].action);

				/* check for special user requected actions */
				if (yy->yymexitflg) {
#ifdef YYDEBUG
					yymexit(yy, yy->yymexitcode);
#endif
					return yy->yymexitcode;
				}
				if (yy->yymerrorflg) {
					errorpop = yy->yymerrorpop;
#ifdef YYDEBUG
					yymdthrowerror(yy, yy->yymerrorpop);
#endif
					yy->yymerrorcount++;
					break;		/* go to error handler */
				}
			}

			yympop(yy, yy->yymreduction[sr].length);
			{
				yystack_t state = yympeek(yy);	/* get top state */
				short next;

				int nonterm = yy->yymreduction[sr].nonterm;
				int index = yy->yymcstategoto[state];
				while (1) {
					if (yy->yymcnontermgoto[index].nonterm == -1) {
						if (yy->yymcnontermgoto[index].next != -1) {
							state = yy->yymcnontermgoto[index].next;
							index = yy->yymcstategoto[state];
							continue;
						}
						break;
					}
					if (yy->yymcnontermgoto[index].nonterm == nonterm) {
						break;
					}
					index++;
				}
				next = yy->yymcnontermgoto[index].next;
				yyassert(next != -1);

				if (!yympush(yy, next)) {
#ifdef YYDEBUG
					yymdabort(yy);
#endif
					if (yy->yymwipeflg) {
						yymcwipe(yy);	/* clean up */
					}
					return 1;
				}
			}
			if (yy->yymreduction[sr].action != -1) {
				memcpy(&((char YYFAR *) yy->yymattributestackptr)
					[yy->yymtop * yy->yymattribute_size],
					yy->yymvalptr, yy->yymattribute_size);
			}
			if (yy->yymretireflg) {
#ifdef YYDEBUG
				yymdretire(yy, yy->yymretirecode);
#endif
				return yy->yymretirecode;
			}
			continue;		/* go to top of while loop */
		case YYAT_ERROR:
#ifdef YYDEBUG
			yymdsyntaxerror(yy);
#endif
			if (yy->yymskip == 0) {
				yy->yymerrorcount++;
				(*yy->yymsyntaxerror)(yy);
			}
			break;		/* go to error handler */
		default:
			yyassert(type == YYAT_ACCEPT);
#ifdef YYDEBUG
			yymdaccept(yy);
#endif
			return 0;
		}

		/* error handler */
		if (yy->yymskip < 3 || errorpop > 0) {
#ifdef YYDEBUG
			yymdattemptrecovery(yy);
#endif
			yy->yympopflg = 0;		/* clear flag */
			while (1) {
				state = yympeek(yy);			/* get top state */
				index = yy->yymcstateaction[state];
				while (1) {
					if (yy->yymctokenaction[index].token == YYTK_ALL) {
						if (yy->yymctokenaction[index].type == YYAT_DEFAULT) {
							state = yy->yymctokenaction[index].sr;
							index = yy->yymcstateaction[state];
							continue;
						}
						break;
					}
					if (yy->yymctokenaction[index].token == YYTK_ERROR) {
						break;
					}
					index++;
				}
				type = yy->yymctokenaction[index].type;
				sr = yy->yymctokenaction[index].sr;

				if (type == YYAT_SHIFT) {
					if (errorpop <= 0) {
#ifdef YYDEBUG
						yymdshift(yy, YYTK_ERROR);
#endif
						if (!yympush(yy, sr)) {
#ifdef YYDEBUG
							yymdabort(yy);
#endif
							if (yy->yymwipeflg) {
								yymcwipe(yy);	/* clean up */
							}
							return 1;
						}
						yymsetskip(yy, 3);		/* skip 3 erroneous characters */
						break;
					}
					errorpop--;
				}

				yy->yympopflg = 1;

				/* clean up any symbol attributes */
				if (yy->yymdestructorptr != NULL) {
					int action;
					state = yympeek(yy);
					action = yy->yymdestructorptr[state];
					if (action != -1) {
						/* user actions in here */
						memcpy(yy->yymvalptr, &((char YYFAR *) yy->yymattributestackptr)
							[yy->yymtop * yy->yymattribute_size],
							yy->yymattribute_size);
        
						(*yy->yymparseaction)(yy, action);

						memcpy(&((char YYFAR *) yy->yymattributestackptr)
							[yy->yymtop * yy->yymattribute_size], yy->yymvalptr,
							yy->yymattribute_size);
					}
				}
				yympop(yy, 1);
				if (yy->yymtop < 0) {
#ifdef YYDEBUG
					yymdabort(yy);
#endif
					if (yy->yymwipeflg) {
						yymcwipe(yy);	/* clean up */
					}
					return 1;
				}
			}
		}
		else {
			if (yy->yymlookahead) {
				if (yy->yymchar != 0) {
#ifdef YYDEBUG
					yymddiscard(yy, yy->yymchar);
#endif
					(*yy->yymdiscard)(yy, yy->yymchar);

					/* clean up any token attributes */
					if (yy->yymctokendestptr != NULL) {
						YYCONST yyctokendest_t YYNEARFAR *tokendestptr = yy->yymctokendestptr;
						while (tokendestptr->token != 0) {
							if (tokendestptr->token == yy->yymchar) {
								/* user actions in here */
								memcpy(yy->yymvalptr, yy->yymlvalptr, yy->yymattribute_size);

								(*yy->yymparseaction)(yy, tokendestptr->action);

								memcpy(yy->yymlvalptr, yy->yymvalptr, yy->yymattribute_size);
								break;
							}
							tokendestptr++;
						}
					}

					yy->yymlookahead = 0;	/* skip erroneous character */
				}
				else {
#ifdef YYDEBUG
					yymdabort(yy);
#endif
					if (yy->yymwipeflg) {
						yymcwipe(yy);	/* clean up */
					}
					return 1;
				}
			}
		}
	}
}
