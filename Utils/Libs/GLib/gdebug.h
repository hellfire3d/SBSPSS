/* ===========================================================================
	File:		GDEBUG.H

	Notes:		API for machine independant debugging

	Author:		Gary Liddon @ 73b

	Created:	Wednesday 27th March 1996

	Copyright (C) 1996 - 1997 Gary Liddon
	All rights reserved. 

  ============================================================================ */

#ifndef __GDEBUG_H__
#define __GDEBUG_H__

/* ---------------------------------------------------------------------------
	Includes
	-------- */

/*	Standard Lib
	------------ */
#include "stdarg.h"


/*	Glib Includes
	------------- */
#include "gtypes.h"


/* ---------------------------------------------------------------------------
	Defines
	------- */

/* ---------------------------------------------------------------------------
	Typedefs
	-------- */
#ifdef __GL_DEBUG__
#define ASSERT(p) ( (p) ? (void)0 : (void) DBG_Error(#p,__FILE__,__LINE__) )
#else
#define ASSERT(p) ( (p) ? (void)0 : (void) DBG_Error(NULL,__FILE__,__LINE__) )
#endif

#ifdef __GL_DEBUG__

#define DBG_MSG0(a)			DBG_SendMessage(a)
#define DBG_MSG1(a,b)		DBG_SendMessage(a,b)
#define DBG_MSG2(a,b,c)		DBG_SendMessage(a,b,c)
#define DBG_MSG3(a,b,c,d)	DBG_SendMessage(a,b,c,d)
#define DBG_MSG4(a,b,c,d,e)	DBG_SendMessage(a,b,c,d,e)

#else

#define DBG_MSG0(a)
#define DBG_MSG1(a,b)
#define DBG_MSG2(a,b,c)
#define DBG_MSG3(a,b,c,d)
#define DBG_MSG4(a,b,c,d,e)

#endif


/* ---------------------------------------------------------------------------
	Enums
	----- */

/* ---------------------------------------------------------------------------
	Id for each file
	---------------- */

/* ---------------------------------------------------------------------------
	Externs
	------- */

/* ---------------------------------------------------------------------------
	Structures
	---------- */

/* ---------------------------------------------------------------------------
	Supported Functions
	------------------- */


#ifdef __cplusplus
extern "C" {
#endif


GLIB_API BOOL	DBG_OpenModule(void);
GLIB_API void	DBG_Halt(void);
GLIB_API void	DBG_PollHost(void);

GLIB_API void	DBG_SendMessage(char *e,...);
GLIB_API void	DBG_SendErrorMessage(char *e,...);

GLIB_API void	DBG_SetMessageHandler(void (*Func)(char *e,va_list argptr));
GLIB_API void	DBG_SetErrorMessageHandler(void (*Func)(char *e,va_list argptr));

GLIB_API void	DBG_Error(char *Text,char *File,int Line);

GLIB_API void	DBG_SetErrorFunc(void (*EFunc)(char *Text,char *File,int Line));
GLIB_API void	DBG_SetPollRoutine(void (*Func)(void));

#ifdef __cplusplus
};
#endif

/* --------------------------------------------------------------------------- */
#endif
/* ---------------------------------------------------------------------------
	ends */
