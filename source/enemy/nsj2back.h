/*=========================================================================

	nsj2back.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef __ENEMY_NSJ2BACK_H__
#define __ENEMY_NSJ2BACK_H__

#ifndef __ENEMY_NSJBACK_H__
#include "enemy\nsjback.h"
#endif

class CNpcSmallJellyfish2BackgroundEnemy : public CNpcSmallJellyfishBackgroundEnemy
{
public:
	void				postInit();
	void				render();
protected:
	void				collidedWith(CThing *_thisThing);
};

#endif
