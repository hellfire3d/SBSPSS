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

#ifndef __PFILE_H__
#include "pfile.h"
#endif


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

static int ppf_define(char *_cmd);
static int ppf_include(char *_cmd);
static int ppf_print(char *_cmd);
static int ppf_undefine(char *_cmd);

static int addMacro(char *_name,char *_replacement);
static int removeMacro(char *_name);


/*----------------------------------------------------------------------
	Vars
	---- */
static PreproCmd s_preproCmds[]=
{
	{	"define",	ppf_define		},
	{	"include",	ppf_include		},
	{	"print",	ppf_print		},
	{	"undef",	ppf_undefine	},
};
static int		s_numPreproCmds=sizeof(s_preproCmds)/sizeof(PreproCmd);


static Macro	*s_macros;

static char	s_seps[]=" \t";



/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:	true on success, false on failure
  ---------------------------------------------------------------------- */
extern int preprocessorCmd(char *_cmd)
{
	int			i;
	PreproCmd	*pp;

	pp=s_preproCmds;
	for(i=0;i<s_numPreproCmds;i++)
	{
		if(strncmp(_cmd,pp->m_cmd,strlen(pp->m_cmd))==0)
		{
			return pp->m_func(_cmd);
		}
		pp++;
	}

	printf("UNKNOWN PREPROCESSOR CMD '%s'\n",_cmd);
	return true;
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
	
	return openPFile(includeFile);
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
	
	return true;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
static int ppf_undefine(char *_cmd)
{
	char	*macroName;
	
	macroName=strtok(_cmd,s_seps);
	macroName=strtok(NULL,s_seps);
	
	return removeMacro(macroName);
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
	while(mac)
	{
		if(strcmp(_name,mac->m_name)==0)
		{
			printf("MACRO '%s' ALREADY DEFINED\n",_name);
			return false;
		}
		mac=mac->m_next;
	}

	newMac=(Macro*)malloc(sizeof(Macro));
	newMac->m_name=(char*)malloc(strlen(_name)+1);
	newMac->m_replacement=(char*)malloc(strlen(_replacement)+1);
	strcpy(newMac->m_name,_name);
	strcpy(newMac->m_replacement,_replacement);
	newMac->m_next=s_macros;
	s_macros=newMac;

	return true;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
static int removeMacro(char *_name)
{
	Macro	*mac,*prev;

	mac=s_macros;
	prev=NULL;
	while(mac)
	{
		if(strcmp(_name,mac->m_name)==0)
		{
			if(prev)
			{
				prev->m_next=mac->m_next;
			}
			else
			{
				s_macros=mac->m_next;
			}
			free(mac->m_name);
			free(mac->m_replacement);
			free(mac);
			return true;
		}
		prev=mac;
		mac=mac->m_next;
	}

	printf("MACRO '%s' NOT DEFINED, CANNOT UNDEF IT\n",_name);
	return false;
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
