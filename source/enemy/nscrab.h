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
protected:
	virtual bool		processSensor();
	virtual void		processClose( int _frames );
	virtual void		processCollision();
	virtual void		processMovement( int _frames );
	void				processSpiderCrabInitJumpMovement( int _frames );
};

#endif
