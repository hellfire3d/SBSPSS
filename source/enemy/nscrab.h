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
	virtual void		postInit();
	virtual u8			canCollideWithEnemy();
	virtual void		processEnemyCollision( CThing *thisThing );
protected:
	virtual void		processMovementModifier( int _frames, s32 distX, s32 distY, s32 dist, s16 headingChange );
	virtual bool		processSensor();
	virtual void		processClose( int _frames );
	virtual void		processCollision();
	virtual void		processMovement( int _frames );
	void				processSpiderCrabInitJumpMovement( int _frames );

	enum NPC_SPIDER_CRAB_STATE
	{
		SPIDER_CRAB_DEFAULT = 0,
		SPIDER_CRAB_INIT_JUMP = 1,
	};
};

#endif
