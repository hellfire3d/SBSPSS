/*=========================================================================

	psjump.h

	Author:		PKG
	Created:	
	Project:	Spongebob
	Purpose:	

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__PLAYER_PSRUN_H__
#define __PLAYER_PSRUN_H__

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

class CPlayerStateRun : public CPlayerState
{
public:
	void			enter(class CPlayerModeBasic *_playerMode);
	void			think(class CPlayerModeBasic *_playerMode);

private:
	int				m_numberOfTimeAnimHasLooped;
};


/*----------------------------------------------------------------------
	Globals
	------- */

/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif	/* __PLAYER_PSRUN_H__ */

/*===========================================================================
 end */



