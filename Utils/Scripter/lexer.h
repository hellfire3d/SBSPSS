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
#line 24 "C:\\spongebob\\Utils\\scripter\\lexer.l"

	int	openInputFile(char *_filename);
	int closeInputFile();

	void			setCurrentParser(class myparser *_parser)	{m_currentParser=_parser;}
	class myparser	*getCurrentParser()							{return m_currentParser;}

	int		getCurrentLine()		{return m_lineCount+1;}
	int		getCurrentCharOnLine()	{return m_currentCharOnLine;}
	int		getErrorCount()			{return m_errorCount;}

	void	error()					{m_errorCount++;}


	// Overridden lexer functions
	int		yygetchar();

private:
//	FILE	*m_fhInput;
	int		m_charCount;
	int		m_lineCount;
	int		m_currentCharOnLine;
	int		m_errorCount;

	class myparser	*m_currentParser;

#line 46 "C:\\spongebob\\Utils\\scripter\\lexer.h"
};

#ifndef YYLEXNAME
#define YYLEXNAME mylexer
#endif

#ifndef INITIAL
#define INITIAL 0
#endif

#endif
