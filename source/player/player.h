/*=========================================================================

	player.h

	Author:		PKG
	Created:	
	Project:	Spongebob
	Purpose:	

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__PLAYER_PLAYER_H__
#define __PLAYER_PLAYER_H__

/*----------------------------------------------------------------------
	Includes
	-------- */

#ifndef __GAME_THING_H__
#include "Game/Thing.h"
#endif

#ifndef __SKEL_HEADER__
#include "Gfx/Skel.h"
#endif

#ifndef __DATA_STRUCTS_HEADER__
#include <dstructs.h>
#endif

#ifndef	__PLAYER_PSTATES_H__
#include "player\pstates.h"
#endif


/*	Std Lib
	------- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

typedef enum
{
	STATE_IDLE,
	STATE_JUMP,
	STATE_RUN,
	STATE_FALL,
	STATE_FALLFAR,
	STATE_BUTTBOUNCE,
	STATE_BUTTFALL,
	STATE_BUTTLAND,
	STATE_ATTACK,
	STATE_RUNATTACK,
	STATE_AIRATTACK,
	STATE_DUCK,
	STATE_SOAKUP,
	STATE_GETUP,
		
	NUM_STATES,
}PLAYER_STATE;

enum
{
	FACING_LEFT=+1,
	FACING_RIGHT=-1,
};

typedef enum
{
	PM__JUMP_VELOCITY,
	PM__MAX_JUMP_FRAMES,
	PM__MAX_SAFE_FALL_FRAMES,
	PM__GRAVITY_VALUE,
	PM__TERMINAL_VELOCITY,
	PM__MAX_RUN_VELOCITY,
	PM__RUN_SPEEDUP,
	PM__RUN_REVERSESLOWDOWN,
	PM__RUN_SLOWDOWN,
		
	NUM_PLAYER_METRICS
}PLAYER_METRIC;


/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

struct PlayerMetrics
{
	s16		m_metric[NUM_PLAYER_METRICS];
};


class CPlayer : public CThing
{
public:
	enum
	{
		VELOCITY_SHIFT=2
	};

	void			init();
	void			shutdown();
	void			think(int _frames);
	void			render();

	DVECTOR			getCameraPos();

protected:		
	const PlayerMetrics	*getPlayerMetrics();

	void			setState(PLAYER_STATE _state);
	int				getFacing();
	void			setFacing(int _facing);
	int				getAnimFrame();
	void			setAnimFrame(int _animFrame);
	int				getAnimFrameCount();
	int				getAnimNo();
	void			setAnimNo(int _animNo);
	DVECTOR			getMoveVelocity();
	void			setMoveVelocity(DVECTOR *_moveVel);
	virtual int		getPadInput();

	int				isOnSolidGround();

	void moveLeft();
	void moveRight();
	void slowdown();
	void jump();
	void fall();

friend class CPlayerState;



private:

	typedef struct
	{
		PlayerMetrics		m_metrics;
		class CPlayerState	*m_states[NUM_STATES];
	}PlayerMode;

	static PlayerMode		s_modes;
	
	int				m_animFrame;
	int				m_animNo;
	PLAYER_STATE	m_state;
	CSkel			m_skel;

	DVECTOR			m_moveVel;
	int				m_facing;
	int				m_fallFrames;

	enum
	{
		INVIBCIBLE_FRAMES__START=120,	// Invincible for this many frames at start of life
		INVINCIBLE_FRAMES__HIT=25,		// Invincible for this many frames after taking damage
	};
	int				m_invincibleFrameCount;

	class CPlayerState			*m_currentState;

	int				m_lives;

	DVECTOR			m_cameraOffsetTarget;
	DVECTOR			m_cameraOffset;

};


/*----------------------------------------------------------------------
	Globals
	------- */

/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif	/* __PLAYER_PLAYER_H__ */

/*===========================================================================
 end */
