/* ===========================================================================
	File:			TICK.C

	Notes:			Game Frame Clock stuff
	
	Author:			G Robert Liddon @ Croydon

	Created:		Monday 2nd October 1995

	Copyright (C) 1995 - 1997 Gary Liddon
	All rights reserved. 
  =========================================================================== */

#ifndef __TICK_H
#define __TICK_H

/* ---------------------------------------------------------------------------
	Glib Includes
	------------- */
#include "gtypes.h"

/* ---------------------------------------------------------------------------
	Defines
	------- */

/* ---------------------------------------------------------------------------
	Structures
	---------- */

/* ---------------------------------------------------------------------------
	Global Routines
	--------------- */
#ifdef __cplusplus
extern "C" {
#endif

GLIB_API void			TICK_InitModule(void);
GLIB_API void			TICK_Set(U32 Val);
GLIB_API U32				TICK_Get(void);
GLIB_API void			TICK_Update(void);
GLIB_API U32				TICK_GetAge(U32 OldTick);
GLIB_API const char *	TICK_GetTimeString(void);
GLIB_API const char *	TICK_GetDateString(void);

#ifdef __cplusplus
};
#endif


/* ---------------------------------------------------------------------------
	Global Vars
	----------- */

/* --------------------------------------------------------------------------- */
#endif

/* ---------------------------------------------------------------------------
	ends */

