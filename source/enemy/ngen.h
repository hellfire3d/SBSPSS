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
	void				shutdown();
	void				processEnemyCollision( CThing *thisThing );
	void				render();
	void				think(int _frames);
	u8					canCollideWithEnemy()									{return( false );}
	CRECT const			*getThinkBBox()											{return( CThing::getThinkBBox() );}
};

#endif