/*=========================================================================

	npc.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef	__ENEMY_NPC_H__
#define	__ENEMY_NPC_H__

//#include <dstructs.h>

#ifndef __THING_THING_H__
#include "thing/thing.h"
#endif


//#include "Gfx/Skel.h"

#ifndef __ENEMY_NPCPATH_H__
#include "enemy\npcpath.h"
#endif

#ifndef	__PLAYER_PLAYER_H__
#include "player\player.h"
#endif

#ifndef __GFX_SPRBANK_H__
#include "gfx\sprbank.h"
#endif

/*****************************************************************************/

class	CNpcFriend : public CNpcThing
{
public:
	enum NPC_FRIEND_UNIT_TYPE
	{
		NPC_FRIEND_SANDY_CHEEKS = 0,
		NPC_FRIEND_GARY = 1,
		NPC_FRIEND_UNIT_TYPE_MAX,
	};

	void				init();
	void				shutdown();
	void				think(int _frames);
	void				render();
	void				processEvent( GAME_EVENT evt, CThing *sourceThing );
	void				setLayerCollision( class CLayerCollision *_layer )		{m_layerCollision=_layer;}
	void				setType( NPC_FRIEND_UNIT_TYPE newType )					{m_type = newType;}

private:
	class CLayerCollision	*m_layerCollision;

protected:
	enum NPC_FRIEND_MOVEMENT_FUNC
	{
		NPC_FRIEND_MOVEMENT_STATIC = 0,
		NPC_FRIEND_MOVEMENT_GARY = 1,
	};

	typedef struct NPC_FRIEND_DATA_TYPE
	{
		//NPC_FRIEND_INIT_FUNC					initFunc;
		//NPC_FRIEND_SENSOR_FUNC					sensorFunc;
		NPC_FRIEND_MOVEMENT_FUNC				movementFunc;
		//NPC_FRIEND_CLOSE_FUNC					closeFunc;
		//NPC_FRIEND_TIMER_FUNC					timerFunc;
		bool							canTalk;
		u8								speed;
		u16								turnSpeed;
		bool							detectCollision;
		DAMAGE_TYPE						damageToUserType;
	}
	NPC_FRIEND_DATA;

	// gary functions

	void				processGaryMovement( int _frames );

	// data

	static NPC_FRIEND_DATA		m_data[NPC_FRIEND_UNIT_TYPE_MAX];

//	class SpriteBank	*m_spriteBank;

	enum
	{
		EXTEND_RIGHT = true,
		EXTEND_LEFT = false,
	};

	// internal variables
	
	NPC_FRIEND_UNIT_TYPE		m_type;
	s32							m_extension;

	int				m_frame;
	int				m_animNo;
	CActorGfx		*m_actorGfx;
	DVECTOR			m_drawOffset;
	bool			m_reversed;
};

class	CNpcEnemy : public CEnemyThing
{
public:
	enum NPC_UNIT_TYPE
	{
		NPC_FALLING_ITEM = 0,
		NPC_FISH_HOOK = 1,
		NPC_DUST_DEVIL,
		NPC_PENDULUM,
		NPC_FIREBALL,
		NPC_SAW_BLADE,
		NPC_SMALL_JELLYFISH_1,
		NPC_SMALL_JELLYFISH_2,
		NPC_ANEMONE_1,
		NPC_ANEMONE_2,
		NPC_ANEMONE_3,
		NPC_SKELETAL_FISH,
		NPC_CLAM_JUMP,
		NPC_CLAM_STATIC,
		NPC_SQUID_DART,
		NPC_FISH_FOLK,
		NPC_PRICKLY_BUG,
		NPC_SEA_SNAKE,
		NPC_PUFFA_FISH,
		NPC_ANGLER_FISH,
		NPC_HERMIT_CRAB,
		NPC_MINE,
		NPC_BOOGER_MONSTER,
		NPC_SPIDER_CRAB,
		NPC_SPIDER_CRAB_SPAWNER,
		NPC_EYEBALL,
		NPC_BABY_OCTOPUS,
		NPC_ZOMBIE_FISH_FOLK,
		NPC_NINJA_STARFISH,
		NPC_GHOST,
		NPC_GHOST_PIRATE,
		NPC_FLAMING_SKULL,
		NPC_SHARK_MAN,
		NPC_OIL_BLOB,
		NPC_SKULL_STOMPER,
		NPC_MOTHER_JELLYFISH,
		NPC_SUB_SHARK,
		NPC_PARASITIC_WORM,
		NPC_FLYING_DUTCHMAN,
		NPC_IRON_DOGFISH,
		NPC_PARASITIC_WORM_SEGMENT,
		NPC_BALL_BLOB,
		NPC_UNIT_TYPE_MAX,
	};

	void				init();
	virtual void		postInit();
	void				shutdown();
	virtual void		think(int _frames);
	virtual void		render();
	void				processEvent( GAME_EVENT evt, CThing *sourceThing );
	void				setLayerCollision( class CLayerCollision *_layer )		{m_layerCollision=_layer;}
	void				setType( NPC_UNIT_TYPE newType )						{m_type = newType;}
	static NPC_UNIT_TYPE		getTypeFromMapEdit( u16 newType );
	void				setHeading( s32 newHeading )							{m_heading = newHeading;}
	void				setHeading( s32 xPos, s32 yPos );
	void				addWaypoint( s32 xPos, s32 yPos );
	void				setPathType( u8 newType )								{m_npcPath.setPathType( newType );}
	void				setStartPos( s32 xPos, s32 yPos );
	void				hasBeenAttacked();
	bool				canBeCaughtByNet();
	void				caughtWithNet();
	virtual int			getFrameCount();

	static CNpcEnemy	*Create(sThingActor *ThisActor);
	void				setupWaypoints( sThingActor *ThisActor );


protected:
	class CLayerCollision	*m_layerCollision;

protected:
	// NPC data structure definitions //

	enum NPC_CONTROL_FUNC
	{
		NPC_CONTROL_NONE,
		NPC_CONTROL_MOVEMENT,
		NPC_CONTROL_SHOT,
		NPC_CONTROL_CLOSE,
		NPC_CONTROL_COLLISION,
	};

	enum NPC_SENSOR_FUNC
	{
		NPC_SENSOR_NONE = 0,
		NPC_SENSOR_USER_CLOSE = 1,
		NPC_SENSOR_GENERIC_USER_CLOSE,
		NPC_SENSOR_GENERIC_USER_VISIBLE,
	};

	enum NPC_CLOSE_FUNC
	{
		NPC_CLOSE_NONE = 0,
		NPC_CLOSE_GENERIC_USER_SEEK = 1,
	};

	enum NPC_MOVEMENT_FUNC
	{
		NPC_MOVEMENT_STATIC = 0,
		NPC_MOVEMENT_FIXED_PATH = 1,
		NPC_MOVEMENT_FIXED_PATH_WALK,
		NPC_MOVEMENT_STATIC_CYCLE_ANIM,
	};

	enum NPC_TIMER_FUNC
	{
		NPC_TIMER_NONE = 0,
		NPC_TIMER_EVADE_DONE = 1,
		NPC_TIMER_ATTACK_DONE,
		NPC_TIMER_RESPAWN,
	};

	enum NPC_SHOT_FUNC
	{
		NPC_SHOT_NONE = 0,
		NPC_SHOT_GENERIC = 1,
	};

	enum NPC_GENERIC_HIT_STATE
	{
		NPC_GENERIC_HIT_CHECK_HEALTH = 100,
		NPC_GENERIC_HIT_RECOIL = 101,
		NPC_GENERIC_HIT_DEATH_START,
		NPC_GENERIC_HIT_DEATH_END,
	};

	enum
	{
		EXTEND_UP = true,
		EXTEND_DOWN = false,
		EXTEND_RIGHT = true,
		EXTEND_LEFT = false,
		EXTEND_CLOCKWISE = true,
		EXTEND_ANTICLOCKWISE = false,

		NPC_ENEMY_MAPEDIT_OFFSET = 10,
	};

	enum DETECT_TYPE
	{
		DETECT_NO_COLLISION = 0,
		DETECT_ALL_COLLISION = 1,
		DETECT_ATTACK_COLLISION_GENERIC,
	};


	typedef struct NPC_DATA_TYPE
	{
		int								skelType;
//		FileEquate						animData;
		u16								initAnim;
		NPC_SENSOR_FUNC					sensorFunc;
		NPC_MOVEMENT_FUNC				movementFunc;
		NPC_CLOSE_FUNC					closeFunc;
		NPC_TIMER_FUNC					timerFunc;
		bool							canTalk;
		u8								speed;
		u16								turnSpeed;
		DETECT_TYPE						detectCollision;
		DAMAGE_TYPE						damageToUserType;
		u16								initHealth;
		u16								moveAnim;
		NPC_SHOT_FUNC					shotFunc;
		u16								dieAnim;
		u16								recoilAnim;
		bool							canBeNetted;
		bool							respawning;
	}
	NPC_DATA;

	static NPC_UNIT_TYPE mapEditConvertTable[NPC_UNIT_TYPE_MAX];

	// functions

	virtual bool		processSensor();
	virtual void		processMovement( int _frames );
	virtual void		processMovementModifier( int _frames, s32 distX, s32 distY, s32 dist, s16 headingChange );
	void				processShot();
	virtual void		processClose( int _frames );
	virtual void		processCollision();
	void				processTimer( int _frames );
	bool				isCollisionWithGround();

	void				processGenericGotoTarget( int _frames, s32 xDist, s32 yDist, s32 speed );
	void				processGenericGetUserDist( int _frames, s32 *distX, s32 *distY );
	void				processGenericFixedPathMove( int _frames, s32 *moveX, s32 *moveY, s32 *moveVel, s32 *moveDist );
	void				processGenericFixedPathWalk( int _frames, s32 *moveX, s32 *moveY );
	bool				processGroundCollisionReverse( s32 *moveX, s32 *moveY );

	void				reinit();

	// data

	static NPC_DATA		m_data[NPC_UNIT_TYPE_MAX];

	static s32			playerXDist;
	static s32			playerYDist;

	static s32			playerXDistSqr;
	static s32			playerYDistSqr;

	// internal variables
	
	NPC_UNIT_TYPE		m_type;
	NPC_CONTROL_FUNC	m_controlFunc;
	NPC_CONTROL_FUNC	m_oldControlFunc;
	NPC_TIMER_FUNC		m_timerFunc;
	NPC_SENSOR_FUNC		m_sensorFunc;
	NPC_MOVEMENT_FUNC	m_movementFunc;
	CNpcPath			m_npcPath;
	s32					m_heading;
	s32					m_fireHeading;
	s32					m_velocity;
	bool				m_evadeClockwise;
	s32					m_movementTimer;
	s32					m_timerTimer;
	s32					m_extension;
	bool				m_extendDir;
	s16					m_rotation;
	DVECTOR				m_base;
	DVECTOR				m_initPos;
	u8					m_state;
	u8					m_salvoCount;
	bool				m_animPlaying;
	bool				m_reversed;
	s32					m_health;
	bool				m_isActive;

	s32				m_frame;
	int				m_animNo;
	CActorGfx		*m_actorGfx;
	SpriteBank		*m_spriteBank;
	DVECTOR			m_drawOffset;
	POLY_FT4		*SprFrame;

	virtual void		collidedWith(CThing *_thisThing);

	// position history stuff

	class CNpcPositionHistory
	{
	public:
		DVECTOR						pos;
		CNpcPositionHistory			*next;
		CNpcPositionHistory			*prev;
	};

	CNpcPositionHistory		*m_positionHistory;
};

/*****************************************************************************/
#endif
