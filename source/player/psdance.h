/*=========================================================================

	psdance.h

	Author:		PKG
	Created:	
	Project:	Spongebob
	Purpose:	

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__PLAYER_PSDANCE_H__
#define __PLAYER_PSDANCE_H__

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

class CPlayerStateDance : public CPlayerState
{
public:
	void			enter(class CPlayerModeBase *_playerMode);
	void			think(class CPlayerModeBase *_playerMode);

private:
	int				m_hitGround;
	int				m_startedConversation;
};


/*----------------------------------------------------------------------
	Globals
	------- */

extern	CPlayerStateDance		s_stateDance;


/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif	/* __PLAYER_PSDANCE_H__ */

/*===========================================================================
 end */



