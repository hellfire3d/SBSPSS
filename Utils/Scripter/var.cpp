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
	"tmp1",
	"tmp2",
	"tmp3",
};
static int s_numLocalVarNames=sizeof(s_localVarNames)/sizeof(char *);

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
extern int lookupVarName(char *_name)
{
	int	i,vnum;

	vnum=0;

	// Global vars
	for(i=0;i<s_numGlobalVarNames;i++,vnum++)
	{
		if(stricmp(_name,s_globalVarNames[i])==0)
		{
			return vnum;
		}
	}

	// Local vars
	for(i=0;i<s_numLocalVarNames;i++,vnum++)
	{
		if(stricmp(_name,s_localVarNames[i])==0)
		{
			return vnum;
		}
	}

	printf("Unknown variable $%s\n",_name);
//	s_errorCount++;
	return -1;
}


/*===========================================================================
 end */
