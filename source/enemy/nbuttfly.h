/*=========================================================================

	nbuttfly.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef __ENEMY_NBUTTFLY_H__
#define __ENEMY_NBUTTFLY_H__

#ifndef __ENEMY_NSJBACK_H__
#include "enemy\nsjback.h"
#endif

class CNpcButterflyBackgroundEnemy : public CNpcSmallJellyfishBackgroundEnemy
{
public:
	virtual void		render();
protected:
	virtual void		processMovementModifier( int _frames, s32 distX, s32 distY, s32 dist, s16 headingChange );
};

#endif
