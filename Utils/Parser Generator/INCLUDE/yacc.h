#ifndef YACC_H
#define YACC_H

/************************************************************
yacc.h
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
#define YYNT_ALL		(-1)		/* match all nonterminals */

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

/* general functions */
#ifdef YYPROTOTYPE
int YYCDECL yycparse(void);
void YYCDECL yycwipe(void);
int YYCDECL yycwork(void);
void YYCDECL yydestructpop(int num);
int YYCDECL yyparse(void);
void YYCDECL yyparsecleanup(void);
void YYCDECL yyparseinit(void);
int YYCDECL yysetstacksize(int size);
int YYCDECL yysetup(void);
void YYCDECL yywipe(void);
int YYCDECL yywork(void);
#else
int YYCDECL yycparse();
void YYCDECL yycwipe();
int YYCDECL yycwork();
void YYCDECL yydestructpop();
int YYCDECL yyparse();
void YYCDECL yyparsecleanup();
void YYCDECL yyparseinit();
int YYCDECL yysetstacksize();
int YYCDECL yysetup();
void YYCDECL yywipe();
int YYCDECL yywork();
#endif

/* service functions */
#ifdef YYPROTOTYPE
void YYCDECL yydiscard(int token);
void YYCDECL yyerror(YYCONST char YYFAR *text);
int YYCDECL yygettoken(void);
void YYCDECL yystackoverflow(void);
void YYCDECL yysyntaxerror(void);
#else
void YYCDECL yydiscard();
void YYCDECL yyerror();
int YYCDECL yygettoken();
void YYCDECL yystackoverflow();
void YYCDECL yysyntaxerror();
#endif

/* action functions */
#ifdef YYPROTOTYPE
void YYCDECL yycdestructclearin(void);
void YYCDECL yydestructclearin(void);
void YYCDECL yysetin(int token);
int YYCDECL yyunclearin(void);
#else
void YYCDECL yycdestructclearin();
void YYCDECL yydestructclearin();
void YYCDECL yysetin();
int YYCDECL yyunclearin();
#endif
#define yyabort() yyexit(1)
#define yyaccept() yyexit(0)
#define yyclearin() (yylookahead = 0)
#define yyerrok() yysetskip(0)
#define yyexit(exitcode) (yyexitflg = 1, yyexitcode = (exitcode))
#define yyforceerror() yythrowerror(0)
#define yypopping() yypopflg
#define yyrecovering() (yyskip > 0)
#define yyretire(retirecode) (yyretireflg = 1, yyretirecode = (retirecode))
#define yythrowerror(pop) (yyerrorflg = 1, yyerrorpop = (pop))

/* compatibility */
#ifdef YY_COMPATIBLE
#undef yyclearin
#define yyclearin (yylookahead = 0)
#undef yyerrok
#define yyerrok yysetskip(0)
#endif
#define YYABORT yyexit(1)
#define YYACCEPT yyexit(0)
#define YYERROR yyforceerror()
#define YYRECOVERING yyrecovering()

/* helper functions */
#ifdef YYPROTOTYPE
#ifdef YYDEBUG
void YYCDECL yypop(int num);
void YYCDECL yysetskip(int skip);
#endif
int YYCDECL yypush(yystack_t state);
#else
#ifdef YYDEBUG
void YYCDECL yypop();
void YYCDECL yysetskip();
#endif
int YYCDECL yypush();
#endif
#ifndef yyassert
#define yyassert(expr) assert(expr)
#endif
#define yypeek() (yystackptr[yytop])
#ifndef YYDEBUG
#define yypop(num) (yytop -= (num))
#define yysetskip(skip) (yyskip = (skip))
#endif

/* variables */
extern unsigned char YYNEAR YYDCDECL yylookahead;
extern unsigned char YYNEAR YYDCDECL yystackgrow;
extern unsigned char YYNEAR YYDCDECL yyexitflg;
extern unsigned char YYNEAR YYDCDECL yyretireflg;
extern unsigned char YYNEAR YYDCDECL yyerrorflg;
extern unsigned char YYNEAR YYDCDECL yypopflg;
extern unsigned char YYNEAR YYDCDECL yywipeflg;
extern int YYNEAR YYDCDECL yytop;				/* the current top of the stack */
extern int YYNEAR YYDCDECL yychar;
extern int YYNEAR YYDCDECL yyskip;
extern int YYNEAR YYDCDECL yyerrorcount;
extern FILE YYFAR *YYNEAR YYDCDECL yyparseerr;
extern int YYNEAR YYDCDECL yyexitcode;
extern int YYNEAR YYDCDECL yyretirecode;
extern int YYNEAR YYDCDECL yyerrorpop;

/* debugging functions */
#ifdef YYDEBUG
#ifdef YYPROTOTYPE
YYCONST char *YYCDECL yytokenstring(int token);
void YYCDECL yydgettoken(int token);
void YYCDECL yydshift(int token);
void YYCDECL yydreduce(int rule);
void YYCDECL yydsyntaxerror(void);
void YYCDECL yydaccept(void);
void YYCDECL yydabort(void);
void YYCDECL yyddiscard(int token);
void YYCDECL yydexit(int exitcode);
void YYCDECL yydthrowerror(int errorpop);
void YYCDECL yydretire(int retirecode);
void YYCDECL yydattemptrecovery(void);
void YYCDECL yyparsedebugoutput(YYCONST char *string);
#else
YYCONST char *YYCDECL yytokenstring();
void YYCDECL yydgettoken();
void YYCDECL yydshift();
void YYCDECL yydreduce();
void YYCDECL yydsyntaxerror();
void YYCDECL yydaccept();
void YYCDECL yydabort();
void YYCDECL yyddiscard();
void YYCDECL yydexit();
void YYCDECL yydthrowerror();
void YYCDECL yydretire();
void YYCDECL yydattemptrecovery();
void YYCDECL yyparsedebugoutput();
#endif
#endif

/* debugging variables */
#ifdef YYDEBUG
extern int YYNEAR YYDCDECL yydebug;
extern int YYNEAR YYDCDECL yydebugstack;
extern int YYNEAR YYDCDECL yydebugflush;
extern int YYNEAR YYDCDECL yyparsedebug;
extern int YYNEAR YYDCDECL yyparsedebugstack;
extern int YYNEAR YYDCDECL yyparsedebugflush;
extern FILE YYFAR *YYNEAR YYDCDECL yyparsedebugout;
extern YYCONST yysymbol_t YYNEARFAR YYDCDECL yysymbol[];
extern YYCONST char *YYCONST YYNEARFAR YYDCDECL yyrule[];
#endif

/* externally defined */
#ifdef YYPROTOTYPE
int YYCDECL yylex(void);
void YYCDECL yyparseaction(int action);
#else
int YYCDECL yylex();
void YYCDECL yyparseaction();
#endif

extern yystack_t YYFAR *YYNEAR YYDCDECL yystackptr;			/* (state) stack */
extern yystack_t YYFAR *YYNEAR YYDCDECL yysstackptr;		/* static (state) stack */
extern int YYNEAR YYDCDECL yystack_size;					/* number of elements in stack */
extern int YYNEAR YYDCDECL yysstack_size;					/* initial number of elements in stack */

extern size_t YYNEAR YYDCDECL yyattribute_size;				/* size of attribute */

#ifdef YYPROTOTYPE
extern void YYFAR *YYNEAR YYDCDECL yyvalptr;				/* attribute for $$ */
extern void YYFAR *YYNEAR YYDCDECL yylvalptr;				/* current token attribute */
extern void YYFAR *YYNEAR YYDCDECL yyattributestackptr;		/* attribute stack */
extern void YYFAR *YYNEAR YYDCDECL yysattributestackptr;	/* static attribute stack */
#else
extern char YYFAR *YYNEAR YYDCDECL yyvalptr;				/* attribute for $$ */
extern char YYFAR *YYNEAR YYDCDECL yylvalptr;				/* current token attribute */
extern char YYFAR *YYNEAR YYDCDECL yyattributestackptr;		/* attribute stack */
extern char YYFAR *YYNEAR YYDCDECL yysattributestackptr;	/* static attribute stack */
#endif

/* compact parser */
extern YYCONST yycstateaction_t YYNEARFAR YYDCDECL yycstateaction[];
extern YYCONST yyctokenaction_t YYNEARFAR YYDCDECL yyctokenaction[];
extern YYCONST yycstategoto_t YYNEARFAR YYDCDECL yycstategoto[];
extern YYCONST yycnontermgoto_t YYNEARFAR YYDCDECL yycnontermgoto[];
extern YYCONST yyctokendest_t YYNEARFAR *YYNEAR YYDCDECL yyctokendestptr;

/* fast parser */
extern YYCONST yystateaction_t YYNEARFAR YYDCDECL yystateaction[];
extern YYCONST yytokenaction_t YYNEARFAR YYDCDECL yytokenaction[];
extern int YYNEAR YYDCDECL yytokenaction_size;
extern YYCONST yystategoto_t YYNEARFAR YYDCDECL yystategoto[];
extern YYCONST yynontermgoto_t YYNEARFAR YYDCDECL yynontermgoto[];
extern int YYNEAR YYDCDECL yynontermgoto_size;
extern YYCONST yytokendest_t YYNEARFAR *YYNEAR YYDCDECL yytokendestptr;
extern int YYNEAR YYDCDECL yytokendest_size;
extern int YYNEAR YYDCDECL yytokendestbase;

extern YYCONST yyreduction_t YYNEARFAR YYDCDECL yyreduction[];

extern YYCONST yydestructor_t YYNEARFAR *YYNEAR YYDCDECL yydestructorptr;

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
