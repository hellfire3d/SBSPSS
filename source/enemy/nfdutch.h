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
	void				think( int _frames );
	void				postInit();
	void				render();
	virtual void		renderOnMapScreen( DVECTOR drawPos );
	void				shutdown();
	u8					hasBeenAttacked();
protected:
	void				processClose( int _frames );
	void				processMovement( int _frames );
	void				processShotRecoil( int _frames );
	void				processShotDeathEnd( int _frames );
	void				collidedWith(CThing *_thisThing);

	enum NPC_FLYING_DUTCHMAN_STATE
	{
		//FLYING_DUTCHMAN_ATTACK_PLAYER_1 = 0,
		//FLYING_DUTCHMAN_ATTACK_PLAYER_2 = 1,
		//FLYING_DUTCHMAN_ATTACK_PLAYER_3,
		FLYING_DUTCHMAN_GOTO_PLAYER = 0,
		FLYING_DUTCHMAN_ATTACK_PLAYER = 1,
		FLYING_DUTCHMAN_CHARGE_PLAYER_START,
		FLYING_DUTCHMAN_CHARGE_PLAYER,
		FLYING_DUTCHMAN_LEVEL_SHAKE,
		FLYING_DUTCHMAN_LEVEL_SHAKE_WAIT,
		FLYING_DUTCHMAN_RETURN,
	};

	s32					m_minY, m_maxY;
	u8					m_inRange;
	u8					m_fireCount;
	u8					m_oldState;
	s16					m_fadeVal;
	u8					m_fadeDown;
};

#endif