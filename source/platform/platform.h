/*=========================================================================

	platform.h

	Author:		CRB
	Created:
	Project:	Spongebob
	Purpose:

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef	__PLATFORM_PLATFORM_H__
#define	__PLATFORM_PLATFORM_H__

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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// All platforms are fied to this width at the minute..
#define PLATFORMWIDTH				80

// The collision box is this high.. if SB keeps falling through platforms then it *should* be sufficient
// just to up this a bit
#define PLATFORMCOLLISIONHEIGHT		90


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
		NPC_FISH_HOOK_2_PLATFORM,
		NPC_BRANCH_PLATFORM,
		NPC_SEESAW_PLATFORM,
		NPC_OILDRUM_PLATFORM,
		NPC_CRATE_PLATFORM,
		NPC_BOUNCE_PLATFORM,
		NPC_DUAL_PLATFORM,
		NPC_OILDRUM_GENERATOR,
		NPC_CRATE_GENERATOR,
		NPC_RAFT_PLATFORM,
		NPC_VERTICAL_OILDRUM_GENERATOR,
		NPC_VERTICAL_OILDRUM_PLATFORM,
		NPC_LANTERN_PLATFORM,
		NPC_BUBBLE_GEYSER_GENERATOR,
		NPC_LEAF_PLATFORM,
		NPC_BIG_WHEEL_PLATFORM,
		NPC_STEERABLE_BARREL_PLATFORM,
		NPC_JELLYFISH_PLATFORM,
		NPC_FISH_HOOK_3_PLATFORM,
		NPC_RISING_BRIDGE_PLATFORM,
		NPC_BALLOON_BRIDGE_PLATFORM,
		NPC_TRAPDOOR_PLATFORM,
		NPC_CONVEYOR_GENERATOR,
		NPC_CONVEYOR_PLATFORM,
		NPC_PLAYER_BUBBLE_PLATFORM,
		NPC_CLAM_PLATFORM,
		NPC_COLLAPSING_ACRID_PLATFORM,
		NPC_DROP_PLATFORM,
		NPC_STEAM_SWITCH_PLATFORM,
		NPC_LIFT_PLATFORM,
		NPC_LOVE_BOAT_PLATFORM,
		NPC_STEERABLE_OILDRUM_PLATFORM,
		NPC_BUBBLE_TUBE_PLATFORM,
		NPC_FALLING_BLOCK_PLATFORM,
		NPC_GHOST_TRAIN_PLATFORM,
		NPC_PLATFORM_TYPE_MAX,
	};
	enum
	{ // For Dynamic ThingCache
		MAX_SUBTYPE	=NPC_PLATFORM_TYPE_MAX,
	};

	void				init();
	void				init( DVECTOR initPos );
	void				init( DVECTOR initPos, s32 initLifetime );
	virtual void		postInit();
	virtual void		shutdown();
	virtual void		think(int _frames);
	virtual void		render();
	virtual u8			canDrop()												{return true;}
	void				setTypeFromMapEdit( u16 newType );
	virtual int			checkCollisionAgainst(CThing *_thisThing, int _frames);
	virtual u8			checkCollisionDelta( CThing *_thisThing, int threshold, CRECT collisionArea );
	virtual int			getHeightFromPlatformAtPosition(int _x,int _y, int offsetX = 0, int offsetY = 0);
	void				setTiltable( bool isTiltable );
	void				setWaypointPtr( u16 *newPtr )							{m_npcPath.setWaypointPtr( newPtr );}
	void				setWaypointCount( u8 newCount )							{m_npcPath.setWaypointCount( newCount );}
	virtual void		setGraphic( sThingPlatform *ThisPlatform );
	virtual void		setGraphic( u8 graphicNum );
	virtual void		setWaypoints( sThingPlatform *ThisPlatform );
	virtual void		trigger()												{;}
	virtual u8			isCart()												{return( false );}
	virtual void		jump()													{;}
	virtual void		slowDown()												{;}
	virtual void		speedUp()												{;}
	virtual	void		leftThinkZone(int _frames);
	s16					getCollisionAngle()										{return m_collisionAngle;}
	virtual	CRECT const	*getThinkBBox()											{return &m_thinkArea;}

	static NPC_PLATFORM_UNIT_TYPE	getTypeFromMapEdit( u16 newType );
	static CNpcPlatform	*Create(int Type);
	static CNpcPlatform	*Create(sThingPlatform *ThisPlatform);

	virtual void		setSpeed( s16 newSpeed )								{m_speed = newSpeed;}
	virtual bool		alwaysThink();

protected:
	// NPC data structure definitions //

	enum NPC_PLATFORM_LIFETIME_TYPE
	{
		NPC_PLATFORM_FINITE_LIFE = 0,
		NPC_PLATFORM_INFINITE_LIFE = 1,
		NPC_PLATFORM_FINITE_LIFE_RESPAWN,
		NPC_PLATFORM_INFINITE_LIFE_COLLAPSIBLE,
	};

	enum NPC_PLATFORM_TIMER_TYPE
	{
		NPC_PLATFORM_TIMER_NONE = 0,
		NPC_PLATFORM_TIMER_RESPAWN = 1,
	};

	enum
	{
		EXTEND_UP = true,
		EXTEND_DOWN = false,
		EXTEND_RIGHT = true,
		EXTEND_LEFT = false,
		EXTEND_CLOCKWISE = true,
		EXTEND_ANTICLOCKWISE = false,
	};

	typedef struct NPC_PLATFORM_DATA_TYPE
	{
		u8								speed;
		u16								turnSpeed;
		bool							detectCollision;
		DAMAGE_TYPE						damageToUserType;
		u16								initHealth;
		s32								lifetime;
		NPC_PLATFORM_LIFETIME_TYPE		lifetimeType;
		s32								initTimer;
		u8								initTimerType;
	}
	NPC_PLATFORM_DATA;

	// functions

	virtual void		reinit();
	bool				processSensor();
	virtual void		processMovement( int _frames );
	virtual void		processLifetime( int _frames );
	void				processClose( int _frames );
	void				processCollision();
	virtual void		processTimer( int _frames );
	void				processTilt( int _frames );
	bool				isCollisionWithGround();

	// data

	static NPC_PLATFORM_DATA		m_data[NPC_PLATFORM_TYPE_MAX];

	static s32			playerXDist;
	static s32			playerYDist;

	static s32			playerXDistSqr;
	static s32			playerYDistSqr;

	// internal variables

//	NPC_PLATFORM_UNIT_TYPE		m_type;
	NPC_PLATFORM_DATA			*m_dataPtr;
	CNpcPath					m_npcPath;
	s32							m_heading;
	s32							m_velocity;
	s16							m_rotation;
	DVECTOR						m_base;
	DVECTOR						m_initPos;
	u8							m_state;
	bool						m_reversed;
	s32							m_extension;
	s32							m_lifetime;
	s32							m_initLifetime;
	NPC_PLATFORM_LIFETIME_TYPE	m_lifetimeType;
	bool						m_contact;
	s32							m_timer;
	bool						m_isActive;
	u8							m_timerType;
	bool						m_detectCollision;
	bool						m_tiltable;
	s32							m_tiltAngle;
	s32							m_tiltVelocity;
	bool						m_extendDir;
	s16							m_speed;
	s16							m_initRotation;

	CModelGfx					*m_modelGfx;

	int							m_graphicNum;

	sBBox						m_nonRotatedCollisionArea;
	DVECTOR						m_nonRotatedCollisionOffset;

	int							m_soundId;

	virtual void		collidedWith(CThing *_thisThing);

	static NPC_PLATFORM_UNIT_TYPE mapEditConvertTable[NPC_PLATFORM_TYPE_MAX];



protected:
	virtual void	calculateNonRotatedCollisionData();
	virtual void	calculateBoundingBoxSize();

	virtual void	setCollisionAngle(int newAngle);	// Actually.. this probly doesn't need to be in the base calss anymore.. :/
	s16				m_collisionAngle;

	CRECT				m_thinkArea;

};


/*****************************************************************************/
#endif