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
		NPC_ANEMONE_1,
		NPC_CLAM,
		NPC_UNIT_TYPE_MAX,
	};

	enum NPC_INIT_FUNC
	{
		NPC_INIT_DEFAULT = 0,
		NPC_INIT_SNAKE = 1,
		NPC_INIT_ACID,
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
	};

	enum NPC_CLOSE_FUNC
	{
		NPC_CLOSE_NONE = 0,
		NPC_CLOSE_JELLYFISH_EVADE = 1,
		NPC_CLOSE_CLAM_ATTACK,
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

	// small jellyfish functions

	void				processSmallJellyfishSensor();
	void				processSmallJellyfishMovementModifier( int _frames, s32 distX, s32 distY, s32 dist, s16 headingChange );
	void				processCloseSmallJellyfishEvade( int _frames );

	// clam functions

	void				processCloseClamAttack( int _frames );

	// data

	static NPC_DATA		m_data[NPC_UNIT_TYPE_MAX];

	// internal variables
	
	NPC_UNIT_TYPE		m_type;
	NPC_CONTROL_FUNC	m_controlFunc;
	NPC_TIMER_FUNC		m_timerFunc;
	NPC_SENSOR_FUNC		m_sensorFunc;
	CNpcPath			m_npcPath;
	s32					m_heading;
	s32					m_movementTimer;
	s32					m_velocity;
	bool				m_evadeClockwise;
	s32					m_timerTimer;
	s32					m_extension;
	bool				m_extendOut;
};


/*****************************************************************************/
#endif
