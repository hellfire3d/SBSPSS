#ifndef LEX_H
#define LEX_H

/************************************************************
lex.h
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include <stdio.h>
#include <assert.h>

/* defines */
#include <yytdefs.h>

/* user defines */
#if defined(YYTUDEFS) || defined(YYUDEFS)
#include <yytudefs.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define YY_ALEX

/* modifiers */
#ifndef YYCDECL
#define YYCDECL
#endif
#ifndef YYDCDECL
#define YYDCDECL
#endif
#ifndef YYNEAR
#define YYNEAR
#endif
#ifndef YYFAR
#define YYFAR
#endif
#ifndef YYNEARFAR
#define YYNEARFAR
#endif
#ifndef YYBASED_CODE
#define YYBASED_CODE
#endif

/* function prototypes */
#ifndef YYPROTOTYPE
#if defined(__STDC__) || defined(__cplusplus)
#define YYPROTOTYPE
#endif
#endif

/* qualifiers */
#ifndef YYCONST
#if defined(__STDC__) || defined(__cplusplus)
#define YYCONST const
#else
#define YYCONST
#endif
#endif

/* testing */
#ifdef YYNOPROTOTYPE
#undef YYPROTOTYPE
#endif
#ifdef YYNOCONST
#undef YYCONST
#define YYCONST
#endif

/* yylex return values */
#define YYEOF 0			/* end of file */

#ifndef YYSTATE_T
#define YYSTATE_T
typedef struct yystate {
	short def;			/* default state */
	short base;			/* base */
	short match;		/* action associated with state */
} yystate_t;
#endif

#ifndef YYTRANSITION_T
#define YYTRANSITION_T
typedef struct yytransition {
	short next;			/* next state on transition */
	short check;		/* check */
} yytransition_t;
#endif

#ifndef YYCTRANSITION_T
#define YYCTRANSITION_T
typedef struct yyctransition {
	unsigned char first;	/* first character in range */
	unsigned char last;		/* last character in range */
	short next;				/* next state on transition */
} yyctransition_t;
#endif

typedef short yymatch_t;
typedef unsigned char yybackup_t;

/* general functions */
#ifdef YYPROTOTYPE
int YYCDECL yyclex(void);
int YYCDECL yylex(void);
void YYCDECL yylexcleanup(void);
void YYCDECL yylexinit(void);
void YYCDECL yyreset(void);
int YYCDECL yysettextsize(int size);
int YYCDECL yysetunputsize(int size);
#else
int YYCDECL yyclex();
int YYCDECL yylex();
void YYCDECL yylexcleanup();
void YYCDECL yylexinit();
void YYCDECL yyreset();
int YYCDECL yysettextsize();
int YYCDECL yysetunputsize();
#endif

/* service functions */
#ifdef YYPROTOTYPE
int YYCDECL yygetchar(void);
int YYCDECL yyinput(void);
void YYCDECL yyoutput(int ch);
void YYCDECL yytextoverflow(void);
void YYCDECL yyunput(int ch);
void YYCDECL yyunputoverflow(void);
int YYCDECL yywrap(void);
#else
int YYCDECL yygetchar();
int YYCDECL yyinput();
void YYCDECL yyoutput();
void YYCDECL yytextoverflow();
void YYCDECL yyunput();
void YYCDECL yyunputoverflow();
int YYCDECL yywrap();
#endif

/* action functions */
#ifdef YYPROTOTYPE
void YYCDECL yyecho(void);
void YYCDECL yyless(int length);
#else
void YYCDECL yyecho();
void YYCDECL yyless();
#endif
#define yybegin(state) (yystart = (state))
#define yymore() (yymoreflg = 1)
#define yynewline(newline) ((newline) ? yyeol = 1 : (yyeol = 0))
#define yyreject() (yyrejectflg = 1)
#define yyunputcount() yyunputindex

/* compatibility */
#define BEGIN yystart = 
#define ECHO yyecho()
#define REJECT yyreject()
#define YYSTATE yystart
#define YY_START yystart

/* helper functions */
#ifdef YYPROTOTYPE
int YYCDECL yyback(YYCONST yymatch_t YYNEARFAR *p, int action);
#else
int YYCDECL yyback();
#endif
#ifndef yyassert
#define yyassert(expr) assert(expr)
#endif

/* variables */
extern unsigned char YYNEAR YYDCDECL yymoreflg;
extern unsigned char YYNEAR YYDCDECL yyrejectflg;
extern unsigned char YYNEAR YYDCDECL yyreturnflg;
extern unsigned char YYNEAR YYDCDECL yytextgrow;
extern unsigned char YYNEAR YYDCDECL yyunputgrow;
extern unsigned char YYNEAR YYDCDECL yyeol;
extern unsigned char YYNEAR YYDCDECL yyoldeol;
extern int YYNEAR YYDCDECL yystart;

extern FILE YYFAR *YYNEAR YYDCDECL yyin;
extern FILE YYFAR *YYNEAR YYDCDECL yyout;
extern FILE YYFAR *YYNEAR YYDCDECL yylexerr;

extern int YYNEAR YYDCDECL yyleng;
extern int YYNEAR YYDCDECL yylineno;
extern int YYNEAR YYDCDECL yyunputindex;

/* debugging functions */
#ifdef YYDEBUG
#ifdef YYPROTOTYPE
void YYCDECL yydebugoutput(int ch);
void YYCDECL yydmatch(int expr);
void YYCDECL yylexdebugoutput(YYCONST char *string);
#else
void YYCDECL yydebugoutput();
void YYCDECL yydmatch();
void YYCDECL yylexdebugoutput();
#endif
#endif

/* debugging variables */
#ifdef YYDEBUG
extern int YYNEAR YYDCDECL yydebug;
extern int YYNEAR YYDCDECL yydebugflush;
extern int YYNEAR YYDCDECL yylexdebug;
extern int YYNEAR YYDCDECL yylexdebugflush;
extern FILE YYFAR *YYNEAR YYDCDECL yylexdebugout;
#endif

/* externally defined */
#ifdef YYPROTOTYPE
int YYCDECL yylexaction(int action);
#else
int YYCDECL yylexaction();
#endif

extern char YYFAR *YYNEAR YYDCDECL yytext;
extern char YYFAR *YYNEAR YYDCDECL yystext;
extern int YYNEAR YYDCDECL yytext_size;
extern int YYNEAR YYDCDECL yystext_size;

extern int YYFAR *YYNEAR YYDCDECL yystatebuf;
extern int YYFAR *YYNEAR YYDCDECL yysstatebuf;

/* unput buffer */
extern int YYFAR *YYNEAR YYDCDECL yyunputbufptr;
extern int YYFAR *YYNEAR YYDCDECL yysunputbufptr;
extern int YYNEAR YYDCDECL yyunput_size;
extern int YYNEAR YYDCDECL yysunput_size;

/* fast lexical analyser */
extern YYCONST yytransition_t YYNEARFAR YYDCDECL yytransition[];
extern int YYNEAR YYDCDECL yytransitionmax;

/* compact lexical analyser */
extern YYCONST yyctransition_t YYNEARFAR YYDCDECL yyctransition[];

extern YYCONST yymatch_t YYNEARFAR YYDCDECL yymatch[];
extern YYCONST yystate_t YYNEARFAR YYDCDECL yystate[];
extern YYCONST yybackup_t YYNEARFAR YYDCDECL yybackup[];

#ifdef __cplusplus
}
#endif

/* user defines */
#if defined(YYBUDEFS) || defined(YYUDEFS)
#include <yybudefs.h>
#endif

/* defines */
#include <yybdefs.h>

#endif
