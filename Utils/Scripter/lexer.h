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
#line 25 "C:\\spongebob\\Utils\\scripter\\lexer.l"

	int	openInputFile(char *_filename);
	int closeInputFile();

	void			setCurrentParser(class myparser *_parser)	{m_currentParser=_parser;}
	class myparser	*getCurrentParser()							{return m_currentParser;}

	void			error();
	int				getErrorCount();

	// Overridden lexer functions
	int				yygetchar();

private:
	void			unexpectedChar();
	void			comment();


	class myparser	*m_currentParser;

#line 40 "C:\\spongebob\\Utils\\scripter\\lexer.h"
};

#ifndef YYLEXNAME
#define YYLEXNAME mylexer
#endif

#ifndef INITIAL
#define INITIAL 0
#endif

#endif
