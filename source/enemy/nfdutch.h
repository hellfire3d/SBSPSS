/*=========================================================================

	nfdutch.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef	__ENEMY_NFDUTCH_H__
#define	__ENEMY_NFDUTCH_H__

#ifndef __ENEMY_NBOSS_H__
#include "enemy\nboss.h"
#endif


class CNpcFlyingDutchmanEnemy : public CNpcBossEnemy
{
public:
	virtual void		think( int _frames );
	virtual void		postInit();
	void				render();
	virtual void		shutdown();
	virtual u8			hasBeenAttacked();
protected:
	virtual void		processClose( int _frames );
	virtual void		processMovement( int _frames );
	virtual void		processShotDeathEnd( int _frames );
	virtual void		collidedWith(CThing *_thisThing);

	enum NPC_FLYING_DUTCHMAN_STATE
	{
		FLYING_DUTCHMAN_ATTACK_PLAYER_1 = 0,
		FLYING_DUTCHMAN_ATTACK_PLAYER_2 = 1,
		FLYING_DUTCHMAN_ATTACK_PLAYER_3,
	};

	s32					m_minY, m_maxY;
	u8					m_inRange;
	u8					m_fireCount;
	s32					m_invulnerableTimer;
};

#endif