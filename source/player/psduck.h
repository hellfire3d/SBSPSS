/*=========================================================================

	psduck.h

	Author:		PKG
	Created:	
	Project:	Spongebob
	Purpose:	

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__PLAYER_PSDUCK_H__
#define __PLAYER_PSDUCK_H__

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

class CPlayerStateSoakUp : public CPlayerState
{
public:
	void			enter(class CPlayerModeBase *_playerMode);
	void			think(class CPlayerModeBase *_playerMode);

private:
	enum
	{
		BUBBLE_SPAWNDELAY=20,
		BUBBLE_XOFF=-10,
		BUBBLE_YOFF=-57,
		BUBBLE_W=20,
		BUBBLE_H=10,
	};

	int				m_breatheDelayFrames;
};


class CPlayerStateGetUp : public CPlayerState
{
public:
	void			enter(class CPlayerModeBase *_playerMode);
	void			think(class CPlayerModeBase *_playerMode);

};


/*----------------------------------------------------------------------
	Globals
	------- */

extern	CPlayerStateSoakUp			s_stateSoakUp;
extern	CPlayerStateGetUp			s_stateGetUp;


/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif	/* __PLAYER_PSDUCK_H__ */

/*===========================================================================
 end */



