/*=========================================================================

	nclam.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef	__ENEMY_NCLAM_H__
#define	__ENEMY_NCLAM_H__

class CNpcClamEnemy : public CNpcEnemy
{
protected:
	virtual void		processEnemyCollision( CThing *thisThing );
	virtual bool		processSensor();
};

class CNpcJumpingClamEnemy : public CNpcClamEnemy
{
protected:
	virtual void		processClose( int _frames );
};

class CNpcStaticClamEnemy : public CNpcClamEnemy
{
protected:
	virtual void		processClose( int _frames );
};

#endif