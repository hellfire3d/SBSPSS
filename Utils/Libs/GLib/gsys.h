/* ==========================================================================
	File:		GSYS.H

	Notes:		Machine Independant API to target for low level system info
				and manipulation
	
	Author:		Gary Liddon

	Copyright (C) 1995 - 1997 Gary Liddon
	All rights reserved. 
  =========================================================================== */

#ifndef __GSYS_H__
#define __GSYS_H__

/* ---------------------------------------------------------------------------
	Includes
	-------- */
#include "gtypes.h"

/* ---------------------------------------------------------------------------
	Defines
	------- */

/* ---------------------------------------------------------------------------
	Typedefs
	-------- */

/* ---------------------------------------------------------------------------
	Enums
	----- */

/* ---------------------------------------------------------------------------
	Externs
	------- */

/* ---------------------------------------------------------------------------
	Structures
	---------- */
typedef struct MEM_INFO
{
	void *	Addr;
	U32		Size;

}	MEM_INFO;


/* ---------------------------------------------------------------------------
	Supported Functions
	------------------- */
#ifdef __cplusplus
extern "C" {
#endif

/*	System Initialisation stuff
	--------------------------- */
GLIB_API BOOL	GSYS_InitMachine(void);

/*	Stack handling functions
	------------------------ */
GLIB_API void	GSYS_SetStackAndJump(void *Stack,void(*Func)(void *),void *Param);
GLIB_API void	GSYS_MarkStack(void * Stack, U32 StackSize);
GLIB_API BOOL	GSYS_IsStackCorrupted(void * Stack, U32 StackSize);
GLIB_API BOOL	GSYS_CheckPtr(void *Ptr);
GLIB_API BOOL	GSYS_IsStackOutOfBounds(void* Stack, U32 StackSize);

/*	Machine Info Functions
	---------------------- */
GLIB_API const MEM_INFO *	GSYS_GetWorkMemInfo(void);

#ifdef __cplusplus
};
#endif

/*	Global Vars
	----------- */
GLIB_API extern UINT		GSYS_MemStart;
GLIB_API extern UINT		GSYS_MemEnd;

/* --------------------------------------------------------------------------- */
#endif
/* ---------------------------------------------------------------------------
	ends */
