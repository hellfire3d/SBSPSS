/************************************************************
yycwork.cpp
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include <string.h>
#include "cyacc.h"

// the Visual C++ v1.52 compiler generates an error in NT if this isn't present!
#ifdef _MSC_VER
#if defined(M_I86HM) && defined(NDEBUG)
#pragma function(memcpy)
#endif
#endif

int yyfparser::yywork()
{
	int errorpop = 0;
	while (1) {
		unsigned char type;
		short sr;
		yystack_t state = yypeek();			// get top state
		while (1) {
			if (yystateaction[state].lookahead) {
				int index;
				if (!yylookahead) {
					yychar = yygettoken();
					if (yychar < 0) {
						yychar = 0;
					}
					yylookahead = 1;
#ifdef YYDEBUG
					yydgettoken(yychar);
#endif
				}
				index = yystateaction[state].base + yychar;
				if (index >= 0 && index < yytokenaction_size) {
					if (yytokenaction[index].check == state) {
						type = yytokenaction[index].type;
						sr = yytokenaction[index].sr;
						break;		// escape from loop
					}
				}
			}

			type = yystateaction[state].type;
			sr = yystateaction[state].sr;
			if (type != YYAT_DEFAULT) {
				break;		// escape from loop
			}
			state = sr;
		}

		// action
		switch (type) {
		case YYAT_SHIFT:
#ifdef YYDEBUG
			yydshift(yychar);
#endif
			if (yyskip > 0) {
				yysetskip(yyskip - 1);
			}
			if (!yypush(sr)) {
#ifdef YYDEBUG
				yydabort();
#endif
				if (yywipeflg) {
					yywipe();	// clean up
				}
				return 1;
			}
			memcpy(&((char YYFAR*)yyattributestackptr)[yytop * yyattribute_size],
				yylvalptr, yyattribute_size);
			yylookahead = 0;
			continue;		// go to top of while loop
		case YYAT_REDUCE:
#ifdef YYDEBUG
			yydreduce(sr);
#endif
			yyretireflg = 0;
			if (yyreduction[sr].action != -1) {
				// user actions in here
				if (yyreduction[sr].length > 0) {
					memcpy(yyvalptr, &((char YYFAR*)yyattributestackptr)
						[(yytop + 1 - yyreduction[sr].length) * yyattribute_size],
						yyattribute_size);
				}

				yyerrorflg = 0;
				yyexitflg = 0;
				yyaction(yyreduction[sr].action);

				// check for special user requected actions
				if (yyexitflg) {
#ifdef YYDEBUG
					yydexit(yyexitcode);
#endif
					return yyexitcode;
				}
				if (yyerrorflg) {
					errorpop = yyerrorpop;
#ifdef YYDEBUG
					yydthrowerror(yyerrorpop);
#endif
					yyerrorcount++;
					break;		// go to error handler
				}
			}

			yypop(yyreduction[sr].length);
			{
				yystack_t state = yypeek();       // get top state
				short next;

				int nonterm = yyreduction[sr].nonterm;
				while (1) {
					int index = yystategoto[state].base + nonterm;
					if (index >= 0 && index < yynontermgoto_size) {
						if (yynontermgoto[index].check == state) {
							next = yynontermgoto[index].next;
							break;
						}
					}

					next = yystategoto[state].def;
					if (next == -1) {
						break;
					}
					state = next;
				}
				yyassert(next != -1);

				if (!yypush(next)) {
#ifdef YYDEBUG
					yydabort();
#endif
					if (yywipeflg) {
						yywipe();	// clean up
					}
					return 1;
				}
			}
			if (yyreduction[sr].action != -1) {
				memcpy(&((char YYFAR*)yyattributestackptr)[yytop * yyattribute_size],
					yyvalptr, yyattribute_size);
			}
			if (yyretireflg) {
#ifdef YYDEBUG
				yydretire(yyretirecode);
#endif
				return yyretirecode;
			}
			continue;		// go to top of while loop
		case YYAT_ERROR:
#ifdef YYDEBUG
			yydsyntaxerror();
#endif
			if (yyskip == 0) {
				yyerrorcount++;
				yysyntaxerror();
			}
			break;		// go to error handler
		default:
			yyassert(type == YYAT_ACCEPT);
#ifdef YYDEBUG
			yydaccept();
#endif
			return 0;
		}

		// error handler
		if (yyskip < 3 || errorpop > 0) {
#ifdef YYDEBUG
			yydattemptrecovery();
#endif
			yypopflg = 0;		// clear flag
			while (1) {
				state = yypeek();			// get top state
				while (1) {
					if (yystateaction[state].lookahead) {
						int index = yystateaction[state].base + YYTK_ERROR;
						if (index >= 0 && index < yytokenaction_size) {
							if (yytokenaction[index].check == state) {
								type = yytokenaction[index].type;
								sr = yytokenaction[index].sr;
								break;		// escape from loop
							}
						}
					}

					type = yystateaction[state].type;
					sr = yystateaction[state].sr;
					if (type != YYAT_DEFAULT) {
						break;		// escape from loop
					}
					state = sr;
				}

				if (type == YYAT_SHIFT) {
					if (errorpop <= 0) {
#ifdef YYDEBUG
						yydshift(YYTK_ERROR);
#endif
						if (!yypush(sr)) {
#ifdef YYDEBUG
							yydabort();
#endif
							if (yywipeflg) {
								yywipe();	// clean up
							}
							return 1;
						}
						yysetskip(3);		// skip 3 erroneous characters
						break;
					}
					errorpop--;
				}

				yypopflg = 1;

				// clean up any symbol attributes
				if (yydestructorptr != NULL) {
					int action;
					state = yypeek();
					action = yydestructorptr[state];
					if (action != -1) {
						// user actions in here
						memcpy(yyvalptr, &((char YYFAR*)yyattributestackptr)
							[yytop * yyattribute_size], yyattribute_size);

						yyaction(action);

						memcpy(&((char YYFAR*)yyattributestackptr)
							[yytop * yyattribute_size], yyvalptr, yyattribute_size);
					}
				}
				yypop(1);
				if (yytop < 0) {
#ifdef YYDEBUG
					yydabort();
#endif
					if (yywipeflg) {
						yywipe();	// clean up
					}
					return 1;
				}
			}
		}
		else {
			if (yylookahead) {
				if (yychar != 0) {
#ifdef YYDEBUG
					yyddiscard(yychar);
#endif
					yydiscard(yychar);

					// clean up any token attributes
					if (yytokendestptr != NULL) {
						int index = yytokendestbase + yychar;
						if (index >= 0 && index < yytokendest_size) {
							int action = yytokendestptr[index];
							if (action != -1) {
								// user actions in here
								memcpy(yyvalptr, yylvalptr, yyattribute_size);

								yyaction(action);

								memcpy(yylvalptr, yyvalptr, yyattribute_size);
							}
						}
					}

					yylookahead = 0;	// skip erroneous character
				}
				else {
#ifdef YYDEBUG
					yydabort();
#endif
					if (yywipeflg) {
						yywipe();	// clean up
					}
					return 1;
				}
			}
		}
	}
}
