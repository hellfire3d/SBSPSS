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
		//NPC_FRIEND_MOVEMENT_MODIFIER_FUNC		movementModifierFunc;
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

	//class SpriteBank	*m_spriteBank;

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
	void				postInit();
	void				shutdown();
	void				think(int _frames);
	void				render();
	void				processEvent( GAME_EVENT evt, CThing *sourceThing );
	void				setLayerCollision( class CLayerCollision *_layer )		{m_layerCollision=_layer;}
	void				setType( NPC_UNIT_TYPE newType )						{m_type = newType;}
	void				setTypeFromMapEdit( u16 newType );
	void				setHeading( s32 newHeading )							{m_heading = newHeading;}
	void				addWaypoint( s32 xPos, s32 yPos );
	void				setPathType( u8 newType )								{m_npcPath.setPathType( newType );}
	void				setStartPos( s32 xPos, s32 yPos );
	void				hasBeenAttacked();

	static void			CacheActor(int Type);

private:
	class CLayerCollision	*m_layerCollision;

protected:
	// NPC data structure definitions //

	enum NPC_INIT_FUNC
	{
		NPC_INIT_DEFAULT = 0,
		NPC_INIT_SNAKE = 1,
		NPC_INIT_ACID,
		NPC_INIT_GHOST_PIRATE,
		NPC_INIT_SKULL_STOMPER,
		NPC_INIT_MOTHER_JELLYFISH,
		NPC_INIT_SUB_SHARK,
		NPC_INIT_FLYING_DUTCHMAN,
		NPC_INIT_IRON_DOGFISH,
		NPC_INIT_FALLING_ITEM,
		NPC_INIT_FISH_HOOK,
		NPC_INIT_PENDULUM,
		NPC_INIT_FIREBALL,
		NPC_INIT_RETURNING_HAZARD,
		NPC_INIT_FISH_FOLK,
		NPC_INIT_FLAMING_SKULL,
		NPC_INIT_CIRCULAR_PLATFORM,
		NPC_INIT_PARASITIC_WORM,
		NPC_INIT_PARASITIC_WORM_SEGMENT,
		NPC_INIT_HERMIT_CRAB,
		NPC_INIT_EYEBALL,
		NPC_INIT_BALL_BLOB,
		NPC_INIT_ANEMONE_2,
	};

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
		NPC_SENSOR_JELLYFISH_USER_CLOSE = 1,
		NPC_SENSOR_CLAM_USER_CLOSE,
		NPC_SENSOR_SPIDER_CRAB_USER_CLOSE,
		NPC_SENSOR_NINJA_STARFISH_USER_CLOSE,
		NPC_SENSOR_GHOST_PIRATE_USER_CLOSE,
		NPC_SENSOR_GENERIC_USER_VISIBLE,
		NPC_SENSOR_OIL_BLOB_USER_CLOSE,
		NPC_SENSOR_ANEMONE_USER_CLOSE,
		NPC_SENSOR_EYEBALL_USER_CLOSE,
		NPC_SENSOR_SKULL_STOMPER_USER_CLOSE,
		NPC_SENSOR_BOOGER_MONSTER_USER_CLOSE,
		NPC_SENSOR_IRON_DOGFISH_USER_CLOSE,
		NPC_SENSOR_FALLING_ITEM_USER_CLOSE,
		NPC_SENSOR_FISH_HOOK_USER_CLOSE,
		NPC_SENSOR_FLAMING_SKULL_USER_CLOSE,
		NPC_SENSOR_HERMIT_CRAB_USER_CLOSE,
		NPC_SENSOR_OCTOPUS_USER_CLOSE,
		NPC_SENSOR_PUFFA_FISH_USER_CLOSE,
	};

	enum NPC_CLOSE_FUNC
	{
		NPC_CLOSE_NONE = 0,
		NPC_CLOSE_JELLYFISH_EVADE = 1,
		NPC_CLOSE_CLAM_JUMP_ATTACK,
		NPC_CLOSE_CLAM_SNAP_ATTACK,
		NPC_CLOSE_SPIDER_CRAB_ATTACK,
		NPC_CLOSE_GHOST_PIRATE_ATTACK,
		NPC_CLOSE_SHARK_MAN_ATTACK,
		NPC_CLOSE_GENERIC_USER_SEEK,
		NPC_CLOSE_ANEMONE_1_ATTACK,
		NPC_CLOSE_ANEMONE_2_ATTACK,
		NPC_CLOSE_ANEMONE_3_ATTACK,
		NPC_CLOSE_EYEBALL_ATTACK,
		NPC_CLOSE_SKULL_STOMPER_ATTACK,
		NPC_CLOSE_BOOGER_MONSTER_ATTACK,
		NPC_CLOSE_MOTHER_JELLYFISH_ATTACK,
		NPC_CLOSE_SUB_SHARK_ATTACK,
		NPC_CLOSE_FLYING_DUTCHMAN_ATTACK,
		NPC_CLOSE_IRON_DOGFISH_ATTACK,
		NPC_CLOSE_FALLING_ITEM_FALL,
		NPC_CLOSE_FISH_HOOK_RISE,
		NPC_CLOSE_FLAMING_SKULL_ATTACK,
		NPC_CLOSE_SKELETAL_FISH_ATTACK,
		NPC_CLOSE_HERMIT_CRAB_ATTACK,
		NPC_CLOSE_OCTOPUS_ATTACK,
		NPC_CLOSE_PUFFA_FISH_INFLATE,
	};

	enum NPC_MOVEMENT_FUNC
	{
		NPC_MOVEMENT_STATIC = 0,
		NPC_MOVEMENT_FIXED_PATH = 1,
		NPC_MOVEMENT_FIXED_PATH_WALK,
		NPC_MOVEMENT_MOTHER_JELLYFISH,
		NPC_MOVEMENT_SUB_SHARK,
		NPC_MOVEMENT_FLYING_DUTCHMAN,
		NPC_MOVEMENT_IRON_DOGFISH,
		NPC_MOVEMENT_PENDULUM,
		NPC_MOVEMENT_FIREBALL,
		NPC_MOVEMENT_RETURNING_HAZARD,
		NPC_MOVEMENT_CLAM_RETRACT,
		NPC_MOVEMENT_PARASITIC_WORM,
		NPC_MOVEMENT_STATIC_CYCLE_ANIM,
		NPC_MOVEMENT_SHARK_MAN,
		NPC_MOVEMENT_BALL_BLOB,
	};

	enum NPC_MOVEMENT_MODIFIER_FUNC
	{
		NPC_MOVEMENT_MODIFIER_NONE = 0,
		NPC_MOVEMENT_MODIFIER_BOB = 1,
		NPC_MOVEMENT_MODIFIER_JELLYFISH,
		NPC_MOVEMENT_MODIFIER_FISH_FOLK,
		NPC_MOVEMENT_MODIFIER_OCTOPUS,
	};

	enum NPC_TIMER_FUNC
	{
		NPC_TIMER_NONE = 0,
		NPC_TIMER_EVADE_DONE = 1,
		NPC_TIMER_ATTACK_DONE,
	};

	enum NPC_MOTHER_JELLYFISH_STATE
	{
		MOTHER_JELLYFISH_RETURN_TO_START_1 = 0,
		MOTHER_JELLYFISH_CYCLE_1 = 1,	
		MOTHER_JELLYFISH_ATTACK_PLAYER_SPAWN_1,
		MOTHER_JELLYFISH_RETURN_TO_START_2,
		MOTHER_JELLYFISH_CYCLE_2,
		MOTHER_JELLYFISH_ATTACK_PLAYER_SPAWN_2,
		MOTHER_JELLYFISH_RETURN_TO_START_3,
		MOTHER_JELLYFISH_CYCLE_3,
		MOTHER_JELLYFISH_ATTACK_PLAYER_SHOCK,
	};

	enum NPC_SUB_SHARK_STATE
	{
		SUB_SHARK_MINE_1 = 0,
		SUB_SHARK_MINE_2 = 1,
		SUB_SHARK_CYCLE,
		SUB_SHARK_SWALLOW,
	};

	enum NPC_FLYING_DUTCHMAN_STATE
	{
		FLYING_DUTCHMAN_ATTACK_PLAYER_1 = 0,
		FLYING_DUTCHMAN_ATTACK_PLAYER_2 = 1,
		FLYING_DUTCHMAN_ATTACK_PLAYER_3,
	};

	enum NPC_IRON_DOGFISH_STATE
	{
		IRON_DOGFISH_THUMP_1 = 0,
		IRON_DOGFISH_LASER_EYE_1 = 1,
		IRON_DOGFISH_THUMP_2,
		IRON_DOGFISH_ROLL,
		IRON_DOGFISH_LASER_EYE_2,
	};

	enum NPC_FLAMING_SKULL_STATE
	{
		FLAMING_SKULL_ATTACK = 0,
		FLAMING_SKULL_RETURN = 1,
	};

	enum NPC_HERMIT_CRAB_STATE
	{
		HERMIT_CRAB_NO_ATTACK = 0,
		HERMIT_CRAB_PUNCH_ATTACK = 1,
		HERMIT_CRAB_ROLL_ATTACK,
	};

	enum NPC_PUFFA_FISH_STATE
	{
		PUFFA_FISH_NO_INFLATE = 0,
		PUFFA_FISH_TURN = 1,
		PUFFA_FISH_INFLATE,
	};

	enum
	{
		NPC_JELLYFISH_RESISTANCE = 64,
		NPC_BOOGER_MONSTER_MAX_EXTENSION = 20,
		NPC_SUB_SHARK_HIGH_SPEED = 6,
		NPC_PARASITIC_WORM_SPACING = 6,
		EXTEND_UP = true,
		EXTEND_DOWN = false,
		EXTEND_RIGHT = true,
		EXTEND_LEFT = false,
		EXTEND_CLOCKWISE = true,
		EXTEND_ANTICLOCKWISE = false,

		NPC_ENEMY_MAPEDIT_OFFSET = 10,
	};


	typedef struct NPC_DATA_TYPE
	{
		FileEquate						skelType;
//		FileEquate						animData;
		u16								initAnim;
		NPC_INIT_FUNC					initFunc;
		NPC_SENSOR_FUNC					sensorFunc;
		NPC_MOVEMENT_FUNC				movementFunc;
		NPC_MOVEMENT_MODIFIER_FUNC		movementModifierFunc;
		NPC_CLOSE_FUNC					closeFunc;
		NPC_TIMER_FUNC					timerFunc;
		bool							canTalk;
		u8								speed;
		u16								turnSpeed;
		bool							detectCollision;
		DAMAGE_TYPE						damageToUserType;
		u16								initHealth;
		u16								moveAnim;
	}
	NPC_DATA;

	static NPC_UNIT_TYPE mapEditConvertTable[NPC_UNIT_TYPE_MAX];

	// functions

	bool				processSensor();
	void				processMovement( int _frames );
	void				processMovementModifier( int _frames, s32 distX, s32 distY, s32 dist, s16 headingChange );
	void				processShot();
	void				processClose( int _frames );
	void				processCollision();
	void				processTimer( int _frames );
	bool				isCollisionWithGround();

	void				processGenericGotoTarget( int _frames, s32 xDist, s32 yDist, s32 speed );
	void				processGenericGetUserDist( int _frames, s32 *distX, s32 *distY );
	void				processGenericFixedPathMove( int _frames, s32 *moveX, s32 *moveY, s32 *moveVel, s32 *moveDist );
	void				processGenericFixedPathWalk( int _frames, s32 *moveX, s32 *moveY );
	bool				processGroundCollisionReverse( s32 *moveX, s32 *moveY );

	// small jellyfish functions

	void				processSmallJellyfishSensor();
	void				processSmallJellyfishMovementModifier( int _frames, s32 distX, s32 distY, s32 dist, s16 headingChange );
	void				processCloseSmallJellyfishEvade( int _frames );

	// baby octopus functions

	void				processBabyOctopusMovementModifier( int _frames, s32 dist, s16 headingChange );
	void				processCloseOctopusAttack( int _frames );

	// fish folk functions

	void				processFishFolkMovementModifier( int _frames, s32 distX, s32 distY );

	// ball blob functions

	void				processBallBlobMovement( int _frames, s32 *moveX, s32 *moveY );

	// clam functions

	void				processCloseClamJumpAttack( int _frames );
	void				processCloseClamSnapAttack( int _frames );

	// spider crab functions

	void				processCloseSpiderCrabAttack( int _frames );

	// hermit crab functions

	void				processCloseHermitCrabAttack( int _frames );

	// ghost pirate functions

	void				processCloseGhostPirateAttack( int _frames );

	// puffa fish functions

	void				processClosePuffaFishInflate( int _frames );

	// shark man functions

	void				processSharkManMovement( int _frames, s32 *moveX, s32 *moveY );
	void				processCloseSharkManAttack( int _frames );

	// anemone functions

	void				processCloseAnemone1Attack( int _frames );
	void				processCloseAnemone2Attack( int _frames );
	void				processCloseAnemone3Attack( int _frames );

	// skeletal fish functions

	void				processCloseSkeletalFishAttack( int _frames );

	// eyeball functions

	void				processCloseEyeballAttack( int _frames );

	// flaming skull functions

	void				processCloseFlamingSkullAttack( int _frames );

	// skull stomper functions

	void				processCloseSkullStomperAttack( int _frames );

	// booger monster functions

	void				processCloseBoogerMonsterAttack( int _frames );

	// mother jellyfish functions

	void				processMotherJellyfishMovement( int _frames );
	void				processCloseMotherJellyfishAttack( int _frames );

	// sub shark functions

	void				processSubSharkMovement( int _frames );
	void				processCloseSubSharkAttack( int _frames );

	// parasitic worm functions

	void				processParasiticWormMovement( int _frames );

	// flying dutchman functions

	void				processFlyingDutchmanMovement( int _frames );
	void				processCloseFlyingDutchmanAttack( int _frames );

	// iron dogfish functions

	void				processIronDogfishMovement( int _frames );
	void				processStandardIronDogfishAttack( int _frames );
	void				processCloseIronDogfishAttack( int _frames );

	// hazard functions

	void				processCloseFallingItemFall( int _frames );
	void				processCloseFishHookRise( int _frames );
	void				processPendulumMovement( int _frames );
	void				processFireballMovement( int _frames );
	void				processReturningHazardMovement( int _frames );

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
	u8					m_state;
	u8					m_salvoCount;
	bool				m_animPlaying;
	bool				m_reversed;
	s32					m_health;

	s32				m_frame;
	int				m_animNo;
	CActorGfx		*m_actorGfx;
	DVECTOR			m_drawOffset;

	virtual void		collidedWith(CThing *_thisThing);

	// position history stuff

	class CNpcPositionHistory
	{
	public:
		DVECTOR						pos;
		CNpcPositionHistory			*next;
	};

	CNpcPositionHistory		*m_positionHistory;
};


/*****************************************************************************/
#endif
