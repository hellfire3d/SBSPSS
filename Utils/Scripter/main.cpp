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


/*	Std Lib
	------- */

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


int openInputFile(char *_filename)
{
	int	ret=1;

	if((yyin=fopen(_filename,"rt"))==NULL)
	{
		ret=0;
	}
	return ret;
}
int closeInputFile()
{
	fclose(yyin);
	return 1;
}



/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
extern int main(int argc, char *argv[])
{
	int		ret=-1;


	// Process args
	if(argc!=3)
	{
		printf("Script compiler thingy\n");
		printf("Usage: yl infile outfile\n");
		printf("So there..\n\n");
	}
	else
	{
		char	*inputfile;
		char	*outputfile;

		inputfile=argv[1];
		outputfile=argv[2];

		// Parse file
		printf("Parsing file: %s\n",inputfile);
		if(!openInputFile(inputfile))
		{
			printf("Cannot open input file!\n");
		}
		else
		{
			if(yyparse()!=YYEXIT_SUCCESS)
			{
				s_errorCount++;
			}
			closeInputFile();
			printf("%d line(s) parsed, %d error(s) found\n",s_linesProcessed,s_errorCount);

			// Generate code
			if(s_baseTreeNode&&!s_errorCount)
			{
				printf("Creating output file: %s\n",outputfile);
				if(!openOutputFile(outputfile))
				{
					printf("Couldn't open output file!\n");
				}
				else
				{
					int		codeSize=0;

					codeSize=genCode(s_baseTreeNode,true);
					closeOutputFile();
					printf("Generated %d instructions(s)\n",codeSize);
					ret=0;		// Huzzah!
				}
			}
		}
	}

	return ret;
}
/*
	int n=1;

openOutputFile("test.dat");
	mylexer lexer;
	myparser parser;
	if(parser.yycreate(&lexer))
	{
		if(lexer.yycreate(&parser))
		{
			n=parser.yyparse();
		}
	}

if(s_baseTreeNode)
{
	s_baseTreeNode->generateCode(true);
}
closeOutputFile();

	return n;
*/


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */


/*===========================================================================
 end */
