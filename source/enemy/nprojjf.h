/*=========================================================================

	nprojjf.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef __ENEMY_NPROJJF_H__
#define __ENEMY_NPROJJF_H__

#ifndef __ENEMY_NSJFISH_H__
#include "enemy\nsjfish.h"
#endif

#ifndef __SPR_SPRITES_H__
#include <sprites.h>
#endif

class CNpcSmallJellyfishProjectileEnemy : public CNpcSmallJellyfishEnemy
{
public:
	void				postInit();
	void				think(int _frames);
protected:
	s32					m_lifetime;
};

#endif
