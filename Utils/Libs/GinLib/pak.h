/*=========================================================================

	PAK.H

	Author:  Carl Muller (algorithm Nick Pelling && Carl Muller)
	Created:
	Project:
	Purpose:

	Copyright (c) 1997 Climax Development Ltd

===========================================================================*/

#ifndef __PAK_PAK_H__
#define __PAK_PAK_H__

/*----------------------------------------------------------------------
	Includes
	-------- */

/*	Std Lib
	------- */

/*	Glib
	---- */
#include <gtypes.h>

/*	Local
	----- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

/*----------------------------------------------------------------------
	Globals
	------- */

/*	Vars
	---- */

/*	Data
	---- */

/*	Functions
	--------- */
int		PAK_doPak(u8 * Dest,u8 const * source,int insize);
int		PAK_doUnpak(u8 * Dest,u8 const * Source);
int		PAK_findPakSize(u8 const * souce,int insize);

/*---------------------------------------------------------------------- */

#endif	/* __PAK_PAK_H__ */

/*===========================================================================
 end */
