/* ==========================================================================
	File:		GUTILS.C

	Notes:		General miscellaneous utilities

	Author:		Gary Liddon

	Copyright (C) 1995 - 1997 Gary Liddon
	All rights reserved. 
  =========================================================================== */

#ifndef __GUTILS_H
#define __GUTILS_H

/* ---------------------------------------------------------------------------
	Includes
	-------- */

/*	Glib
	---- */
#include "gtypes.h"

/*	Includes
	-------- */

/* ---------------------------------------------------------------------------
	Defines, enums & Typedefs
	------------------------- */
#define STRUCT_OFFSET(type,member) ((int)(&(((type *)0)->member)));

/* ---------------------------------------------------------------------------
	Structure Definitions
	--------------------- */


/* ---------------------------------------------------------------------------
	Globals
	------- */
#ifdef __cplusplus
extern "C" {
#endif


GLIB_API BOOL	GU_InitModule(void);

/*	Random number stuff
	------------------- */
GLIB_API void	GU_SetRndSeed(U32 *Tab);
GLIB_API U32	GU_GetRnd(void);
GLIB_API S32	GU_GetSRnd(void);
GLIB_API U32	GU_GetRndRange(UINT Range);				/* 0- Range-1 */
GLIB_API UINT	GU_AlignVal(UINT w,UINT round);


#ifdef __cplusplus
};
#endif


/* --------------------------------------------------------------------------- */
#endif
/* ---------------------------------------------------------------------------
	ends */
