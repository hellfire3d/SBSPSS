/*=========================================================================

	ndustdev.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef	__ENEMY_NDUSTDEV_H__
#define	__ENEMY_NDUSTDEV_H__

class CNpcDustDevilEnemy : public CNpcEnemy
{
public:
	virtual void		postInit();
protected:
	virtual void		processMovement( int _frames );
};

#endif