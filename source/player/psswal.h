/*=========================================================================

	psswal.h

	Author:		CRB
	Created:	
	Project:	Spongebob
	Purpose:	

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__PLAYER_PSSWAL_H__
#define __PLAYER_PSSWAL_H__

/*----------------------------------------------------------------------
	Includes
	-------- */

#include "player\pstates.h"


/*	Std Lib
	------- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

class CPlayerStateSwallow : public CPlayerState
{
public:
	void			enter(class CPlayerModeBase *_playerMode);
	void			think(class CPlayerModeBase *_playerMode);
};


/*----------------------------------------------------------------------
	Globals
	------- */

extern	CPlayerStateSwallow			s_stateSwallow;

/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif	/* __PLAYER_PSSWAL_H__ */

/*===========================================================================
 end */
