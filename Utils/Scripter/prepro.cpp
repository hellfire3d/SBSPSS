/*=========================================================================

	prepro.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/


/*----------------------------------------------------------------------
	Includes
	-------- */

#include "prepro.h"


/*	Std Lib
	------- */

#include <stdlib.h>
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
typedef struct
{
	char	*m_cmd;
	int		(*m_func)(char *_cmd);
} PreproCmd;


typedef struct _macro
{
	char	*m_name;
	char	*m_replacement;
	_macro	*m_next;
} Macro;


/*----------------------------------------------------------------------
	Function Prototypes
	------------------- */

static int	ppf_define(char *_cmd);
static int	ppf_include(char *_cmd);
static int	ppf_print(char *_cmd);

static int addMacro(char *_name,char *_replacement);


/*----------------------------------------------------------------------
	Vars
	---- */
static PreproCmd s_preproCmds[]=
{
	{	"define",	ppf_define	},
	{	"include",	ppf_include	},
	{	"print",	ppf_print	},
};
static int		s_numPreproCmds=sizeof(s_preproCmds)/sizeof(PreproCmd);


static Macro	*s_macros;

static char	s_seps[]=" \t";



/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
extern int preprocessorCmd(char *_cmd)
{
	int			i;
	PreproCmd	*pp;

	pp=s_preproCmds;
	for(i=0;i<s_numPreproCmds;i++)
	{
		if(strnicmp(_cmd,pp->m_cmd,strlen(pp->m_cmd))==0)
		{
			return pp->m_func(_cmd);
		}
		pp++;
	}

	printf("UNKNOWN PREPROCESSOR CMD '%s'\n",_cmd);
	return 1;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
static int ppf_define(char *_cmd)
{
	char	*macroName;
	char	*macroReplacement;

	macroName=strtok(_cmd,s_seps);
	macroName=strtok(NULL,s_seps);
	macroReplacement=strtok(NULL,s_seps);

	return addMacro(macroName,macroReplacement);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
static int ppf_include(char *_cmd)
{
	char	*includeFile;
	
	includeFile=strtok(_cmd,s_seps);
	includeFile=strtok(NULL,s_seps);
	
//	printf("include: '%s'\n",includeFile);
	
	return 0;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
static int ppf_print(char *_cmd)
{
	char	*printMessage;
	
	printMessage=strtok(_cmd,s_seps);
	printMessage=strtok(NULL,s_seps);
	
	printf("#print %s\n",printMessage);
	
	return 0;
}




/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
static int addMacro(char *_name,char *_replacement)
{
	Macro	*mac,*newMac;

	mac=s_macros;
	while(mac&&mac->m_next)
	{
		if(strcmp(_name,mac->m_replacement)==0)
		{
			printf("MACRO '%s' ALREADY DEFINED\n",_name);
			return -1;
		}
	}

	newMac=(Macro*)malloc(sizeof(Macro));
	newMac->m_name=(char*)malloc(strlen(_name+1));
	newMac->m_replacement=(char*)malloc(strlen(_name+1));
	strcpy(newMac->m_name,_name);
	strcpy(newMac->m_replacement,_replacement);
	newMac->m_next=s_macros;
	s_macros=newMac;

	return 0;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
extern char *lookupMacro(char *_name)
{
	return NULL;
}



/*===========================================================================
 end */
