/*=========================================================================

	pstates.h

	Author:		PKG
	Created:	
	Project:	Spongebob
	Purpose:	

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__PLAYER_PSTATES_H__
#define __PLAYER_PSTATES_H__

/*----------------------------------------------------------------------
	Includes
	-------- */

#ifndef _GLOBAL_HEADER_
#include "system\global.h"
#endif


/*	Std Lib
	------- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

class CPlayerState
{
public:
	virtual void	enter(class CPlayer *_player)		{;}
	virtual void	think(class CPlayer *_player)		{;}


protected:
	const struct PlayerMetrics	*getPlayerMetrics(class CPlayer *_player);
	int				setState(class CPlayer *_player,int _state);
	int				getFacing(class CPlayer *_player);
	void			setFacing(class CPlayer *_player,int _facing);
	int				getAnimNo(class CPlayer *_player);
	void			setAnimNo(class CPlayer *_player,int _animNo);
	void			setAnimFrame(class CPlayer *_player,int _animFrame);
	int				advanceAnimFrameAndCheckForEndOfAnim(class CPlayer *_player);
	int				retreatAnimFrameAndCheckForEndOfAnim(class CPlayer *_player);
	DVECTOR			getMoveVelocity(class CPlayer *_player);
	void			setMoveVelocity(class CPlayer *_player,DVECTOR *_moveVel);
	DVECTOR			getPlayerPos(class CPlayer *_player);
	int				getPadInputHeld(class CPlayer *_player);
	int				getPadInputDown(class CPlayer *_player);

	int				isOnSolidGround(class CPlayer *_player);
	int				isOnEdge(class CPlayer *_player);
	int				canMoveLeft(class CPlayer *_player);
	int				canMoveRight(class CPlayer *_player);

	void moveLeft(class CPlayer *_player);
	void moveRight(class CPlayer *_player);
	void slowdown(class CPlayer *_player);
	void jump(class CPlayer *_player);
	void fall(class CPlayer *_player);


};


/*----------------------------------------------------------------------
	Globals
	------- */

/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif	/* __PLAYER_PSTATES_H__ */

/*===========================================================================
 end */



