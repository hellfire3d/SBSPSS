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

/*****************************************************************************/

class	CNpc : public CThing
{
public:
	void				init();
	void				shutdown();
	void				think(int _frames);
	virtual void		render();
	void				processEvent( GAME_EVENT evt, CThing *sourceThing );


protected:
	// NPC data structure definitions //

	enum NPC_UNIT_TYPE
	{
		NPC_TEST_TYPE = 0,
		NPC_SANDY_CHEEKS = 1,
		NPC_SMALL_JELLYFISH_1,
		NPC_SMALL_JELLYFISH_2,
		NPC_LARGE_JELLYFISH,
		NPC_ANEMONE_1,
		NPC_ANEMONE_2,
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
		NPC_UNIT_TYPE_MAX,
	};

	enum NPC_INIT_FUNC
	{
		NPC_INIT_DEFAULT = 0,
		NPC_INIT_SNAKE = 1,
		NPC_INIT_ACID,
		NPC_INIT_GHOST_PIRATE,
	};

	enum NPC_CONTROL_FUNC
	{
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
	};

	enum NPC_MOVEMENT_FUNC
	{
		NPC_MOVEMENT_STATIC = 0,
		NPC_MOVEMENT_FIXED_PATH = 1,
		NPC_MOVEMENT_USER_SEEK,
		NPC_MOVEMENT_VERTICAL,
	};

	enum NPC_MOVEMENT_MODIFIER_FUNC
	{
		NPC_MOVEMENT_MODIFIER_NONE = 0,
		NPC_MOVEMENT_MODIFIER_BOB = 1,
		NPC_MOVEMENT_MODIFIER_JELLYFISH,
	};

	enum NPC_TIMER_FUNC
	{
		NPC_TIMER_NONE = 0,
		NPC_TIMER_EVADE_DONE = 1,
		NPC_TIMER_ATTACK_DONE,
	};

	enum
	{
		NPC_JELLYFISH_RESISTANCE = 64,
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

	void				processCloseGenericUserSeek( int _frames );

	// small jellyfish functions

	void				processSmallJellyfishSensor();
	void				processSmallJellyfishMovementModifier( int _frames, s32 distX, s32 distY, s32 dist, s16 headingChange );
	void				processCloseSmallJellyfishEvade( int _frames );

	// clam functions

	void				processCloseClamAttack( int _frames );

	// spider crab functions

	void				processCloseSpiderCrabAttack( int _frames );

	// ghost pirate functions

	void				processCloseGhostPirateAttack( int _frames );

	// shark man functions

	void				processCloseSharkManAttack( int _frames );

	// anemone functions

	void				processCloseAnemone1Attack( int _frames );
	void				processCloseAnemone2Attack( int _frames );

	// data

	static NPC_DATA		m_data[NPC_UNIT_TYPE_MAX];

	// internal variables
	
	NPC_UNIT_TYPE		m_type;
	NPC_CONTROL_FUNC	m_controlFunc;
	NPC_TIMER_FUNC		m_timerFunc;
	NPC_SENSOR_FUNC		m_sensorFunc;
	CNpcPath			m_npcPath;
	s32					m_heading;
	s32					m_fireHeading;
	s32					m_movementTimer;
	s32					m_velocity;
	bool				m_evadeClockwise;
	s32					m_timerTimer;
	s32					m_extension;
	bool				m_extendDir;
	DVECTOR				m_extensionBase;
};


/*****************************************************************************/
#endif
