/*=========================================================================

	pmbloon.h

	Author:		PKG
	Created:	
	Project:	Spongebob
	Purpose:	

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__PLAYER_PMBLOON_H__
#define __PLAYER_PMBLOON_H__

/*----------------------------------------------------------------------
	Includes
	-------- */

#ifndef __PLAYER_PMODES_H__
#include "player\pmodes.h"
#endif


/*	Std Lib
	------- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

class CPlayerModeBalloon : public CPlayerModeBase
{
public:
	virtual int							canFallForever()		{return true;}
	virtual const struct PlayerMetrics	*getPlayerMetrics();
};


/*----------------------------------------------------------------------
	Globals
	------- */

/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif	/* __PLAYER_PMBLOON_H__ */

/*===========================================================================
 end */



