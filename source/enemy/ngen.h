/*=========================================================================

	ngen.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__ENEMY_NGEN_H__
#define	__ENEMY_NGEN_H__

#ifndef __ENEMY_NPC_H__
#include "enemy\npc.h"
#endif

class	CNpcEnemyGenerator : public CNpcEnemy
{
public:
	virtual void		processEnemyCollision( CThing *thisThing );
	void				render();
	void				think(int _frames);
};

#endif