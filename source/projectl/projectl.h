/*=========================================================================

	projectl.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef	__PROJECTL_PROJECTL_H__
#define	__PROJECTL_PROJECTL_H__

#ifndef __THING_THING_H__
#include "thing/thing.h"
#endif

/*****************************************************************************/

class	CProjectile : public CEnemyProjectileThing
{
public:
	enum PROJECTILE_MOVEMENT_TYPE
	{
		PROJECTILE_FIXED = 0,
		PROJECTILE_DUMBFIRE = 1,
		PROJECTILE_USER_SEEK,
		PROJECTILE_GAS_CLOUD,
		PROJECTILE_MINE,
	};

	enum PROJECTILE_LIFETIME_TYPE
	{
		PROJECTILE_FINITE_LIFE = 0,
		PROJECTILE_INFINITE_LIFE = 1,
	};

	enum PROJECTILE_STATE
	{
		PROJECTILE_ATTACK = 0,
		PROJECTILE_RETURN = 1,
	};
	enum
	{ // For Dynamic ThingCache
		MAX_SUBTYPE	=1,
	};

	static CProjectile	*Create();
	void				init();
	void				init( DVECTOR initPos, s16 initHeading );
	void				init( DVECTOR initPos, s16 initHeading, PROJECTILE_MOVEMENT_TYPE initMoveType, PROJECTILE_LIFETIME_TYPE initLifeType );
	void				init( DVECTOR initPos, s16 initHeading, PROJECTILE_MOVEMENT_TYPE initMoveType, PROJECTILE_LIFETIME_TYPE initLifeType, s32 initLifetime );
	void				shutdown();
	void				think(int _frames);
	bool				alwaysThink()											{return(true);}
	virtual void		render();
	void				processEvent( GAME_EVENT evt, CThing *sourceThing );
	void				setMovementType( PROJECTILE_MOVEMENT_TYPE moveType );
	PROJECTILE_MOVEMENT_TYPE	getMovementType();
	void				setState( PROJECTILE_STATE newState );
	void				setLifeTime( PROJECTILE_LIFETIME_TYPE lifeType );
	void				setPosition( DVECTOR const &newPos )					{Pos = newPos;}
	void				setHeading( s16 newHeading )							{m_heading = newHeading;}
	void				setOt( u8 newOt )										{m_ot=newOt;}
	void				setGraphic( int frame );
	void				setSpeed( u16 newSpeed )								{m_speed = newSpeed;}
	void				setXScale( s16 newScale )								{m_xScale = newScale;}
	void				setYScale( s16 newScale )								{m_yScale = newScale;}
	void				setShock()												{m_shock = true;}

protected:
	bool				processTargetSeek( int _frames, DVECTOR targetPos );
	void				collidedWith( CThing *_thisThing );

	DVECTOR										m_initPos;
	s16											m_heading;
	s16											m_initHeading;
	s32											m_lifetime;
	s32											m_extension;
	PROJECTILE_MOVEMENT_TYPE					m_movementType;
	PROJECTILE_LIFETIME_TYPE					m_lifetimeType;
	PROJECTILE_STATE							m_state;
	u16											m_turnSpeed;
	u8											m_ot;
	int											m_spriteFrame;
	u16											m_speed;
	s16											m_xScale;
	s16											m_yScale;
	u8											m_shock;
	s32											m_highResX, m_highResY;
};

/*****************************************************************************/

class	CPlayerProjectile : public CPlayerProjectileThing
{
public:
	enum PLAYER_PROJECTILE_MOVEMENT_TYPE
	{
		PLAYER_PROJECTILE_DUMBFIRE = 0,
		PLAYER_PROJECTILE_BOUNCING = 1,
		PLAYER_PROJECTILE_DUMBFIRE_CORAL,
	};

	enum PLAYER_PROJECTILE_LIFETIME_TYPE
	{
		PLAYER_PROJECTILE_FINITE_LIFE = 0,
		PLAYER_PROJECTILE_INFINITE_LIFE = 1,
	};
	enum
	{ // For Dynamic ThingCache
		MAX_SUBTYPE	=2,
	};

	static CPlayerProjectile	*Create();
	void				init();
	void				init( DVECTOR initPos, s16 initHeading );
	void				init( DVECTOR initPos, s16 initHeading, PLAYER_PROJECTILE_MOVEMENT_TYPE initMoveType, PLAYER_PROJECTILE_LIFETIME_TYPE initLifeType );
	void				init( DVECTOR initPos, s16 initHeading, PLAYER_PROJECTILE_MOVEMENT_TYPE initMoveType, PLAYER_PROJECTILE_LIFETIME_TYPE initLifeType, s32 initLifetime );
	void				shutdown();
	virtual void		think(int _frames);
	virtual void		render();
	void				processEvent( GAME_EVENT evt, CThing *sourceThing );

	void				setMovementType( PLAYER_PROJECTILE_MOVEMENT_TYPE moveType );
	PLAYER_PROJECTILE_MOVEMENT_TYPE		getMovementType();
	void				setLifeTime( PLAYER_PROJECTILE_LIFETIME_TYPE lifeType );
	void				setPosition( DVECTOR const &newPos )	{Pos = newPos;}
	void				setRGB( u32 new_RGB );


protected:
	void				collidedWith( CThing *_thisThing );

	DVECTOR										m_initPos;
	s16											m_heading;
	s32											m_lifetime;
	s32											m_extension;
	PLAYER_PROJECTILE_MOVEMENT_TYPE				m_movementType;
	PLAYER_PROJECTILE_LIFETIME_TYPE				m_lifetimeType;
	u16											m_turnSpeed;
	int											m_frame;
	POLY_FT4									*SprFrame;
	u8											m_reversed;
	CVECTOR										m_RGB;
	s16											m_vertVelocity;
	u8											m_bounceCount;
	u8											m_hitTarget;
	s32											m_highResX, m_highResY;
};


/*****************************************************************************/
#endif
