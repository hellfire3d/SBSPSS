#ifndef MYACC_H
#define MYACC_H

/************************************************************
myacc.h
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include <stdio.h>
#include <stddef.h>
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

#define YY_AYACC

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

typedef short yystack_t;

/* yyparse return values */
#define YYEXIT_SUCCESS	0
#define YYEXIT_FAILURE	1

/* common tokens */
#define YYTK_ALL		(-1)		/* match all tokens */
#define YYTK_END		0			/* $end token */
#define YYTK_ERROR		256			/* error token */

#ifndef YYCSTATEGOTO_T
#define YYCSTATEGOTO_T
typedef short yycstategoto_t;
#endif

#ifndef YYCNONTERMGOTO_T
#define YYCNONTERMGOTO_T
typedef struct yycnontermgoto {
	short nonterm;					/* nonterminal */
	short next;						/* next state */
} yycnontermgoto_t;
#endif

#ifndef YYSTATEGOTO_T
#define YYSTATEGOTO_T
typedef struct yystategoto {
	short base;						/* base */
	short def;						/* default state */
} yystategoto_t;
#endif

#ifndef YYNONTERMGOTO_T
#define YYNONTERMGOTO_T
typedef struct yynontermgoto {
	short check;					/* check */
	short next;						/* next state */
} yynontermgoto_t;
#endif

/* action types */
#define YYAT_SHIFT		0			/* shift action */
#define YYAT_REDUCE		1			/* reduce action */
#define YYAT_ERROR		2			/* error */
#define YYAT_ACCEPT		3			/* accept */
#define YYAT_DEFAULT	4			/* default state */

#ifndef YYCSTATEACTION_T
#define YYCSTATEACTION_T
typedef short yycstateaction_t;
#endif

#ifndef YYCTOKENACTION_T
#define YYCTOKENACTION_T
typedef struct yyctokenaction {
	int token;						/* lookahead token */
	unsigned char type;				/* action to perform */
	short sr;						/* state to shift/production to reduce */
} yyctokenaction_t;
#endif

#ifndef YYSTATEACTION_T
#define YYSTATEACTION_T
typedef struct yystateaction {
	short base;						/* base */
	unsigned char lookahead;		/* lookahead needed */
	unsigned char type;				/* action to perform */
	short sr;						/* shift/reduce */
} yystateaction_t;
#endif

#ifndef YYTOKENACTION_T
#define YYTOKENACTION_T
typedef struct yytokenaction {
	short check;					/* check */
	unsigned char type;				/* action type */
	short sr;						/* shift/reduce */
} yytokenaction_t;
#endif

/* nonterminals */
#define YYST_ALL		(-1)		/* match all states */

/* states */
#define YYST_ERROR		(-1)		/* goto error */

#ifndef YYREDUCTION_T
#define YYREDUCTION_T
typedef struct yyreduction {
	short nonterm;					/* the rhs symbol */
	short length;					/* number of symbols on lhs */
	short action;					/* the user action */
} yyreduction_t;
#endif

typedef short yydestructor_t;

typedef short yytokendest_t;

#ifndef YYCTOKENDEST_T
#define YYCTOKENDEST_T
typedef struct yyctokendest {
	int token;						/* token */
	short action;					/* the user action */
} yyctokendest_t;
#endif

/* debugging */
#ifdef YYDEBUG
#ifndef YYSYMBOL_T
#define YYSYMBOL_T
typedef struct yysymbol {
	YYCONST char *name;				/* symbol name */
	int token;						/* symbol token */
} yysymbol_t;
#endif
#endif

typedef struct yymparse {
	/* flags */
	unsigned char yymlookahead;			/* whether current lookahead token is valid */
	unsigned char yymstackgrow;			/* whether stack can grow */
	unsigned char yymexitflg;			/* whether yymexit called */
	unsigned char yymretireflg;			/* whether yymretire called */
	unsigned char yymerrorflg;			/* whether yymforceerror called */
	unsigned char yympopflg;			/* popping symbols during error recovery */
	unsigned char yymwipeflg;			/* whether to "wipe" stack on abort */

	int yymtop;							/* the current top of the stack */
	int yymchar;						/* current lookahead token */
	int yymskip;						/* error recovery token shift counter */
	int yymerrorcount;					/* how many syntax errors have occurred */
	int yymexitcode;					/* yymexit exit code */
	int yymretirecode;					/* yymretire exit code */
	int yymerrorpop;					/* how many error transitions to pop */
	FILE YYFAR *yymerr;					/* error output file */

	/* stack */
	yystack_t YYFAR *yymstackptr;		/* (state) stack */
	yystack_t YYFAR *yymsstackptr;		/* static (state) stack */
	int yymstack_size;					/* number of elements in stack */
	int yymsstack_size;					/* initial number of elements in stack */

	size_t yymattribute_size;			/* size of attribute */

#ifdef YYPROTOTYPE
	void YYFAR *yymvalptr;				/* attribute for $$ */
	void YYFAR *yymlvalptr;				/* current token attribute */
	void YYFAR *yymattributestackptr;	/* attribute stack */
	void YYFAR *yymsattributestackptr;	/* static attribute stack */
#else
	char YYFAR *yymvalptr;				/* attribute for $$ */
	char YYFAR *yymlvalptr;				/* current token attribute */
	char YYFAR *yymattributestackptr;	/* attribute stack */
	char YYFAR *yymsattributestackptr;	/* static attribute stack */
#endif

	/* service functions */
#ifdef YYPROTOTYPE
	void (YYCDECL *yymstackoverflow)(struct yymparse YYFAR *yy);
	void (YYCDECL *yymerror)(struct yymparse YYFAR *yy, YYCONST char YYFAR *text);
	void (YYCDECL *yymsyntaxerror)(struct yymparse YYFAR *yy);
	void (YYCDECL *yymdiscard)(struct yymparse YYFAR *yy, int token);
	int (YYCDECL *yymgettoken)(struct yymparse YYFAR *yy);
	void (YYCDECL *yymparseaction)(struct yymparse YYFAR *yy, int action);
#else
	void (YYCDECL *yymstackoverflow)();
	void (YYCDECL *yymerror)();
	void (YYCDECL *yymsyntaxerror)();
	void (YYCDECL *yymdiscard)();
	int (YYCDECL *yymgettoken)();
	void (YYCDECL *yymparseaction)();
#endif

	/* tables */
	/* fast parser */
	YYCONST yystateaction_t YYNEARFAR *yymstateaction;
	YYCONST yytokenaction_t YYNEARFAR *yymtokenaction;
	int yymtokenaction_size;
	YYCONST yystategoto_t YYNEARFAR *yymstategoto;
	YYCONST yynontermgoto_t YYNEARFAR *yymnontermgoto;
	int yymnontermgoto_size;
	YYCONST yytokendest_t YYNEARFAR *yymtokendestptr;
	int yymtokendest_size;
	int yymtokendestbase;
	/* compact parser */
	YYCONST yycstateaction_t YYNEARFAR *yymcstateaction;
	YYCONST yyctokenaction_t YYNEARFAR *yymctokenaction;
	YYCONST yycstategoto_t YYNEARFAR *yymcstategoto;
	YYCONST yycnontermgoto_t YYNEARFAR *yymcnontermgoto;
	YYCONST yyctokendest_t YYNEARFAR *yymctokendestptr;
	/* common */
	YYCONST yyreduction_t YYNEARFAR *yymreduction;

	YYCONST yydestructor_t YYNEARFAR *yymdestructorptr;

	void YYFAR *yymdata;				/* user data */

	/* debugging */
#ifdef YYDEBUG
	int yymdebug;
	int yymdebugstack;
	int yymdebugflush;
	FILE YYFAR *yymdebugout;
	YYCONST yysymbol_t YYNEARFAR *yymsymbol;
	YYCONST char *YYCONST YYNEARFAR *yymrule;
#endif
} yymparse_t;

/* instance functions */
#ifdef YYPROTOTYPE
int YYCDECL yymcreateparse(yymparse_t YYFAR *yy, YYCONST yymparse_t YYFAR *src);
void YYCDECL yymdestroyparse(yymparse_t YYFAR *yy);
#else
int YYCDECL yymcreateparse();
void YYCDECL yymdestroyparse();
#endif

/* general functions */
#ifdef YYPROTOTYPE
int YYCDECL yymcparse(yymparse_t YYFAR *yy);
void YYCDECL yymcwipe(yymparse_t YYFAR *yy);
int YYCDECL yymcwork(yymparse_t YYFAR *yy);
void YYCDECL yymdestructpop(yymparse_t YYFAR *yy, int num);
int YYCDECL yymparse(yymparse_t YYFAR *yy);
void YYCDECL yymparsecleanup(yymparse_t YYFAR *yy);
void YYCDECL yymparseinit(yymparse_t YYFAR *yy);
int YYCDECL yymsetstacksize(yymparse_t YYFAR *yy, int size);
int YYCDECL yymsetup(yymparse_t YYFAR *yy);
void YYCDECL yymwipe(yymparse_t YYFAR *yy);
int YYCDECL yymwork(yymparse_t YYFAR *yy);
#else
int YYCDECL yymcparse();
void YYCDECL yymcwipe();
int YYCDECL yymcwork();
void YYCDECL yymdestructpop();
int YYCDECL yymparse();
void YYCDECL yymparsecleanup();
void YYCDECL yymparseinit();
int YYCDECL yymsetstacksize();
int YYCDECL yymsetup();
void YYCDECL yymwipe();
int YYCDECL yymwork();
#endif

/* service functions */
#ifdef YYPROTOTYPE
void YYCDECL yymdiscard(yymparse_t YYFAR *yy, int token);
void YYCDECL yymerror(yymparse_t YYFAR *yy, YYCONST char YYFAR *text);
int YYCDECL yymgettoken(yymparse_t YYFAR *yy);
void YYCDECL yymstackoverflow(yymparse_t YYFAR *yy);
void YYCDECL yymsyntaxerror(yymparse_t YYFAR *yy);
#else
void YYCDECL yymdiscard();
void YYCDECL yymerror();
int YYCDECL yymgettoken();
void YYCDECL yymstackoverflow();
void YYCDECL yymsyntaxerror();
#endif

/* action functions */
#ifdef YYPROTOTYPE
void YYCDECL yymcdestructclearin(yymparse_t YYFAR *yy);
void YYCDECL yymdestructclearin(yymparse_t YYFAR *yy);
void YYCDECL yymsetin(yymparse_t YYFAR *yy, int token);
int YYCDECL yymunclearin(yymparse_t YYFAR *yy);
#else
void YYCDECL yymcdestructclearin();
void YYCDECL yymdestructclearin();
void YYCDECL yymsetin();
int YYCDECL yymunclearin();
#endif
#define yymabort(yy) yymexit(1)
#define yymaccept(yy) yymexit(0)
#define yymclearin(yy) ((yy)->yymlookahead = 0)
#define yymerrok(yy) yymsetskip((yy), 0)
#define yymexit(yy, exitcode) ((yy)->yymexitflg = 1, (yy)->yymexitcode = (exitcode))
#define yymforceerror(yy) yymthrowerror(0)
#define yympopping(yy) (yy)->yympopflg
#define yymrecovering(yy) ((yy)->yymskip > 0)
#define yymretire(yy, retirecode) ((yy)->yymretireflg = 1, (yy)->yymretirecode = (retirecode))
#define yymthrowerror(yy, pop) ((yy)->yymerrorflg = 1, (yy)->yymerrorpop = (pop))

/* utility functions */
#ifdef YYPROTOTYPE
#ifdef YYDEBUG
void YYCDECL yympop(yymparse_t YYFAR *yy, int num);
void YYCDECL yymsetskip(yymparse_t YYFAR *yy, int skip);
#endif
int YYCDECL yympush(yymparse_t YYFAR *yy, yystack_t state);
#else
#ifdef YYDEBUG
void YYCDECL yympop();
void YYCDECL yymsetskip();
#endif
int YYCDECL yympush();
#endif
#ifndef yyassert
#define yyassert(expr) assert(expr)
#endif
#define yympeek(yy) ((yy)->yymstackptr[(yy)->yymtop])
#ifndef YYDEBUG
#define yympop(yy, num) ((yy)->yymtop -= (num))
#define yymsetskip(yy, skip) ((yy)->yymskip = (skip))
#endif

/* debugging functions */
#ifdef YYDEBUG
#ifdef YYPROTOTYPE
YYCONST char *YYCDECL yymtokenstring(yymparse_t YYFAR *yy, int token);
void YYCDECL yymdgettoken(yymparse_t YYFAR *yy, int token);
void YYCDECL yymdshift(yymparse_t YYFAR *yy, int token);
void YYCDECL yymdreduce(yymparse_t YYFAR *yy, int rule);
void YYCDECL yymdsyntaxerror(yymparse_t YYFAR *yy);
void YYCDECL yymdaccept(yymparse_t YYFAR *yy);
void YYCDECL yymdabort(yymparse_t YYFAR *yy);
void YYCDECL yymddiscard(yymparse_t YYFAR *yy, int token);
void YYCDECL yymdexit(yymparse_t YYFAR *yy, int exitcode);
void YYCDECL yymdthrowerror(yymparse_t YYFAR *yy, int errorpop);
void YYCDECL yymdretire(yymparse_t YYFAR *yy, int retirecode);
void YYCDECL yymdattemptrecovery(yymparse_t YYFAR *yy);
void YYCDECL yymparsedebugoutput(yymparse_t YYFAR *yy, YYCONST char *string);
#else
YYCONST char *YYCDECL yymtokenstring();
void YYCDECL yymdgettoken();
void YYCDECL yymdshift();
void YYCDECL yymdreduce();
void YYCDECL yymdsyntaxerror();
void YYCDECL yymdaccept();
void YYCDECL yymdabort();
void YYCDECL yymddiscard();
void YYCDECL yymdexit();
void YYCDECL yymdthrowerror();
void YYCDECL yymdretire();
void YYCDECL yymdattemptrecovery();
void YYCDECL yymparsedebugoutput();
#endif
#endif

#define yymisfastparser(yy) ((yy)->yymstateaction != NULL)
#define yymiscompactparser(yy) ((yy)->yymcstateaction != NULL)

/* debugging variables */
#ifdef YYDEBUG
extern int YYNEAR YYDCDECL yydebug;
extern int YYNEAR YYDCDECL yydebugstack;
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
