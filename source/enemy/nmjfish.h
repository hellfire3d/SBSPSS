/*=========================================================================

	nmjfish.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef	__ENEMY_NMJFISH_H__
#define	__ENEMY_NMJFISH_H__

#include "fx\fx.h"
#include "fx\fxjfish.h"
#include "fx\fxnrgbar.h"

class CNpcMotherJellyfishEnemy : public CNpcEnemy
{
public:
	virtual void		postInit();
	void				render();
	virtual void		shutdown();
	virtual void		setupWaypoints( sThingActor *ThisActor );
protected:
	virtual void		processClose( int _frames );
	virtual void		processMovement( int _frames );
	virtual void		processShot( int _frames );
	void				spawnJellyfish( int _frames );
	//virtual void		processUserCollision( CThing *thisThing );

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

	int					m_jellyfishCount;
	s32					m_spawnTimer;
	s32					m_cycleWidth;
	s32					m_halfCycleWidth;
	bool				m_meterOn;

	CFXJellyFishLegs	*legs[4];
	DVECTOR				legsPos[4];
};

#endif