/*=========================================================================

	pscart.h

	Author:		CRB
	Created:	
	Project:	Spongebob
	Purpose:	

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__PLAYER_PSCART_H__
#define __PLAYER_PSCART_H__

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

class CPlayerStateCart : public CPlayerState
{
public:
	void			enter(class CPlayerModeBase *_playerMode);
	void			think(class CPlayerModeBase *_playerMode);
};


/*----------------------------------------------------------------------
	Globals
	------- */

extern	CPlayerStateCart			s_stateCart;

/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif	/* __PLAYER_PSCART_H__ */

/*===========================================================================
 end */
