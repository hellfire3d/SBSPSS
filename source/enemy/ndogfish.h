/*=========================================================================

	ndogfish.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef	__ENEMY_NDOGFISH_H__
#define	__ENEMY_NDOGFISH_H__

#include "fx\fx.h"
#include "fx\fxlaser.h"

#ifndef __ENEMY_NBOSS_H__
#include "enemy\nboss.h"
#endif


class CNpcIronDogfishEnemy : public CNpcBossEnemy
{
public:
	virtual void		postInit();
	void				render();
	virtual void		shutdown();
protected:
	virtual void		processTimer( int _frames );
	virtual bool		processSensor();
	virtual void		processClose( int _frames );
	virtual void		processMovement( int _frames );
	void				processStandardIronDogfishAttack( int _frames );
	void				processWalkToUser( int _frames, int speed );
	virtual void		processCollision();
	virtual void		processAttackCollision();
	virtual void		hasBeenSteamed( DVECTOR &steamPos );
	virtual void		processShotDeathEnd( int _frames );
	virtual void		collidedWith(CThing *_thisThing);

	enum NPC_IRON_DOGFISH_STATE
	{
		IRON_DOGFISH_THUMP_1 = 0,
		IRON_DOGFISH_LASER_EYE_1 = 1,
		IRON_DOGFISH_LASER_EYE_1_WAIT,
		IRON_DOGFISH_THUMP_2,
		IRON_DOGFISH_ROLL,
		IRON_DOGFISH_LASER_EYE_2,
		IRON_DOGFISH_LASER_EYE_2_WAIT,
	};

	s32					m_steamTimer;
	s32					m_vulnerableTimer;
	s32					m_laserTimer;

	CFXLaser			*m_effect;
};

#endif