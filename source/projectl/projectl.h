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

#include "Gfx/Skel.h"

/*****************************************************************************/

class	CProjectile : public CEnemyProjectileThing
{
public:
	enum PROJECTILE_MOVEMENT_TYPE
	{
		PROJECTILE_DUMBFIRE = 0,
		PROJECTILE_USER_SEEK = 1,
		PROJECTILE_GAS_CLOUD,
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

	void				init();
	void				init( DVECTOR initPos, s16 initHeading );
	void				init( DVECTOR initPos, s16 initHeading, PROJECTILE_MOVEMENT_TYPE initMoveType, PROJECTILE_LIFETIME_TYPE initLifeType );
	void				init( DVECTOR initPos, s16 initHeading, PROJECTILE_MOVEMENT_TYPE initMoveType, PROJECTILE_LIFETIME_TYPE initLifeType, s32 initLifetime );
	void				shutdown();
	void				think(int _frames);
	virtual void		render();
	void				processEvent( GAME_EVENT evt, CThing *sourceThing );

protected:
	DVECTOR				getScreenOffset();
	bool				processTargetSeek( int _frames, DVECTOR targetPos );

	class SpriteBank							*m_spriteBank;
	DVECTOR										m_initPos;
	s16											m_heading;
	s32											m_lifetime;
	s32											m_extension;
	PROJECTILE_MOVEMENT_TYPE					m_movementType;
	PROJECTILE_LIFETIME_TYPE					m_lifetimeType;
	PROJECTILE_STATE							m_state;
	u16											m_turnSpeed;
};


/*****************************************************************************/
#endif
