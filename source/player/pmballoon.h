/*=========================================================================

	pmballoon.h

	Author:		PKG
	Created:	
	Project:	Spongebob
	Purpose:	

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__PLAYER_PMBALLOON_H__
#define __PLAYER_PMBALLOON_H__

/*----------------------------------------------------------------------
	Includes
	-------- */

#ifndef __PLAYER_PMODES_H__
#include "player/pmodes.h"
#endif


/*	Std Lib
	------- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

class CPlayerModeBalloon: public CPlayerMode
{
public:
	virtual void	enter(class CPlayer *_player);
	virtual void	think(class CPlayer *_player);

private:
	int				m_timer;
};


/*----------------------------------------------------------------------
	Globals
	------- */

/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif	/* __PLAYER_PMBALLOON_H__ */

/*===========================================================================
 end */



