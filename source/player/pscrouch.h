/*=========================================================================

	pscrouch.h

	Author:		PKG
	Created:	
	Project:	Spongebob
	Purpose:	

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__PLAYER_PSCROUCH_H__
#define __PLAYER_PSCROUCH_H__

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

class CPlayerStateCrouchDown : public CPlayerState
{
public:
	void			enter(class CPlayerModeBase *_playerMode);
	void			think(class CPlayerModeBase *_playerMode);

};


class CPlayerStateCrouchUp : public CPlayerState
{
public:
	void			enter(class CPlayerModeBase *_playerMode);
	void			think(class CPlayerModeBase *_playerMode);

};


/*----------------------------------------------------------------------
	Globals
	------- */

extern	CPlayerStateCrouchDown		s_stateCrouchDown;
extern	CPlayerStateCrouchUp		s_stateCrouchUp;


/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif	/* __PLAYER_PSCROUCH_H__ */

/*===========================================================================
 end */



