/*=========================================================================

	tsleftemit.h

	Author:		CRB
	Created:	
	Project:	Spongebob
	Purpose:	

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__TRIGGERS_TSLEFTEMIT_H__
#define __TRIGGERS_TSLEFTEMIT_H__

/*----------------------------------------------------------------------
	Includes
	-------- */

#ifndef __THING_THING_H__
#include "thing/thing.h"
#endif

#ifndef __TRIGGERS_TSEMIT_H__
#include "triggers\tsemit.h"
#endif

/*	Std Lib
	------- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

class CSteamLeftEmitterTrigger : public CSteamEmitterTrigger
{
public:
	virtual void		setPositionAndSize(int _x,int _y,int _w,int _h);
};

/*----------------------------------------------------------------------
	Globals
	------- */

/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif

/*===========================================================================
 end */
