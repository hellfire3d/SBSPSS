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
	virtual void		postInit();
protected:
	virtual bool		processSensor();
	virtual void		processClose( int _frames );
};

#endif