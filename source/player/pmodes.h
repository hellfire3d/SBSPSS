/*=========================================================================

	pmodes.h

	Author:		PKG
	Created:	
	Project:	Spongebob
	Purpose:	

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__PLAYER_PMODES_H__
#define __PLAYER_PMODES_H__

/*----------------------------------------------------------------------
	Includes
	-------- */

#ifndef _GLOBAL_HEADER_
#include "system\global.h"
#endif

#ifndef __PLAYER_PLAYER_H__
#include "player\player.h"
#endif


/*	Std Lib
	------- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

typedef enum
{
	PM__JUMP_VELOCITY,
	PM__MAX_JUMP_FRAMES,
	PM__MAX_SAFE_FALL_FRAMES,
	PM__MAX_RUN_VELOCITY,
	PM__RUN_SPEEDUP,
	PM__RUN_REVERSESLOWDOWN,
	PM__RUN_SLOWDOWN,
		
	NUM_PLAYER_METRICS
}PLAYER_METRIC;

struct PlayerMetrics
{
	s16		m_metric[NUM_PLAYER_METRICS];
};




class CPlayerMode
{
public:
	virtual void	initialise(class CPlayer *_player)	{m_player=_player;}
	virtual void	enter()								{;}
	virtual void	think()								{;}
	virtual void	render()							{;}

	int				getPadInputHeld();
	int				getPadInputDown();

protected:
	DVECTOR			getPlayerPos();
	void			setPlayerPos(DVECTOR *_pos);

	class CPlayer	*m_player;

};


class CPlayerModeBasic : public CPlayerMode
{
public:
	enum
	{
		VELOCITY_SHIFT=4,
	};

	virtual void	enter();
	virtual void	think();
	virtual void	render();

//protected:
	int							canTeeter()				{return m_currentState==STATE_IDLE;}
	int							canFallForever()		{return m_currentState==STATE_BUTTFALL;}

	void						thinkVerticalMovement();
	void						thinkHorizontalMovement();

	
	const struct PlayerMetrics	*getPlayerMetrics();
	int							setState(int _state);
//	virtual void			setMode(class CPlayer *_player,int _mode);
	int							getFacing();
	void						setFacing(int _facing);
//	virtual int				getAnimNo(class CPlayer *_player);
	void						setAnimNo(int _animNo);
	void						setAnimFrame(int _animFrame);
	int							advanceAnimFrameAndCheckForEndOfAnim();
//	virtual int				retreatAnimFrameAndCheckForEndOfAnim(class CPlayer *_player);
	DVECTOR						getMoveVelocity();
	void						zeroMoveVelocity();
	void						setMoveVelocity(DVECTOR *_moveVel);
	int							isOnEdge();
	int							canMoveLeft();
	int							canMoveRight();

	void						moveLeft();
	void						moveRight();
	int							slowdown();
	void						jump();
	void						fall();

//	virtual void			respawn(class CPlayer *_player);
protected:
	enum
	{
		DEFAULT_PLAYER_JUMP_VELOCITY=4,
		DEFAULT_PLAYER_MAX_JUMP_FRAMES=12,
		DEFAULT_PLAYER_MAX_SAFE_FALL_FRAMES=30,
		DEFAULT_PLAYER_MAX_RUN_VELOCITY=6,			//8
		DEFAULT_PLAYER_RUN_SPEEDUP=2<<2,			//4
		DEFAULT_PLAYER_RUN_REVERSESLOWDOWN=4<<2,	//3
		DEFAULT_PLAYER_RUN_SLOWDOWN=3<<2,			//2
		PLAYER_GRAVITY=4<<2,
		PLAYER_TERMINAL_VELOCITY=8,
	};



private:
	int							m_fallFrames;
	DVECTOR						m_moveVelocity;

	static class CPlayerState	*s_stateTable[];
	class CPlayerState			*m_currentStateClass;
	PLAYER_STATE				m_currentState;

	static PlayerMetrics		s_playerMetrics;
};


/*----------------------------------------------------------------------
	Globals
	------- */

/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif	/* __PLAYER_PMODES_H__ */

/*===========================================================================
 end */



