/*=========================================================================

	main.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/


/*----------------------------------------------------------------------
	Includes
	-------- */

#include "main.h"
#include "lexer.h"
#include "parser.h"

#ifndef __CODEGEN_H__
#include "codegen.h"
#endif


/*	Std Lib
	------- */

#include <yacc.h>


/*	Data
	---- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

/*----------------------------------------------------------------------
	Function Prototypes
	------------------- */

/*----------------------------------------------------------------------
	Vars
	---- */


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
extern int main(int argc, char *argv[])
{
	int ret;

	if(argc!=3)
	{
		printf("Script compiler thingy\n");
		printf("Usage: yl infile outfile\n");
		printf("So there..\n\n");
		ret=-1;
	}
	else
	{
		char	*infile;
		char	*outfile;
		
		infile=argv[1];
		outfile=argv[2];

		if(parseFile(infile,s_baseTreeNode)==YYEXIT_SUCCESS&&s_baseTreeNode)
		{
			if(openOutputFile(outfile))
			{
				int byteCount;
				byteCount=s_baseTreeNode->generateCode(true);
				closeOutputFile();
				printf("Generated %d bytes of code\n",byteCount*2);
				ret=0;
			}
			else
			{
				ret=-1;
			}
		}
		else
		{
			ret=-1;
		}
	}

	return ret;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int	mylexer::openInputFile(char *_filename)
{
	int	ret=1;

	printf("Opening %s..\n",_filename);
	if((m_fhInput=fopen(_filename,"rt"))==NULL)
	{
		printf("FATAL: Couldn't open file for reading\n");
		ret=0;
	}
	else
	{
		m_charCount=0;
		m_lineCount=0;
		m_currentCharOnLine=0;
		m_errorCount=0;
	}
	return ret;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int mylexer::closeInputFile()
{
	printf("Processed %d char(s), %d line(s)\n",m_charCount,m_lineCount);
	if(m_errorCount)
	{
		printf("Found %d error(s) :(\n",m_errorCount);
	}
	fclose(m_fhInput);
	return 1;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int mylexer::yygetchar()
{
	char	c;
	int		ret;

	if(fread(&c,sizeof(c),1,m_fhInput)==1)
	{
		m_charCount++;
		m_currentCharOnLine++;
		if(c=='\n')
		{
			m_lineCount++;
			m_currentCharOnLine=0;
		}
		ret=c;
	}
	else
	{
		if(ferror(m_fhInput))
		{
			printf("FATAL: Read error!\n");
		}
		ret=-1;
	}

	// Force compilation to stop after finding errors ( hmm.. )
	if(m_errorCount)
	{
		printf("Stopping compilation!\n");
		ret=-1;
	}
	
	return ret;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void myparser::setCurrentLexer(mylexer *_lexer)
{
	m_currentLexer=_lexer;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void myparser::setBaseNode(class CTreeNode *_baseNode)
{
	m_baseNode=_baseNode;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void myparser::yyerror(const char *_text)
{
	fprintf(yyparseerr,"ERROR: %s\n",_text);
	fprintf(yyparseerr,"       line %d char %d\n",m_currentLexer->getCurrentLine(),m_currentLexer->getCurrentCharOnLine());
	m_currentLexer->error();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int myparser::yyparse()
{
	int	ret=YYEXIT_FAILURE;

	if(yysetup()==0&&
	   yywork()==YYEXIT_SUCCESS&&
	   m_currentLexer->getErrorCount()==0)
	{
		ret=YYEXIT_SUCCESS;
	}

	return ret;
}


/*===========================================================================
 end */
