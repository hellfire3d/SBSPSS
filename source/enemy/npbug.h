/*=========================================================================

	npbug.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __ENEMY_NPBUG_H__
#define __ENEMY_NPBUG_H__

#ifndef	__ENEMY_NPC_H__
#include "enemy\npc.h"
#endif

class CNpcPricklyBugEnemy : public CNpcEnemy
{
protected:
	void				processMovementModifier( int _frames, s32 distX, s32 distY, s32 dist, s16 headingChange );
};

#endif
