/*=========================================================================

	var.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/


/*----------------------------------------------------------------------
	Includes
	-------- */

#include "var.h"

#ifndef _LEXER_H
#include "lexer.h"
#endif


/*	Std Lib
	------- */

#include <stdio.h>
#include <string.h>


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

static char *s_globalVarNames[]=
{
	"lives",
};
static int s_numGlobalVarNames=sizeof(s_globalVarNames)/sizeof(char *);

static char *s_localVarNames[]=
{
	"tmp0",
	"tmp1",
	"tmp2",
	"tmp3",
	"tmp4",
	"tmp5",
	"tmp6",
	"tmp7",
	"tmp8",
	"tmp9",
};
static int s_numLocalVarNames=sizeof(s_localVarNames)/sizeof(char *);

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
extern mylexer s_lexer;
extern int lookupVarName(char *_name)
{
	int	i,vnum;

	vnum=0;

	// Global vars
	for(i=0;i<s_numGlobalVarNames;i++,vnum++)
	{
		if(strcmp(_name,s_globalVarNames[i])==0)
		{
			return vnum;
		}
	}

	// Local vars
	for(i=0;i<s_numLocalVarNames;i++,vnum++)
	{
		if(strcmp(_name,s_localVarNames[i])==0)
		{
			return vnum;
		}
	}

	printf("Unknown variable $%s\n",_name);
	s_lexer.error();
	return -1;
}


/*===========================================================================
 end */
