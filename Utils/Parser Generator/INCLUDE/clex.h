#ifndef CLEX_H
#define CLEX_H

/************************************************************
clex.h
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#include <stdio.h>
#include <assert.h>

// defines
#include <yytdefs.h>

// user defines
#if defined(YYTUDEFS) || defined(YYUDEFS)
#include <yytudefs.h>
#endif

#define YY_ALEX

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
class YYFAR yyparser;

// yylex return values
#define YYEOF 0			// end of file

#ifndef YYSTATE_T
#define YYSTATE_T
typedef struct yystate {
	short def;			// default state
	short base;			// base
	short match;		// action associated with state
} yystate_t;
#endif

#ifndef YYTRANSITION_T
#define YYTRANSITION_T
typedef struct yytransition {
	short next;			// next state on transition
	short check;		// check
} yytransition_t;
#endif

#ifndef YYCTRANSITION_T
#define YYCTRANSITION_T
typedef struct yyctransition {
	unsigned char first;	// first character in range
	unsigned char last;		// last character in range
	short next;				// next state on transition
} yyctransition_t;
#endif

typedef short yymatch_t;
typedef unsigned char yybackup_t;

class YYFAR yylexer {
public:
	yylexer();
	virtual ~yylexer();

// Attributes
protected:
	// left context
	int yystart;					// current start state
	unsigned char yyeol;			// whether an end-of-line '\n' has been seen
	unsigned char yyoldeol;			// previous end-of-line value

	// text buffer
	int YYFAR* yystatebuf;			// state buffer
	int YYFAR* yysstatebuf;			// initial (static) state buffer
	char YYFAR* yystext;			// initial (static) text buffer
	int yytext_size;				// text buffer size
	int yystext_size;				// initial (static) text buffer size

	// unput buffer
	int YYFAR* yyunputbufptr;		// unput buffer
	int YYFAR* yysunputbufptr;		// initial (static) unput buffer
	int yyunput_size;				// unput buffer size
	int yysunput_size;				// initial (static) unput buffer size
	int yyunputindex;				// unput buffer position

	// actions
	unsigned char yymoreflg;		// concatenate matched strings
	unsigned char yyrejectflg;		// yyreject called from an action
	unsigned char yyreturnflg;		// return from an action
public:
	yyparser YYFAR* yyparserptr;	// pointer to the attached parser

	// buffer flags
	unsigned char yytextgrow;		// whether text buffer is allowed to grow
	unsigned char yyunputgrow;		// whether unput buffer is allowed to grow

	// streams
	FILE YYFAR* yyin;				// input text stream
	FILE YYFAR* yyout;				// output text stream
	FILE YYFAR* yyerr;				// error stream

	// matched string
	char YYFAR* yytext;				// text buffer
	int yyleng;						// matched string length
	int yylineno;					// current line number

// Operations
protected:
	// helper functions
	int yyback(const yymatch_t YYNEARFAR* p, int action) const;
public:
	// instance functions
	int yycreate(yyparser YYFAR* parserptr = NULL);
	void yydestroy();

	// general functions
	void yycleanup();
	virtual int yylex() = 0;
	void yyreset();
	int yysettextsize(int size);
	int yysetunputsize(int size);

	// service functions
	virtual int yyinput();
	virtual void yyoutput(int ch);
	virtual void yyunput(int ch);
	virtual int yywrap();
	virtual int yygetchar();
	virtual void yytextoverflow();
	virtual void yyunputoverflow();
	virtual int yyaction(int action) = 0;

	// action functions
	void yyecho();
	void yyless(int length);
	void yybegin(int state) { yystart = state; }
	void yymore() { yymoreflg = 1; }
	void yynewline(int newline) { newline ? yyeol = 1 : (yyeol = 0); }
	void yyreject() { yyrejectflg = 1; }
	int yyunputcount() const { return yyunputindex; }

	// compatibility
	int yyclex() { return yylex(); }
	void yylexcleanup() { yycleanup(); }
	void yylexinit() { /* do nothing */ }
#define BEGIN yystart = 
#define ECHO yyecho()
#define REJECT yyreject()
#define YYSTATE yystart
#define YY_START yystart

// Tables
protected:
	const yymatch_t YYNEARFAR* yymatch;
	const yystate_t YYNEARFAR* yystate;
	const yybackup_t YYNEARFAR* yybackup;

// Debugging
#ifdef YYDEBUG
public:
	int yydebug;					// whether debug information should be output
	int yydebugflush;				// whether debug output should be flushed
	FILE YYFAR* yydebugout;			// debug output file
protected:
	void yydebugoutput(int ch) const;
	void yydmatch(int expr) const;
	void yydebugoutput(const char* string) const;
#endif
};

class YYFAR yyflexer : public yylexer {
public:
	yyflexer() { /* do nothing */ }

// Operations
public:
	virtual int yylex();

// Tables
protected:
	const yytransition_t YYNEARFAR* yytransition;
	int yytransitionmax;
};

class YYFAR yyclexer : public yylexer {
public:
	yyclexer() { /* do nothing */ }

// Operations
public:
	virtual int yylex();

// Tables
protected:
	const yyctransition_t YYNEARFAR* yyctransition;
};

// helper functions
#ifndef yyassert
#define yyassert(expr) assert(expr)
#endif

// debugging variables
#ifdef YYDEBUG
extern "C" int YYNEAR YYDCDECL yydebug;
extern "C" int YYNEAR YYDCDECL yydebugflush;
#endif

// user defines
#if defined(YYBUDEFS) || defined(YYUDEFS)
#include <yybudefs.h>
#endif

// defines
#include <yybdefs.h>

#endif
