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

#define ANIM_IDLE_SHORT		0
#define ANIM_IDLE_LONG		1
#define ANIM_JUMP			0
#define ANIM_RUNSTART		2
#define ANIM_RUN			3
#define ANIM_RUNSTOP		4

typedef enum
{
	STATE_IDLE,
	STATE_JUMP,
	STATE_RUN,
	STATE_FALL,
	STATE_FALLFAR,
		
	NUM_STATES,
}PLAYER_STATE;

enum
{
	FACING_LEFT=+1,
	FACING_RIGHT=-1,
};


extern int JUMP_VELOCITY;
extern int MAX_JUMP_FRAMES;
extern int MAX_SAFE_FALL_FRAMES;
extern int GRAVITY_VALUE;
extern int TERMINAL_VELOCITY;
extern int MAX_RUN_VELOCITY;
extern int RUN_SPEEDUP;
extern int RUN_REVERSESLOWDOWN;
extern int RUN_SLOWDOWN;
extern int PSHIFT;


/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

class CPlayer : public CThing
{
public:
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
		PM__PSHIFT,
		
		NUM_PLAYER_METRICS
	}PLAYER_METRIC;


	void			init();
	void			shutdown();
	void			think(int _frames);
	void			render();


protected:		
	int				getPlayerMetric(PLAYER_METRIC _metric);

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

	int				m_animFrame;
	int				m_animNo;
	PLAYER_STATE	m_state;
	CSkel			m_skel;

	DVECTOR			m_moveVel;
	int				m_facing;

	static class CPlayerState	*s_states[NUM_STATES];

	class CPlayerState	*m_currentState;

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
