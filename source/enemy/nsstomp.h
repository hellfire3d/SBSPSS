/*=========================================================================

	nsstomp.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef	__ENEMY_NSSTOMP_H__
#define	__ENEMY_NSSTOMP_H__

class CNpcSkullStomperEnemy : public CNpcEnemy
{
public:
	void				postInit();
	u8					hasBeenAttacked()										{return(false);}
protected:
	void				processEnemyCollision( CThing *thisThing );
	bool				processSensor();
	void				processClose( int _frames );
	void				collidedWith(CThing *_thisThing);
};

#endif