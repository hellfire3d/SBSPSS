#ifndef _LEXER_H
#define _LEXER_H

#include <clex.h>

/////////////////////////////////////////////////////////////////////////////
// mylexer

class YYFAR mylexer : public yyflexer {
public:
	mylexer();

protected:
	void yytables();
	virtual int yyaction(int action);

public:
#line 23 "C:\\spongebob\\Utils\\scripter\\lexer.l"

	// place any extra class members here

#line 23 "C:\\spongebob\\Utils\\scripter\\lexer.h"
};

#ifndef YYLEXNAME
#define YYLEXNAME mylexer
#endif

#ifndef INITIAL
#define INITIAL 0
#endif

#endif
