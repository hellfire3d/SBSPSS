/* ===========================================================================
	File:		GDEBUG.C

	Notes:		PSX Implementation of glib debug api

	Author:		G Robert Liddon @ 73b

	Created:	Wednesday 27th March 1996

	Copyright (C) 1996 DCI Ltd All rights reserved. 
  ============================================================================ */


/* ---------------------------------------------------------------------------
	Standard Lib Includes
	--------------------- */
#include "stdio.h"

/* ---------------------------------------------------------------------------
	Glib Includes
	------------- */
#include "gdebug.h"

/* ---------------------------------------------------------------------------
	Game Includes
	------------- */

/* ---------------------------------------------------------------------------
	Function Prototypes
	------------------- */

/* ---------------------------------------------------------------------------
	Vars
	---- */
void (*MsgFunc)(char *e,va_list argptr);
void (*ErrorFunc)(char *Text,char *File,int Line);

/*	---------------------------------------------------------------------------
	Function:	BOOL DBG_OpenModule(void);

	Purpose:	Initialise the debug module

	Returns:	FALSE if unable to init

	--------------------------------------------------------------------------- */
GLIB_API BOOL DBG_OpenModule(void)
{
	return(TRUE);
}	


/*	---------------------------------------------------------------------------
	Function:	void	DBG_PollHost(void)

	Purpose:	Poll the host to enable debugging

	--------------------------------------------------------------------------- */
GLIB_API void DBG_PollHost(void)
{
}	

/*	---------------------------------------------------------------------------
	Function:	void	DBG_Halt(void)

	Purpose:	Stop where I am

	--------------------------------------------------------------------------- */
GLIB_API void DBG_Halt(void)
{
	while (1);
}	


/*	---------------------------------------------------------------------------
	Function:	void DBG_SendMessage(char *e,...)

	Purpose:	Send a diagnostic messgae

	--------------------------------------------------------------------------- */

GLIB_API void DBG_SendMessage(char *e,...)
{
}	

/*	---------------------------------------------------------------------------
	Function:	void DBG_SetMessageHandler(void (*Func)(char *e,va_list argptr))

	Purpose:	Set the message handler

	--------------------------------------------------------------------------- */
GLIB_API void DBG_SetMessageHandler(void (*Func)(char *e,va_list argptr))
{
	MsgFunc=Func;
}	
/*	---------------------------------------------------------------------------
	Function:	void DBG_Error(char *Text,char *File,int Line);

	Purpose:	Send a msg to psyq host

	--------------------------------------------------------------------------- */

GLIB_API void DBG_Error(char *Text,char *File,int Line)
{
}	

GLIB_API void DBG_SetErrorFunc(void (*EFunc)(char *Text,char *File,int Line))
{
	ErrorFunc=EFunc;
}

/*	---------------------------------------------------------------------------
	Function:	static void SendPsyqString(char *e)

	Purpose:	Send a msg to psyq host

	--------------------------------------------------------------------------- */
void SendPsyqString(char *e)
{
}	


/* ---------------------------------------------------------------------------
	ends */
