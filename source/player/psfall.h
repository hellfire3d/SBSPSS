/*=========================================================================

	psfall.h

	Author:		PKG
	Created:	
	Project:	Spongebob
	Purpose:	

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__PLAYER_PSFALL_H__
#define __PLAYER_PSFALL_H__

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

class CPlayerStateFall : public CPlayerState
{
public:
	void			enter(class CPlayerModeBasic *_playerMode);
	void			think(class CPlayerModeBasic *_playerMode);

};


class CPlayerStateFallFar : public CPlayerStateFall
{
private:
	void			enter(class CPlayerModeBasic *_playerMode);
	void			think(class CPlayerModeBasic *_playerMode);

};


/*----------------------------------------------------------------------
	Globals
	------- */

/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif	/* __PLAYER_PSFALL_H__ */

/*===========================================================================
 end */



