#ifndef MILCONV_H
#define MILCONV_H

/************************************************************
milconv.h
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

/* variables */
#define yymoreflg (yy)->yymmoreflg
#define yyrejectflg (yy)->yymrejectflg
#define yyreturnflg (yy)->yymreturnflg
#define yytextgrow (yy)->yymtextgrow
#define yyunputgrow (yy)->yymunputgrow
#define yyeol (yy)->yymeol
#define yyoldeol (yy)->yymoldeol
#define yystart (yy)->yymstart
#define yyin (yy)->yymin
#define yyout (yy)->yymout
#define yylexerr (yy)->yymerr
#define yyleng (yy)->yymleng
#define yylineno (yy)->yymlineno
#define yyunputindex (yy)->yymunputindex
#define yytext (yy)->yymtext
#define yystext (yy)->yymstext
#define yytext_size (yy)->yymtext_size
#define yystext_size (yy)->yymstext_size
#define yystatebuf (yy)->yymstatebuf
#define yysstatebuf (yy)->yymsstatebuf
#define yyunputbufptr (yy)->yymunputbufptr
#define yysunputbufptr (yy)->yymsunputbufptr
#define yyunput_size (yy)->yymunputbufptr
#define yysunput_size (yy)->yymsunput_size
#define yylexdebug (yy)->yymdebug
#define yylexdebugflush (yy)->yymdebugflush
#define yylexdebugout (yy)->yymdebugout

/* general functions */
#define yylexinit() yymlexinit(yy)
#define yylex() yymlex(yy)
#define yyclex() yymclex(yy)
#define yyreset() yymreset(yy)
#define yylexcleanup() yymlexcleanup(yy)
#define yysettextsize(size) yymsettextsize(yy, (size))
#define yysetunputsize(size) yymsetunputsize(yy, (size))

/* service functions */
#define yyinput() (*yy->yyminput)(yy)
#define yyoutput(ch) (*yy->yymoutput)(yy, (ch))
#define yyunput(ch) (*yy->yymunput)(yy, (ch))
#define yywrap() (*yy->yymwrap)(yy)
#define yygetchar() (*yy->yymgetchar)(yy)
#define yytextoverflow() (*yy->yymtextoverflow)(yy)
#define yyunputoverflow() (*yy->yymunputoverflow)(yy)

/* action functions */
#define yybegin(state) yymbegin(yy, (state))
#define yyecho() yymecho(yy)
#define yyless(length) yymless(yy, (length))
#define yymore() yymmore(yy)
#define yynewline(newline) yymnewline(yy, (newline))
#define yyreject() yymreject(yy)
#define yyunputcount() yymunputcount(yy)

/* compatibility */
#define ECHO yyecho()
#define REJECT yyreject()
#define BEGIN (yy)->yymstart = 
#define YYSTATE (yy)->yymstart
#define YY_START (yy)->yymstart

#ifndef input
#define input() (*yy->yyminput)(yy)
#define YY_INPUT
#endif
#ifndef output
#define output(ch) (*yy->yymoutput)(yy, (ch))
#define YY_OUTPUT
#endif
#ifdef unput
#define unput(ch) (*yy->yymunput)(yy, (ch))
#define YY_UNPUT
#endif

#endif
