/*=========================================================================

	nbblob.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__ENEMY_NBBLOB_H__
#define	__ENEMY_NBBLOB_H__

class CNpcBallBlobEnemy : public CNpcEnemy
{
protected:
	virtual void		processMovement( int _frames );
};

#endif