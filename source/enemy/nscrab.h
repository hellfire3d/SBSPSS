/*=========================================================================

	nscrab.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef __ENEMY_NSCRAB_H__
#define __ENEMY_NSCRAB_H__

#ifndef __ENEMY_NPC_H__
#include "enemy\npc.h"
#endif

class CNpcSpiderCrabEnemy : public CNpcEnemy
{
public:
	void				postInit();
	u8					canCollideWithEnemy();
	void				processEnemyCollision( CThing *thisThing );
protected:
	void				processMovementModifier( int _frames, s32 distX, s32 distY, s32 dist, s16 headingChange );
	bool				processSensor();
	void				processClose( int _frames );
	void				processCollision();
	void				processMovement( int _frames );
	void				processSpiderCrabInitJumpMovement( int _frames );

	s32					m_attackDist;
	int					m_jumpDelay;
	int					m_initDelay;

	enum NPC_SPIDER_CRAB_STATE
	{
		SPIDER_CRAB_DEFAULT = 0,
		SPIDER_CRAB_INIT_JUMP = 1,
	};

	enum NPC_SPIDER_CRAB_CONSTANTS
	{
		SPIDER_CRAB_EXTENSION = 64,
		SPIDER_CRAB_HEIGHT = 40,
	};
};

#endif
