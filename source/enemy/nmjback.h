/*=========================================================================

	nmjback.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__ENEMY_NMJBACK_H__
#define	__ENEMY_NMJBACK_H__

#ifndef __ENEMY_NPC_H__
#include "enemy\npc.h"
#endif

class CNpcMotherJellyfishBackground : public CNpcEnemy
{
public:
	void				postInit();
	void				render();
};

#endif