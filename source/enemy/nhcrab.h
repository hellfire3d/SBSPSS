/*=========================================================================

	nhcrab.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __ENEMY_NHCRAB_H__

#ifndef __ENEMY_NPC_H__
#include "enemy\npc.h"
#endif

class CNpcHermitCrabEnemy : public CNpcEnemy
{
public:
protected:
	virtual bool		processSensor();
	virtual void		processClose( int _frames );
};

#endif
