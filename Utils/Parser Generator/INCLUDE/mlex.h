#ifndef MLEX_H
#define MLEX_H

/************************************************************
mlex.h
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

typedef struct yymlex {
	/* flags */
	unsigned char yymmoreflg;		/* concatenate matched strings */
	unsigned char yymrejectflg;		/* yyreject called from an action */
	unsigned char yymreturnflg;		/* return from an action */
	unsigned char yymtextgrow;		/* whether text buffer is allowed to grow */
	unsigned char yymunputgrow;		/* whether unput buffer is allowed to grow */
	unsigned char yymeol;			/* whether an end-of-line '\n' has been seen */
	unsigned char yymoldeol;		/* previous end-of-line value */
	int yymstart;					/* current start state */

	/* streams */
	FILE YYFAR *yymin;				/* input text stream */
	FILE YYFAR *yymout;				/* output text stream */
	FILE YYFAR *yymerr;				/* error stream */

	int yymleng;					/* matched string length */
	int yymlineno;					/* current line number */
	int yymunputindex;				/* unput buffer position */

	char YYFAR *yymtext;			/* text buffer */
	char YYFAR *yymstext;			/* initial (static) text buffer */
	int yymtext_size;				/* text buffer size */
	int yymstext_size;				/* initial (static) text buffer size */

	int YYFAR *yymstatebuf;			/* state buffer */
	int YYFAR *yymsstatebuf;		/* initial (static) state buffer */

	/* unput buffer */
	int YYFAR *yymunputbufptr;		/* unput buffer */
	int YYFAR *yymsunputbufptr;		/* initial (static) unput buffer */
	int yymunput_size;				/* unput buffer size */
	int yymsunput_size;				/* initial (static) unput buffer size */

	/* functions */
#ifdef YYPROTOTYPE
	int (YYCDECL *yyminput)(struct yymlex YYFAR *yy);
	void (YYCDECL *yymoutput)(struct yymlex YYFAR *yy, int ch);
	void (YYCDECL *yymunput)(struct yymlex YYFAR *yy, int ch);
	int (YYCDECL *yymwrap)(struct yymlex YYFAR *yy);
	int (YYCDECL *yymgetchar)(struct yymlex YYFAR *yy);
	void (YYCDECL *yymtextoverflow)(struct yymlex YYFAR *yy);
	void (YYCDECL *yymunputoverflow)(struct yymlex YYFAR *yy);
	int (YYCDECL *yymlexaction)(struct yymlex YYFAR *yy, int action);
#else
	int (YYCDECL *yyminput)();
	void (YYCDECL *yymoutput)();
	void (YYCDECL *yymunput)();
	int (YYCDECL *yymwrap)();
	int (YYCDECL *yymgetchar)();
	void (YYCDECL *yymtextoverflow)();
	void (YYCDECL *yymunputoverflow)();
	int (YYCDECL *yymlexaction)();
#endif

	/* tables */
	/* fast lexical analyser */
	YYCONST yytransition_t YYNEARFAR *yymtransition;
	int yymtransitionmax;
	/* compact lexical analyser */
	YYCONST yyctransition_t YYNEARFAR *yymctransition;
	/* common */
	YYCONST yymatch_t YYNEARFAR *yymmatch;
	YYCONST yystate_t YYNEARFAR *yymstate;
	YYCONST yybackup_t YYNEARFAR *yymbackup;

	void YYFAR *yymdata;			/* user data */

	/* debugging */
#ifdef YYDEBUG
	int yymdebug;
	int yymdebugflush;
	FILE YYFAR *yymdebugout;
#endif
} yymlex_t;

/* instance functions */
#ifdef YYPROTOTYPE
int YYCDECL yymcreatelex(yymlex_t YYFAR *yy, YYCONST yymlex_t YYFAR *src);
void YYCDECL yymdestroylex(yymlex_t YYFAR *yy);
#else
int YYCDECL yymcreatelex();
void YYCDECL yymdestroylex();
#endif

/* general functions */
#ifdef YYPROTOTYPE
int YYCDECL yymclex(yymlex_t YYFAR *yy);
int YYCDECL yymlex(yymlex_t YYFAR *yy);
void YYCDECL yymlexcleanup(yymlex_t YYFAR *yy);
void YYCDECL yymlexinit(yymlex_t YYFAR *yy);
void YYCDECL yymreset(yymlex_t YYFAR *yy);
int YYCDECL yymsettextsize(yymlex_t YYFAR *yy, int size);
int YYCDECL yymsetunputsize(yymlex_t YYFAR *yy, int size);
#else
int YYCDECL yymclex();
int YYCDECL yymlex();
void YYCDECL yymlexcleanup();
void YYCDECL yymlexinit();
void YYCDECL yymreset();
int YYCDECL yymsettextsize();
int YYCDECL yymsetunputsize();
#endif

/* service functions */
#ifdef YYPROTOTYPE
int YYCDECL yymgetchar(yymlex_t YYFAR *yy);
int YYCDECL yyminput(yymlex_t YYFAR *yy);
void YYCDECL yymoutput(yymlex_t YYFAR *yy, int ch);
void YYCDECL yymtextoverflow(yymlex_t YYFAR *yy);
void YYCDECL yymunput(yymlex_t YYFAR *yy, int ch);
void YYCDECL yymunputoverflow(yymlex_t YYFAR *yy);
int YYCDECL yymwrap(yymlex_t YYFAR *yy);
#else
int YYCDECL yymgetchar();
int YYCDECL yyminput();
void YYCDECL yymoutput();
void YYCDECL yymtextoverflow();
void YYCDECL yymunput();
void YYCDECL yymunputoverflow();
int YYCDECL yymwrap();
#endif

/* action functions */
#ifdef YYPROTOTYPE
void YYCDECL yymecho(yymlex_t YYFAR *yy);
void YYCDECL yymless(yymlex_t YYFAR *yy, int length);
#else
void YYCDECL yymecho();
void YYCDECL yymless();
#endif
#define yymbegin(yy, state) ((yy)->yymstart = (state))
#define yymmore(yy) ((yy)->yymmoreflg = 1)
#define yymnewline(yy, newline) ((newline) ? (yy)->yymeol = 1 : ((yy)->yymeol = 0))
#define yymreject(yy) ((yy)->yymrejectflg = 1)
#define yymunputcount(yy) (yy)->yymunputindex

/* helper functions */
#ifdef YYPROTOTYPE
int YYCDECL yymback(YYCONST yymatch_t YYNEARFAR *p, int action);
#else
int YYCDECL yymback();
#endif
#ifndef yyassert
#define yyassert(expr) assert(expr)
#endif

/* debugging functions */
#ifdef YYDEBUG
#ifdef YYPROTOTYPE
void YYCDECL yymdebugoutput(yymlex_t YYFAR *yy, int ch);
void YYCDECL yymdmatch(yymlex_t YYFAR *yy, int expr);
void YYCDECL yymlexdebugoutput(yymlex_t YYFAR *yy, YYCONST char *string);
#else
void YYCDECL yymdebugoutput();
void YYCDECL yymdmatch();
void YYCDECL yymlexdebugoutput();
#endif
#endif

#define yymisfastlexer(yy) ((yy)->yymtransition != NULL)
#define yymiscompactlexer(yy) ((yy)->yymctransition != NULL)

/* debugging variables */
#ifdef YYDEBUG
extern int YYNEAR YYDCDECL yydebug;
extern int YYNEAR YYDCDECL yydebugflush;
#endif

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
