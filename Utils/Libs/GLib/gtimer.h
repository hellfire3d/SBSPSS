/* ===========================================================================
	File:		GTIMER.C

	Notes:		PSX stuff for timing things

	Author:		Gary Liddon

	Copyright (C) 1995 - 1997 Gary Liddon
	All rights reserved. 

  ============================================================================ */

#ifndef __GTIMER_H__
#define __GTIMER_H__

/* ---------------------------------------------------------------------------
	Includes
	-------- */

/*	Glib
	---- */
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
	Structures
	---------- */

typedef S32	GTHANDLE;

typedef struct CLOCK
{
	U32		StartTime;
	U32		StopTime;
	U32		Id;
	BOOL	Running;

} CLOCK;

#define NULL_GTHANDLE -1

/* ---------------------------------------------------------------------------
	Supported Functions
	------------------- */
#ifdef __cplusplus
extern "C" {
#endif

GLIB_API BOOL		GTIM_Open(int MaxClocks,U32 RamId);
GLIB_API void		GTIM_ClearClocks(void);
GLIB_API GTHANDLE	GTIM_StartClock(U32 Id);
GLIB_API U32			GTIM_StopClock(GTHANDLE Hnd);
GLIB_API void		GTIM_StopAllClocks(void);
GLIB_API void		GTIM_IterateClocks(void (*Func)(U32 Start,U32 End,U32 Id));
GLIB_API U32			GTIM_TimeFunction(void (*Func)(void),int Tries);

#ifdef __cplusplus
};
#endif

/* --------------------------------------------------------------------------- */
#endif
/* ---------------------------------------------------------------------------
	ends */
