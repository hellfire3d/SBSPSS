/*=========================================================================

	nsklfish.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef	__ENEMY_NSKLFISH_H__
#define	__ENEMY_NSKLFISH_H__

class CNpcSkeletalFishEnemy : public CNpcEnemy
{
protected:
	virtual void		processClose( int _frames );
};

#endif