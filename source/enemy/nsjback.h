/*=========================================================================

	nsjback.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef __ENEMY_NSJBACK_H__
#define __ENEMY_NSJBACK_H__

#ifndef __ENEMY_NSJFISH_H__
#include "enemy\nsjfish.h"
#endif

class CNpcSmallJellyfishBackgroundEnemy : public CNpcSmallJellyfishEnemy
{
public:
	virtual void		render();
	virtual void		postInit();
	void				shutdown();
	void				setTargetHeading( s16 newTargetHeading )			{m_targetHeading = newTargetHeading;}
	u8					hasBeenAttacked();
	bool				allowOffMap()										{return(true);}

protected:
	bool				processSensor();
	void				processMovement( int _frames );
	void				processUserCollision( CThing *thisThing );
	virtual void		collidedWith(CThing *_thisThing);

	s16					m_targetHeading;
};

#endif
