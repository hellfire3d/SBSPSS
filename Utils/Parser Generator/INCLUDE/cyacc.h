#ifndef CYACC_H
#define CYACC_H

/************************************************************
cyacc.h
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include <stdio.h>
#include <stddef.h>
#include <assert.h>

// defines
#include <yytdefs.h>

// user defines
#if defined(YYTUDEFS) || defined(YYUDEFS)
#include <yytudefs.h>
#endif

#define YY_AYACC

// modifiers
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

// forward references
class YYFAR yylexer;

typedef short yystack_t;

// yyparse return values
#define YYEXIT_SUCCESS	0
#define YYEXIT_FAILURE	1

// common tokens
#define YYTK_ALL		(-1)		// match all tokens
#define YYTK_END		0			// $end token
#define YYTK_ERROR		256			// error token

#ifndef YYCSTATEGOTO_T
#define YYCSTATEGOTO_T
typedef short yycstategoto_t;
#endif

#ifndef YYCNONTERMGOTO_T
#define YYCNONTERMGOTO_T
typedef struct yycnontermgoto {
	short nonterm;					// nonterminal
	short next;						// next state
} yycnontermgoto_t;
#endif

#ifndef YYSTATEGOTO_T
#define YYSTATEGOTO_T
typedef struct yystategoto {
	short base;						// base
	short def;						// default state
} yystategoto_t;
#endif

#ifndef YYNONTERMGOTO_T
#define YYNONTERMGOTO_T
typedef struct yynontermgoto {
	short check;					// check
	short next;						// next state
} yynontermgoto_t;
#endif

// action types
#define YYAT_SHIFT		0			// shift action
#define YYAT_REDUCE		1			// reduce action
#define YYAT_ERROR		2			// error
#define YYAT_ACCEPT		3			// accept
#define YYAT_DEFAULT	4			// default state

#ifndef YYCSTATEACTION_T
#define YYCSTATEACTION_T
typedef short yycstateaction_t;
#endif

#ifndef YYCTOKENACTION_T
#define YYCTOKENACTION_T
typedef struct yyctokenaction {
	int token;						// lookahead token
	unsigned char type;				// action to perform
	short sr;						// state to shift/production to reduce
} yyctokenaction_t;
#endif

#ifndef YYSTATEACTION_T
#define YYSTATEACTION_T
typedef struct yystateaction {
	short base;						// base
	unsigned char lookahead;		// lookahead needed
	unsigned char type;				// action to perform
	short sr;						// shift/reduce
} yystateaction_t;
#endif

#ifndef YYTOKENACTION_T
#define YYTOKENACTION_T
typedef struct yytokenaction {
	short check;					// check
	unsigned char type;				// action type
	short sr;						// shift/reduce
} yytokenaction_t;
#endif

// nonterminals
#define YYNT_ALL		(-1)		// match all nonterminals

// states
#define YYST_ERROR		(-1)		// goto error

#ifndef YYREDUCTION_T
#define YYREDUCTION_T
typedef struct yyreduction {
	short nonterm;					// the rhs symbol
	short length;					// number of symbols on lhs
	short action;					// the user action
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

// debugging
#ifdef YYDEBUG
#ifndef YYSYMBOL_T
#define YYSYMBOL_T
typedef struct yysymbol {
	const char* name;				// symbol name
	int token;						// symbol token
} yysymbol_t;
#endif
#endif

class YYFAR yyparser {
public:
	yyparser();
	virtual ~yyparser();

// Attributes
protected:
	// stack
	yystack_t YYFAR* yystackptr;		// (state) stack
	yystack_t YYFAR* yysstackptr;		// static (state) stack
	void YYFAR* yyattributestackptr;	// attribute stack
	void YYFAR* yysattributestackptr;	// static attribute stack
	int yystack_size;					// number of elements in stack
	int yysstack_size;					// initial number of elements in stack

	int yytop;							// the current top of the stack
	size_t yyattribute_size;			// size of attribute

	void YYFAR* yyvalptr;				// attribute for $$

	// lookahead token
	unsigned char yylookahead;			// whether current lookahead token is valid
	int yychar;							// current lookahead token

	// error recovery
	unsigned char yywipeflg;			// whether to "wipe" stack on abort
	unsigned char yypopflg;				// popping symbols during error recovery
	int yyskip;							// error recovery token shift counter

	// actions
	unsigned char yyexitflg;			// whether yymexit called
	unsigned char yyretireflg;			// whether yymretire called
	unsigned char yyerrorflg;			// whether yymforceerror called
	int yyexitcode;						// yymexit exit code
	int yyretirecode;					// yymretire exit code
	int yyerrorpop;						// how many error transitions to pop
public:
	yylexer YYFAR* yylexerptr;			// pointer to the attached lexical analyser

	unsigned char yystackgrow;			// whether stack can grow
	void YYFAR* yylvalptr;				// current token attribute
	FILE YYFAR* yyerr;					// error output file
	int yyerrorcount;					// how many syntax errors have occurred

// Operations
protected:
	virtual void yyaction(int action) = 0;

// utility functions
#ifdef YYDEBUG
	void yypop(int num);
	void yysetskip(int skip);
#else
	void yypop(int num) { yytop -= num; }
	void yysetskip(int skip) { yyskip = skip; }
#endif
	int yypush(yystack_t state);
	yystack_t yypeek() const { return yystackptr[yytop]; }

public:
	// instance functions
	int yycreate(yylexer YYFAR* lexerptr = NULL);
	void yydestroy();

	// general functions
	void yydestructpop(int num);
	int yyparse();
	void yycleanup();
	int yysetstacksize(int size);
	int yysetup();
	void yywipe();
	virtual int yywork() = 0;
	virtual void yydestructclearin() = 0;

	// service functions
	virtual void yystackoverflow();
	virtual void yyerror(const char YYFAR* text);
	virtual void yysyntaxerror();
	virtual void yydiscard(int token);
	virtual int yygettoken();

	// action functions
	void yysetin(int token);
	int yyunclearin();
	void yyabort() { yyexit(1); }
	void yyaccept() { yyexit(0); }
#ifndef YY_COMPATIBLE
	void yyclearin() { yylookahead = 0; }
	void yyerrok() { yysetskip(0); }
#else
	void _yyclearin() { yylookahead = 0; }
	void _yyerrok() { yysetskip(0); }
#endif
	void yyexit(int exitcode) { yyexitflg = 1; yyexitcode = exitcode; }
	void yyforceerror() { yythrowerror(0); }
	int yypopping() const { return yypopflg; }
	int yyrecovering() const { return yyskip > 0; }
	void yyretire(int retirecode) { yyretireflg = 1; yyretirecode = retirecode; }
	void yythrowerror(int pop) { yyerrorflg = 1; yyerrorpop = pop; }

	// compatibility
	int yycparse() { return yyparse(); }
	int yycwork() { return yywork(); }
	void yyparsecleanup() { yycleanup(); }
	void yyparseinit() { /* do nothing */ }
#ifdef YY_COMPATIBLE
#define yyclearin _yyclearin()
#define yyerrok _yyerrok()
#endif
#define YYABORT yyexit(1)
#define YYACCEPT yyexit(0)
#define YYERROR yyforceerror()
#define YYRECOVERING yyrecovering()

// Tables
protected:
	const yyreduction_t YYNEARFAR* yyreduction;
	const yydestructor_t YYNEARFAR* yydestructorptr;

// Debugging
#ifdef YYDEBUG
public:
	int yydebug;				// whether debug information should be output
	int yydebugstack;			// whether stack debug information should be output
	int yydebugflush;			// whether debug output should be flushed
	FILE YYFAR* yydebugout;		// debug output file
protected:
	const yysymbol_t YYNEARFAR* yysymbol;
	const char* const YYNEARFAR* yyrule;

	// debugging functions
protected:
	const char* yytokenstring(int token) const;
	void yydgettoken(int token) const;
	void yydshift(int token) const;
	void yydreduce(int rule) const;
	void yydsyntaxerror() const;
	void yydaccept() const;
	void yydabort() const;
	void yyddiscard(int token) const;
	void yydexit(int exitcode) const;
	void yydthrowerror(int errorpop) const;
	void yydretire(int retirecode) const;
	void yydattemptrecovery() const;
	void yydebugoutput(const char *string) const;
#endif
};

class YYFAR yyfparser : public yyparser {
public:
	yyfparser() { /* do nothing */ }

// Operations
public:
	virtual int yywork();
	virtual void yydestructclearin();

// Tables
protected:
	const yystateaction_t YYNEARFAR* yystateaction;
	const yytokenaction_t YYNEARFAR* yytokenaction;
	int yytokenaction_size;
	const yystategoto_t YYNEARFAR* yystategoto;
	const yynontermgoto_t YYNEARFAR* yynontermgoto;
	int yynontermgoto_size;
	const yytokendest_t YYNEARFAR* yytokendestptr;
	int yytokendest_size;
	int yytokendestbase;
};

class YYFAR yycparser : public yyparser {
public:
	yycparser() { /* do nothing */ }

// Operations
public:
	virtual int yywork();
	virtual void yydestructclearin();

// Tables
protected:
	const yycstateaction_t YYNEARFAR* yycstateaction;
	const yyctokenaction_t YYNEARFAR* yyctokenaction;
	const yycstategoto_t YYNEARFAR* yycstategoto;
	const yycnontermgoto_t YYNEARFAR* yycnontermgoto;
	const yyctokendest_t YYNEARFAR* yyctokendestptr;
};

// utility functions
#ifndef yyassert
#define yyassert(expr) assert(expr)
#endif

// debugging variables
#ifdef YYDEBUG
extern "C" int YYNEAR YYDCDECL yydebug;
extern "C" int YYNEAR YYDCDECL yydebugstack;
extern "C" int YYNEAR YYDCDECL yydebugflush;
#endif

// user defines
#if defined(YYBUDEFS) || defined(YYUDEFS)
#include <yybudefs.h>
#endif

// defines
#include <yybdefs.h>

#endif
