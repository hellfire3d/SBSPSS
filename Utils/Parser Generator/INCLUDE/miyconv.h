#ifndef MIYCONV_H
#define MIYCONV_H

/************************************************************
miyconv.h
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

/* variables */
#define yylookahead (yy)->yymlookahead
#define yystackgrow (yy)->yymstackgrow
#define yyexitflg (yy)->yymexitflg
#define yyretireflg (yy)->yymretireflg
#define yyerrorflg (yy)->yymerrorflg
#define yypopflg (yy)->yympopflg
#define yywipeflg (yy)->yymwipeflg
#define yytop (yy)->yymtop
#define yychar (yy)->yymchar
#define yyskip (yy)->yymskip
#define yyerrorcount (yy)->yymerrorcount
#define yyexitcode (yy)->yymexitcode
#define yyretirecode (yy)->yymretirecode
#define yyerrorpop (yy)->yymerrorpop
#define yyparseerr (yy)->yymerr
#define yystackptr (yy)->yymstackptr
#define yysstackptr (yy)->yymsstackptr
#define yystack_size (yy)->yymstack_size
#define yysstack_size (yy)->yymsstack_size
#define yyattribute_size (yy)->yymattribute_size
#define yyvalptr (yy)->yymvalptr
#define yylvalptr (yy)->yymlvalptr
#define yyattributestackptr (yy)->yymattributestackptr
#define yysattributestackptr (yy)->yymsattributestackptr
#define yyparsedebug (yy)->yymdebug
#define yyparsedebugstack (yy)->yymdebugstack
#define yyparsedebugflush (yy)->yymdebugflush
#define yyparsedebugout (yy)->yymdebugout

/* general functions */
#define yycparse() yymcparse(yy)
#define yycwipe() yymcwipe(yy)
#define yycwork() yymcwork(yy)
#define yydestructpop(num) yymdestructpop(yy, (pop))
#define yyparse() yymparse(yy)
#define yyparsecleanup() yymparsecleanup(yy)
#define yyparseinit() yymparseinit(yy)
#define yysetstacksize(size) yymsetstacksize(yy, (size))
#define yysetup() yymsetup(yy)
#define yywipe() yymwipe(yy)
#define yywork() yymwork(yy)

/* service functions */
#define yygettoken() (*yy->yymgettoken)(yy)
#define yystackoverflow() (*yy->yymstackoverflow)(yy)
#define yyerror(text) (*yy->yymerror)(yy, text)
#define yysyntaxerror() (*yy->yymsyntaxerror)(yy)
#define yydiscard() (*yy->yymdiscard)(yy)

/* action functions */
#define yycdestructclearin() yymcdestructclearin(yy)
#define yydestructclearin() yymdestructclearin(yy)
#define yysetin(token) yymsetin(yy, (token)
#define yyunclearin() yymunclearin(yy)
#define yyabort() yymabort(yy)
#define yyaccept() yymaccept(yy)
#define yyclearin() yymclearin(yy)
#define yydestructclearin() yymdestructclearin(yy)
#define yyerrok() yymerrok(yy)
#define yyexit(exitcode) yymexit(yy, (exitcode))
#define yyforceerror() yymforceerror(yy)
#define yypopping() yympopping(yy)
#define yyrecovering() yymrecovering(yy)
#define yyretire(retirecode) yymretire(yy, (retirecode))
#define yythrowerror(pop) yymthrowerror(yy, (pop))

#ifdef YY_COMPATIBLE
#undef yyerrok
#define yyerrok yymerrok(yy)
#undef yyclearin
#define yyclearin yymclearin(yy)
#endif
#define YYACCEPT yymaccept(yy)
#define YYABORT yymabort(yy)
#define YYERROR yymforceerror(yy)
#define YYRECOVERING yymrecovering(yy)

#endif
