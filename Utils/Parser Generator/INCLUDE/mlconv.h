#ifndef MLCONV_H
#define MLCONV_H

/************************************************************
mlconv.h
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

/* variables */
#define yymoreflg (YYLEXNAME).yymmoreflg
#define yyrejectflg (YYLEXNAME).yymrejectflg
#define yyreturnflg (YYLEXNAME).yymreturnflg
#define yytextgrow (YYLEXNAME).yymtextgrow
#define yyunputgrow (YYLEXNAME).yymunputgrow
#define yyeol (YYLEXNAME).yymeol
#define yyoldeol (YYLEXNAME).yymoldeol
#define yystart (YYLEXNAME).yymstart
#define yyin (YYLEXNAME).yymin
#define yyout (YYLEXNAME).yymout
#define yylexerr (YYLEXNAME).yymerr
#define yyleng (YYLEXNAME).yymleng
#define yylineno (YYLEXNAME).yymlineno
#define yyunputindex (YYLEXNAME).yymunputindex
#define yytext (YYLEXNAME).yymtext
#define yystext (YYLEXNAME).yymstext
#define yytext_size (YYLEXNAME).yymtext_size
#define yystext_size (YYLEXNAME).yymstext_size
#define yystatebuf (YYLEXNAME).yymstatebuf
#define yysstatebuf (YYLEXNAME).yymsstatebuf
#define yyunputbufptr (YYLEXNAME).yymunputbufptr
#define yysunputbufptr (YYLEXNAME).yymsunputbufptr
#define yyunput_size (YYLEXNAME).yymunputbufptr
#define yysunput_size (YYLEXNAME).yymsunput_size
#define yylexdebug (YYLEXNAME).yymdebug
#define yylexdebugflush (YYLEXNAME).yymdebugflush
#define yylexdebugout (YYLEXNAME).yymdebugout

/* general functions */
#define yylexinit() yymlexinit(&(YYLEXNAME))
#define yylex() yymlex(&(YYLEXNAME))
#define yyclex() yymclex(&(YYLEXNAME))
#define yyreset() yymreset(&(YYLEXNAME))
#define yylexcleanup() yymlexcleanup(&(YYLEXNAME))
#define yysettextsize(size) yymsettextsize(&(YYLEXNAME), (size))
#define yysetunputsize(size) yymsetunputsize(&(YYLEXNAME), (size))

/* service functions */
#define yyinput() (*(YYLEXNAME).yyminput)(&(YYLEXNAME))
#define yyoutput(ch) (*(YYLEXNAME).yymoutput)(&(YYLEXNAME), (ch))
#define yyunput(ch) (*(YYLEXNAME).yymunput)(&(YYLEXNAME), (ch))
#define yywrap() (*(YYLEXNAME).yymwrap)(&(YYLEXNAME))
#define yygetchar() (*(YYLEXNAME).yymgetchar)(&(YYLEXNAME))
#define yytextoverflow() (*(YYLEXNAME).yymtextoverflow)(&(YYLEXNAME))
#define yyunputoverflow() (*(YYLEXNAME).yymunputoverflow)(&(YYLEXNAME))

/* action functions */
#define yybegin(state) yymbegin(&(YYLEXNAME), (state))
#define yyecho() yymecho(&(YYLEXNAME))
#define yyless(length) yymless(&(YYLEXNAME), (length))
#define yymore() yymmore(&(YYLEXNAME))
#define yynewline(newline) yymnewline(&(YYLEXNAME), (newline))
#define yyreject() yymreject(&(YYLEXNAME))
#define yyunputcount() yymunputcount(&(YYLEXNAME))

/* compatibility */
#define ECHO yyecho()
#define REJECT yyreject()
#define BEGIN (YYLEXNAME).yymstart = 
#define YYSTATE (YYLEXNAME).yymstart
#define YY_START (YYLEXNAME).yymstart

#ifndef input
#define input() (*(YYLEXNAME)->yyminput)(&(YYLEXNAME))
#define YY_INPUT
#endif
#ifndef output
#define output(ch) (*(YYLEXNAME)->yymoutput)(&(YYLEXNAME), (ch))
#define YY_OUTPUT
#endif
#ifdef unput
#define unput(ch) (*(YYLEXNAME)->yymunput)(&(YYLEXNAME), (ch))
#define YY_UNPUT
#endif

#endif
