/*=========================================================================

	nplatfrm.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef	__ENEMY_NPLATFRM_H__
#define	__ENEMY_NPLATFRM_H__

//#include <dstructs.h>

#ifndef __THING_THING_H__
#include "thing/thing.h"
#endif

#include "Gfx/Actor.h"

#ifndef __ENEMY_NPCPATH_H__
#include "enemy\npcpath.h"
#endif

#ifndef	__PLAYER_PLAYER_H__
#include "player\player.h"
#endif

// temporary
#ifndef __GFX_SPRBANK_H__
#include "gfx\sprbank.h"
#endif

/*****************************************************************************/

class	CNpcPlatform : public CPlatformThing
{
public:
	enum NPC_PLATFORM_UNIT_TYPE
	{
		NPC_LINEAR_PLATFORM = 0,
		NPC_CIRCULAR_PLATFORM = 1,
		NPC_BUBBLE_PLATFORM,
		NPC_COLLAPSING_BUBBLE_PLATFORM,
		NPC_FISH_HOOK_PLATFORM,
		NPC_RETRACTING_PLATFORM,
		NPC_GEYSER_PLATFORM,
		NPC_BOBBING_PLATFORM,
		NPC_FALLING_PLATFORM,
		NPC_CART_PLATFORM,
		NPC_PLAYER_BUBBLE_PLATFORM,
		NPC_PLATFORM_TYPE_MAX,
	};

	void				init();
	void				init( DVECTOR initPos );
	void				init( DVECTOR initPos, s32 initLifetime );
	void				shutdown();
	void				think(int _frames);
	void				render();
	void				setLayerCollision( class CLayerCollision *_layer )		{m_layerCollision=_layer;}
	void				setType( NPC_PLATFORM_UNIT_TYPE newType )				{m_type = newType;}
	void				setTypeFromMapEdit( u16 newType );
	virtual int			checkCollisionAgainst(CThing *_thisThing, int _frames);
	virtual s32			getNewYPos( CThing *_thisThing );
	void				setTiltable( bool isTiltable );
	void				addWaypoint( s32 xPos, s32 yPos );

protected:
	// NPC data structure definitions //

	enum NPC_PLATFORM_MOVEMENT_FUNC
	{
		NPC_PLATFORM_MOVEMENT_STATIC = 0,
		NPC_PLATFORM_MOVEMENT_FIXED_PATH = 1,
		NPC_PLATFORM_MOVEMENT_FIXED_CIRCULAR,
		NPC_PLATFORM_MOVEMENT_BUBBLE,
		NPC_PLATFORM_MOVEMENT_FISH_HOOK,
		NPC_PLATFORM_MOVEMENT_GEYSER,
		NPC_PLATFORM_MOVEMENT_BOB,
		NPC_PLATFORM_MOVEMENT_FALL,
		NPC_PLATFORM_MOVEMENT_CART,
		NPC_PLATFORM_MOVEMENT_PLAYER_BUBBLE,
	};

	enum NPC_PLATFORM_LIFETIME_TYPE
	{
		NPC_PLATFORM_FINITE_LIFE = 0,
		NPC_PLATFORM_INFINITE_LIFE = 1,
		NPC_PLATFORM_FINITE_LIFE_RESPAWN,
		NPC_PLATFORM_INFINITE_LIFE_COLLAPSIBLE,
		NPC_PLATFORM_INFINITE_LIFE_FISH_HOOK,
	};

	enum NPC_PLATFORM_TIMER_TYPE
	{
		NPC_PLATFORM_TIMER_NONE = 0,
		NPC_PLATFORM_TIMER_RESPAWN = 1,
		NPC_PLATFORM_TIMER_RETRACT,
		NPC_PLATFORM_TIMER_EXTEND,
		NPC_PLATFORM_TIMER_GEYSER,
	};

	enum NPC_BOB_STATE
	{
		NPC_BOB_STOP = 0,
		NPC_BOB_MOVE = 1,
	};

	typedef struct NPC_PLATFORM_DATA_TYPE
	{
		FileEquate						ActorType;
//		FileEquate						animData;
		u16								initAnim;
		NPC_PLATFORM_MOVEMENT_FUNC		movementFunc;
		u8								speed;
		u16								turnSpeed;
		bool							detectCollision;
		DAMAGE_TYPE						damageToUserType;
		u16								initHealth;
		s32								lifetime;
		NPC_PLATFORM_LIFETIME_TYPE		lifetimeType;
		s32								initTimer;
		NPC_PLATFORM_TIMER_TYPE			initTimerType;
	}
	NPC_PLATFORM_DATA;

	// functions

	void				reinit();
	bool				processSensor();
	void				processMovement( int _frames );
	void				processMovementModifier( int _frames, s32 distX, s32 distY, s32 dist, s16 headingChange );
	void				processShot();
	void				processClose( int _frames );
	void				processCollision();
	void				processTimer( int _frames );
	void				processTilt( int _frames );
	bool				isCollisionWithGround();

	void				processGenericFixedPathMove( int _frames, s32 *moveX, s32 *moveY, s32 *moveVel, s32 *moveDist );
	void				processGenericCircularPath( int _frames );
	void				processGeyserMove( int _frames, s32 *moveX, s32 *moveY );
	void				processFallingMove( int _frames, s32 *moveX, s32 *moveY );
	void				processBobMove( int _frames, s32 *moveX, s32 *moveY );
	void				processCartMove( int _frames, s32 *moveX, s32 *moveY );

	// data

	static NPC_PLATFORM_DATA		m_data[NPC_PLATFORM_TYPE_MAX];

	static s32			playerXDist;
	static s32			playerYDist;

	static s32			playerXDistSqr;
	static s32			playerYDistSqr;

	static class CLayerCollision	*m_layerCollision;

	// internal variables
	
	NPC_PLATFORM_UNIT_TYPE		m_type;
	CNpcPath					m_npcPath;
	s32							m_heading;
	s32							m_velocity;
	s16							m_rotation;
	DVECTOR						m_base;
	DVECTOR						m_initPos;
	u8							m_state;
	bool						m_animPlaying;
	bool						m_reversed;
	s32							m_extension;
	s32							m_lifetime;
	s32							m_initLifetime;
	NPC_PLATFORM_LIFETIME_TYPE	m_lifetimeType;
	bool						m_contact;
	s32							m_timer;
	bool						m_isActive;
	NPC_PLATFORM_TIMER_TYPE		m_timerType;
	NPC_PLATFORM_MOVEMENT_FUNC	m_movementFunc;
	bool						m_detectCollision;
	bool						m_tiltable;
	s32							m_tiltAngle;
	s32							m_tiltVelocity;

	int							m_frame;
	int							m_animNo;
	CModelGfx					*m_modelGfx;

	virtual void		collidedWith(CThing *_thisThing);

	static NPC_PLATFORM_UNIT_TYPE mapEditConvertTable[NPC_PLATFORM_TYPE_MAX];
};


/*****************************************************************************/
#endif
