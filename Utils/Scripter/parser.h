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
#line 23 "C:\\spongebob\\Utils\\scripter\\parser.y"

public:
	void	setCurrentLexer(class mylexer *_lexer);
	void	setBaseNode(class CTreeNode *_baseNode);

	// Overridden parser functions
	void	yyerror(const char *_text);
	int		yyparse();

private:
	class mylexer	*m_currentLexer;
	class CTreeNode	*m_baseNode;

	// Ugh! (pkg)
	class CTreeNode	*m_functionArgs;
	int				m_functionNumber;


#line 42 "C:\\spongebob\\Utils\\scripter\\parser.h"
};

#ifndef YYPARSENAME
#define YYPARSENAME myparser
#endif

#ifndef YYSTYPE
union tagYYSTYPE {
#line 54 "C:\\spongebob\\Utils\\scripter\\parser.y"

	int					variableIdx;
	signed short		value;
	int					functionNumber;
	class CTreeNode		*treenode;

#line 58 "C:\\spongebob\\Utils\\scripter\\parser.h"
};

#define YYSTYPE union tagYYSTYPE
#endif

#define STOP 257
#define IF 258
#define ELSE 259
#define WHILE 260
#define DO 261
#define PAUSE 262
#define PRINT 263
#define ASSIGN 264
#define EQUAL 265
#define NOTEQUAL 266
#define LESSTHAN 267
#define GREATERTHAN 268
#define PLUS 269
#define SUBTRACT 270
#define END_STMT 271
#define OPEN_PAR 272
#define CLOSE_PAR 273
#define BEGIN_CS 274
#define END_CS 275
#define COMMA 276
#define VARIABLE 277
#define VALUE 278
#define FUNCTION 279
#endif
