#ifndef MYCONV_H
#define MYCONV_H

/************************************************************
myconv.h
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

/* variables */
#define yylookahead (YYPARSENAME).yymlookahead
#define yystackgrow (YYPARSENAME).yymstackgrow
#define yyexitflg (YYPARSENAME).yymexitflg
#define yyretireflg (YYPARSENAME).yymretireflg
#define yyerrorflg (YYPARSENAME).yymerrorflg
#define yypopflg (YYPARSENAME).yympopflg
#define yywipeflg (YYPARSENAME).yymwipeflg
#define yytop (YYPARSENAME).yymtop
#define yychar (YYPARSENAME).yymchar
#define yyskip (YYPARSENAME).yymskip
#define yyerrorcount (YYPARSENAME).yymerrorcount
#define yyexitcode (YYPARSENAME).yymexitcode
#define yyretirecode (YYPARSENAME).yymretirecode
#define yyerrorpop (YYPARSENAME).yymerrorpop
#define yyparseerr (YYPARSENAME).yymerr
#define yystackptr (YYPARSENAME).yymstackptr
#define yysstackptr (YYPARSENAME).yymsstackptr
#define yystack_size (YYPARSENAME).yymstack_size
#define yysstack_size (YYPARSENAME).yymsstack_size
#define yyattribute_size (YYPARSENAME).yymattribute_size
#define yyvalptr (YYPARSENAME).yymvalptr
#define yylvalptr (YYPARSENAME).yymlvalptr
#define yyattributestackptr (YYPARSENAME).yymattributestackptr
#define yysattributestackptr (YYPARSENAME).yymsattributestackptr
#define yyparsedebug (YYPARSENAME).yymdebug
#define yyparsedebugstack (YYPARSENAME)->yymdebugstack
#define yyparsedebugflush (YYPARSENAME).yymdebugflush
#define yyparsedebugout (YYPARSENAME).yymdebugout

/* general functions */
#define yycparse() yymcparse(&(YYPARSENAME))
#define yycwipe() yymcwipe(&(YYPARSENAME))
#define yycwork() yymcwork(&(YYPARSENAME))
#define yydestructpop(num) yymdestructpop(&(YYPARSENAME), (pop))
#define yyparse() yymparse(&(YYPARSENAME))
#define yyparsecleanup() yymparsecleanup(&(YYPARSENAME))
#define yyparseinit() yymparseinit(&(YYPARSENAME))
#define yysetstacksize(size) yymsetstacksize(&(YYPARSENAME), (size))
#define yysetup() yymsetup(&(YYPARSENAME))
#define yywipe() yymwipe(&(YYPARSENAME))
#define yywork() yymwork(&(YYPARSENAME))

/* service functions */
#define yygettoken() (*(YYPARSENAME).yymgettoken)(&(YYPARSENAME))
#define yystackoverflow() (*(YYPARSENAME).yymstackoverflow)(&(YYPARSENAME))
#define yyerror(text) (*(YYPARSENAME).yymerror)(&(YYPARSENAME), (text))
#define yysyntaxerror() (*(YYPARSENAME).yymsyntaxerror)(&(YYPARSENAME))
#define yydiscard() (*(YYPARSENAME).yymdiscard)(&(YYPARSENAME))

/* action functions */
#define yycdestructclearin() yymcdestructclearin(&(YYPARSENAME))
#define yydestructclearin() yymdestructclearin(&(YYPARSENAME))
#define yysetin(token) yymsetin(&(YYPARSENAME), (token)
#define yyunclearin() yymunclearin(&(YYPARSENAME))
#define yyabort() yymabort(&(YYPARSENAME))
#define yyaccept() yymaccept(&(YYPARSENAME))
#define yyclearin() yymclearin(&(YYPARSENAME))
#define yydestructclearin() yymdestructclearin(&(YYPARSENAME))
#define yyerrok() yymerrok(&(YYPARSENAME))
#define yyexit(exitcode) yymexit(&(YYPARSENAME), (exitcode))
#define yyforceerror() yymforceerror(&(YYPARSENAME))
#define yypopping() yympopping(&(YYPARSENAME))
#define yyrecovering() yymrecovering(&(YYPARSENAME))
#define yyretire(retirecode) yymretire(&(YYPARSENAME), (retirecode))
#define yythrowerror(pop) yymthrowerror(&YYPARSENAME), (pop))

#ifdef YY_COMPATIBLE
#undef yyerrok
#define yyerrok yymerrok(&(YYPARSENAME))
#undef yyclearin
#define yyclearin yymclearin(&(YYPARSENAME))
#endif
#define YYACCEPT yymaccept(&(YYPARSENAME))
#define YYABORT yymabort(&(YYPARSENAME))
#define YYERROR yymforceerror(&(YYPARSENAME))
#define YYRECOVERING yymrecovering(&(YYPARSENAME))

#endif
