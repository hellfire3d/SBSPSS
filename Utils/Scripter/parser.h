#ifndef _PARSER_H
#define _PARSER_H

#include <cyacc.h>

/////////////////////////////////////////////////////////////////////////////
// myparser

class YYFAR myparser : public yyfparser {
public:
	myparser();

protected:
	void yytables();
	virtual void yyaction(int action);
#ifdef YYDEBUG
	void YYFAR* yyattribute1(int index) const;
	void yyinitdebug(void YYFAR** p, int count) const;
#endif

public:
#line 22 "C:\\spongebob\\Utils\\scripter\\parser.y"

	// place any extra class members here

#line 27 "C:\\spongebob\\Utils\\scripter\\parser.h"
};

#ifndef YYPARSENAME
#define YYPARSENAME myparser
#endif

#ifndef YYSTYPE
union tagYYSTYPE {
#line 38 "C:\\spongebob\\Utils\\scripter\\parser.y"

	int				variableIdx;
	signed short	value;
	class CTreeNode	*treenode;

#line 42 "C:\\spongebob\\Utils\\scripter\\parser.h"
};

#define YYSTYPE union tagYYSTYPE
#endif

#define STOP 257
#define IF 258
#define ELSE 259
#define PAUSE 260
#define PRINT 261
#define ASSIGN 262
#define EQUAL 263
#define NOTEQUAL 264
#define PLUS 265
#define END_STMT 266
#define OPEN_PAR 267
#define CLOSE_PAR 268
#define BEGIN_CS 269
#define END_CS 270
#define VARIABLE 271
#define VALUE 272
#endif
