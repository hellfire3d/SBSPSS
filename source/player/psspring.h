/*=========================================================================

	psspring.h

	Author:		PKG
	Created:	
	Project:	Spongebob
	Purpose:	

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__PLAYER_PSSPRING_H__
#define __PLAYER_PSSPRING_H__

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

class CPlayerStateSpring : public CPlayerState
{
public:
	void			enter(class CPlayerModeBase *_playerMode);
	void			think(class CPlayerModeBase *_playerMode);

private:
	int				m_springFrames;

};


/*----------------------------------------------------------------------
	Globals
	------- */

extern	CPlayerStateSpring			s_stateSpring;


/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif	/* __PLAYER_PSSPRING_H__ */

/*===========================================================================
 end */



