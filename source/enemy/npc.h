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
#include "Game/Thing.h"
#include "Gfx/Skel.h"

#ifndef __ENEMY_NPCPATH_H__
#include "enemy\npcpath.h"
#endif

#ifndef	__PLAYER_PLAYER_H__
#include "player\player.h"
#endif

/*****************************************************************************/

class	CNpc : public CThing
{
public:
	enum NPC_UNIT_TYPE
	{
		NPC_SANDY_CHEEKS = 0,
		NPC_FALLING_ITEM = 1,
		NPC_FISH_HOOK,
		NPC_DUST_DEVIL,
		NPC_PENDULUM,
		NPC_FIREBALL,
		NPC_SAW_BLADE,
		NPC_SMALL_JELLYFISH_1,
		NPC_SMALL_JELLYFISH_2,
		NPC_LARGE_JELLYFISH,
		NPC_ANEMONE_1,
		NPC_ANEMONE_2,
		NPC_ANEMONE_3,
		NPC_CLAM,
		NPC_SQUID_DART,
		NPC_FISH_FOLK,
		NPC_PRICKLY_BUG,
		NPC_SEA_SNAKE_1,
		NPC_SEA_SNAKE_2,
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
		NPC_FLYING_DUTCHMAN,
		NPC_IRON_DOGFISH,
		NPC_UNIT_TYPE_MAX,
	};

	void				init();
	void				shutdown();
	void				think(int _frames);
	virtual void		render();
	void				processEvent( GAME_EVENT evt, CThing *sourceThing );


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
		NPC_SENSOR_SHARK_MAN_USER_VISIBLE,
		NPC_SENSOR_OIL_BLOB_USER_CLOSE,
		NPC_SENSOR_ANEMONE_USER_CLOSE,
		NPC_SENSOR_EYEBALL_USER_CLOSE,
		NPC_SENSOR_SKULL_STOMPER_USER_CLOSE,
		NPC_SENSOR_BOOGER_MONSTER_USER_CLOSE,
		NPC_SENSOR_IRON_DOGFISH_USER_CLOSE,
		NPC_SENSOR_FALLING_ITEM_USER_CLOSE,
		NPC_SENSOR_FISH_HOOK_USER_CLOSE,
		NPC_SENSOR_FLAMING_SKULL_USER_CLOSE,
	};

	enum NPC_CLOSE_FUNC
	{
		NPC_CLOSE_NONE = 0,
		NPC_CLOSE_JELLYFISH_EVADE = 1,
		NPC_CLOSE_CLAM_ATTACK,
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
	};

	enum NPC_MOVEMENT_FUNC
	{
		NPC_MOVEMENT_STATIC = 0,
		NPC_MOVEMENT_FIXED_PATH = 1,
		NPC_MOVEMENT_MOTHER_JELLYFISH,
		NPC_MOVEMENT_SUB_SHARK,
		NPC_MOVEMENT_FLYING_DUTCHMAN,
		NPC_MOVEMENT_IRON_DOGFISH,
		NPC_MOVEMENT_PENDULUM,
		NPC_MOVEMENT_FIREBALL,
		NPC_MOVEMENT_RETURNING_HAZARD,
		NPC_MOVEMENT_CLAM_RETRACT,
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
		MOTHER_JELLYFISH_RETURN_TO_START = 0,
		MOTHER_JELLYFISH_CYCLE = 1,	
		MOTHER_JELLYFISH_ATTACK_PLAYER,
	};

	enum NPC_SUB_SHARK_STATE
	{
		SUB_SHARK_CYCLE = 0,
		SUB_SHARK_MINE_1 = 1,
		SUB_SHARK_MINE_2,
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

	enum
	{
		NPC_JELLYFISH_RESISTANCE = 64,
		NPC_BOOGER_MONSTER_MAX_EXTENSION = 20,
		EXTEND_UP = true,
		EXTEND_DOWN = false,
		EXTEND_RIGHT = true,
		EXTEND_LEFT = false,
	};


	typedef struct NPC_DATA_TYPE
	{
		NPC_INIT_FUNC					initFunc;
		NPC_SENSOR_FUNC					sensorFunc;
		NPC_MOVEMENT_FUNC				movementFunc;
		NPC_MOVEMENT_MODIFIER_FUNC		movementModifierFunc;
		NPC_CLOSE_FUNC					closeFunc;
		NPC_TIMER_FUNC					timerFunc;
		bool							canTalk;
		u8								speed;
		u16								turnSpeed;
		DAMAGE_TYPE						damageToUserType;
	}
	NPC_DATA;

	// functions

	bool				processSensor();
	void				processMovement( int _frames );
	void				processMovementModifier( int _frames, s32 distX, s32 distY, s32 dist, s16 headingChange );
	void				processShot();
	void				processClose( int _frames );
	void				processCollision();
	void				processTimer( int _frames );
	void				detectCollisionWithPlayer();

	void				processGenericGotoTarget( int _frames, s32 xDist, s32 yDist, s32 speed );
	void				processGenericGetUserDist( int _frames, s32 *distX, s32 *distY );

	// small jellyfish functions

	void				processSmallJellyfishSensor();
	void				processSmallJellyfishMovementModifier( int _frames, s32 distX, s32 distY, s32 dist, s16 headingChange );
	void				processCloseSmallJellyfishEvade( int _frames );

	// baby octopus functions

	void				processBabyOctopusMovementModifier( int _frames, s32 dist, s16 headingChange );

	// fish folk functions

	void				processFishFolkMovementModifier( int _frames, s32 distX, s32 distY );

	// clam functions

	void				processCloseClamAttack( int _frames );
	void				processClamRetract( int _frames );

	// spider crab functions

	void				processCloseSpiderCrabAttack( int _frames );

	// ghost pirate functions

	void				processCloseGhostPirateAttack( int _frames );

	// shark man functions

	void				processCloseSharkManAttack( int _frames );

	// anemone functions

	void				processCloseAnemone1Attack( int _frames );
	void				processCloseAnemone2Attack( int _frames );
	void				processCloseAnemone3Attack( int _frames );

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
	DVECTOR				m_base;
	u8					m_state;
	u8					m_salvoCount;
};


/*****************************************************************************/
#endif
