/*=========================================================================

	.h

	Author:  
	Created:
	Project:
	Purpose:

	Copyright (c) 1998 / 1999 Climax Development Ltd

===========================================================================*/

#ifndef __FMV_HEADER__
#define __FMV_HEADER__

/*----------------------------------------------------------------------
	Includes
	-------- */

/*	Std Lib
	------- */

/*	Glib
	---- */

#ifndef _GLOBAL_HEADER_
#include "system\global.h"
#endif


/*	Local
	----- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

enum
{
	FMV_THQ,
	FMV_CLIMAX,
	FMV_INTRO,
#if defined(__TERRITORY_EUR__)
	FMV_DEMO,
#endif
	MAX_FMVS,
};


/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */
typedef bool (*FmvPerFrameFunc)(void);

/*----------------------------------------------------------------------
	Globals
	------- */

/*	Vars
	---- */

/*	Data
	---- */

/*	Functions
	--------- */
int  FMV_play(int fmvNumber,FmvPerFrameFunc func=NULL);
void SetCDVolumeOn(void);
void SetCDVolumeOff(void);

/*---------------------------------------------------------------------- */

#endif	/* __XYZ_H__ */

/*===========================================================================
 end */
