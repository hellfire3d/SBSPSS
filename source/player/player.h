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

#ifndef __THING_TPLAYER_H__
#include "thing/tplayer.h"
#endif

#ifndef __GFX_SKELSPNG_H__
#include "gfx/skelspng.h"
#endif

#ifndef __DATA_STRUCTS_HEADER__
#include <dstructs.h>
#endif

#ifndef	__PLAYER_PSTATES_H__
#include "player\pstates.h"
#endif

#ifndef __SOUND_SOUND_H__
#include "sound\sound.h"
#endif


/*	Std Lib
	------- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

typedef enum
{
	STATE_IDLE,
	STATE_IDLETEETER,
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

	STATE_DEAD,

	NUM_STATES,
}PLAYER_STATE;

typedef enum
{
	PLAYER_MODE_BASICUNARMED,
	PLAYER_MODE_FULLUNARMED,
	PLAYER_MODE_SQUEAKYBOOTS,
	PLAYER_MODE_NET,
	PLAYER_MODE_CORALBLOWER,
	PLAYER_MODE_FLY,

	NUM_PLAYERMODES,
}PLAYER_MODE;

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
	PM__MAX_RUN_VELOCITY,
	PM__RUN_SPEEDUP,
	PM__RUN_REVERSESLOWDOWN,
	PM__RUN_SLOWDOWN,
		
	NUM_PLAYER_METRICS
}PLAYER_METRIC;

typedef enum
{
	DAMAGE__FALL,
	DAMAGE__ELECTROCUTION,
	DAMAGE__LAVA,
}DAMAGE_TYPE;


// The input from the control pad is remapped to this rather than keeping it in the
// normal pad format. This allows us to store all input in one byte ( as opposed to
// two bytes ) for demo recording and means that the player state codes don't have
// to keep using CPadConfig to remap the controls internally.
typedef enum
{
	PI_NONE			=0,
	PI_UP			=1<<0,
	PI_DOWN			=1<<1,
	PI_LEFT			=1<<2,
	PI_RIGHT		=1<<3,
	PI_JUMP			=1<<4,
	PI_ACTION		=1<<5,
}PLAYERINPUT;


/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

struct PlayerMetrics
{
	s16		m_metric[NUM_PLAYER_METRICS];
};


class CPlayer : public CPlayerThing
{
public:
	enum
	{
		VELOCITY_SHIFT=2
	};

	virtual void	init();
	virtual void	shutdown();
	virtual void	think(int _frames);
	virtual void	render();

	DVECTOR			getCameraPos();

	void			setLayerCollision(class CLayerCollision *_layer)		{m_layerCollision=_layer;}
	void			setMapSize(DVECTOR _mapSize);
	void			setRespawnPos(DVECTOR _respawn)							{m_respawnPos=_respawn;}

protected:		
	enum
	{
		DEFAULT_PLAYER_JUMP_VELOCITY=4,
		DEFAULT_PLAYER_MAX_JUMP_FRAMES=12,
		DEFAULT_PLAYER_MAX_SAFE_FALL_FRAMES=30,
		DEFAULT_PLAYER_MAX_RUN_VELOCITY=8,
		DEFAULT_PLAYER_RUN_SPEEDUP=4,
		DEFAULT_PLAYER_RUN_REVERSESLOWDOWN=3,
		DEFAULT_PLAYER_RUN_SLOWDOWN=2,
		PLAYER_GRAVITY=4,
		PLAYER_TERMINAL_VELOCITY=8,
	};
	const PlayerMetrics	*getPlayerMetrics();

	// State
	int				setState(PLAYER_STATE _state);
	void			setMode(PLAYER_MODE _mode);
	int				getFacing();
	void			setFacing(int _facing);
	int				getAnimFrame();
	void			setAnimFrame(int _animFrame);
	int				getAnimFrameCount();
	int				getAnimNo();
	void			setAnimNo(int _animNo);
	DVECTOR			getMoveVelocity();
	void			setMoveVelocity(DVECTOR *_moveVel);
	DVECTOR			getPlayerPos();
	void			setPlayerPos(DVECTOR *_pos);
	PLAYERINPUT		getPadInputHeld();
	PLAYERINPUT		getPadInputDown();

	// Collision
	int				isOnSlippySurface();
	int				isOnEdge();
	int				canMoveLeft();
	int				canMoveRight();

	// Movement
	void moveLeft();
	void moveRight();
	void slowdown();
	void jump();
	void fall();

	void			respawn();

	void			takeDamage(DAMAGE_TYPE _damage);

friend class CPlayerState;



private:
	typedef struct
	{
		PlayerMetrics		m_metrics;
		class CPlayerState	*m_states[NUM_STATES];
	}PlayerMode;


public:
	typedef struct
	{
		int						m_frame;
		CSoundMediator::SFXID	m_sfxId;
	} AnimFrameSfx;
private:
	typedef struct
	{
		int							m_numAnimFrameSfx;
		const struct AnimFrameSfx	*m_animFrameSfx;
	} AnimSfx;
	static const AnimSfx	s_animSfx[];
	int				m_animFrame;
	int				m_animNo;
	CSkelSpongeBob	m_skel;

	DVECTOR			m_playerScreenPos;
	DVECTOR			m_cameraPos;
	int				m_cameraScrollDir;

	DVECTOR			m_moveVel;
	int				m_facing;
	int				m_fallFrames;

	enum
	{
		INVIBCIBLE_FRAMES__START=120,	// Invincible for this many frames at start of life
		INVINCIBLE_FRAMES__HIT=50,		// Invincible for this many frames after taking damage

		LOOKAROUND_DELAY=90,			// Frames before look around starts
		LOOKAROUND_SCROLLSPEED=2,		// Speed to scroll at
		LOOKAROUND_MAXSCROLL=60,		// Maximum distance to scroll
		LOOKAROUND_RESETSPEED=6,		// Speed of scroll back when look around ended
	};
	int				m_invincibleFrameCount;

	void			thinkVerticalMovement();
	void			thinkHorizontalMovement();

	static PlayerMode	s_modes[NUM_PLAYERMODES];
	int					m_currentMode;
	class CPlayerState	*m_currentStateClass;
	PLAYER_STATE		m_currentState;

	int				m_lives;

	DVECTOR			m_cameraOffset;

	void			updatePadInput();
protected:
	virtual PLAYERINPUT	readPadInput();
private:
	PLAYERINPUT			m_padInput;			// Controls that are being held down
	PLAYERINPUT			m_lastPadInput;		// Last frames controls
	PLAYERINPUT			m_padInputDown;		// Controls that were pressed this frame

	
	// Various info about the current map
	class CLayerCollision	*m_layerCollision;
	DVECTOR					m_mapCameraEdges;
	DVECTOR					m_mapEdge;
	DVECTOR					m_respawnPos;

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
