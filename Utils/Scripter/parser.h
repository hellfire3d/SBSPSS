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
#define COMMA 271
#define VARIABLE 272
#define VALUE 273
#define FUNCTION 274
#endif
