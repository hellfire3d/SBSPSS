/*=========================================================================

	nhcrab.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __ENEMY_NHCRAB_H__
#define __ENEMY_NHCRAB_H__

#ifndef __ENEMY_NPC_H__
#include "enemy\npc.h"
#endif

class CNpcHermitCrabEnemy : public CNpcEnemy
{
public:
	void				postInit();
protected:
	void				processMovementModifier( int _frames, s32 distX, s32 distY, s32 dist, s16 headingChange );
	bool				processSensor();
	void				processClose( int _frames );

	s32					m_jumpBase;

	enum NPC_HERMIT_CRAB_STATE
	{
		HERMIT_CRAB_NO_ATTACK = 0,
		HERMIT_CRAB_ROLL_ATTACK_JUMP1 = 1,
		HERMIT_CRAB_ROLL_ATTACK_JUMP2,
		HERMIT_CRAB_ROLL_ATTACK_ROLL,
	};
};

#endif
