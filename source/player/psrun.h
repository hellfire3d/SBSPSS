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

#ifndef	__ANIM_SPONGEBOB_HEADER__
#include <ACTOR_SPONGEBOB_ANIM.h>
#endif


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
	void			enter(class CPlayerModeBase *_playerMode);
	void			think(class CPlayerModeBase *_playerMode);

protected:
	virtual int		getStartFrame()						{return ANIM_SPONGEBOB_RUNSTART;}
	virtual int		getLoopFrame()						{return ANIM_SPONGEBOB_RUN;}
	virtual int		getEndFrame()						{return ANIM_SPONGEBOB_RUNSTOP;}

private:
	int				m_numberOfTimesAnimHasLooped;
};


class CPlayerStateWalk : public CPlayerStateRun
{
protected:
	virtual int		getStartFrame()						{return -1;}
	virtual int		getLoopFrame()						{return ANIM_SPONGEBOB_WALK;}
	virtual int		getEndFrame()						{return -1;}
};


/*----------------------------------------------------------------------
	Globals
	------- */

extern	CPlayerStateRun				s_stateRun;
extern	CPlayerStateWalk			s_stateWalk;


/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif	/* __PLAYER_PSRUN_H__ */

/*===========================================================================
 end */



