/*=========================================================================

	neyeball.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef	__ENEMY_NEYEBALL_H__
#define	__ENEMY_NEYEBALL_H__

class CNpcEyeballEnemy : public CNpcEnemy
{
	virtual void		postInit();
protected:
	virtual bool		processSensor();
	virtual void		processClose( int _frames );
};

#endif