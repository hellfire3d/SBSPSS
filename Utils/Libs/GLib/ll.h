/* ===========================================================================
	File:			LL.C

	Notes:			General Link List Handling Code
	
	Author:			G Robert Liddon @ Croydon
	Created:		Wednesday 6th October 1994

	Copyright (C) 1995 - 1997 Gary Liddon
	All rights reserved. 
  =========================================================================== */

#ifndef __LL_H
#define __LL_H

/* ---------------------------------------------------------------------------
	Includes
	-------- */
#include "gtypes.h"

/* ---------------------------------------------------------------------------
	Structures
	---------- */
typedef struct LinkList
{
	struct LinkList *Next;
	struct LinkList *Prev;

} LinkList;


/* ---------------------------------------------------------------------------
	Globals
	------- */
#ifdef __cplusplus
extern "C" {
#endif

GLIB_API void LL_DetachFromList(LinkList **Head,LinkList *ThisObj);
GLIB_API void LL_AddToList(LinkList **Head,LinkList *ThisObj);

#ifdef __cplusplus
};
#endif

/* --------------------------------------------------------------------------- */
#endif
/* ---------------------------------------------------------------------------
	ends */
