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

#ifndef _LEXER_H
#include "lexer.h"
#endif

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

		printf("Scripter attempting to compile %s..\n",infile);
		if(parseFile(infile,s_baseTreeNode)==YYEXIT_SUCCESS&&s_baseTreeNode)
		{
			if(openOutputFile(outfile))
			{
				int byteCount;
				byteCount=s_baseTreeNode->generateCode(true);
				closeOutputFile();
				printf("Generated %d bytes of code in %s\n",byteCount*2,outfile);
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

	printf("\n");
	return ret;
}


/*===========================================================================
 end */
