/*=========================================================================

	nanemone.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef	__ENEMY_NANEMONE_H__
#define	__ENEMY_NANEMONE_H__

#ifndef __ENEMY_NPC_H__
#include "enemy\npc.h"
#endif

class	CNpcAnemoneEnemy : public CNpcEnemy
{
protected:
	virtual bool		processSensor();
};

class	CNpcAnemone1Enemy : public CNpcAnemoneEnemy
{
protected:
	virtual void		processClose( int _frames );
};

class	CNpcAnemone2Enemy : public CNpcAnemoneEnemy
{
public:
	virtual void		postInit();
protected:
	virtual void		processClose( int _frames );
};

class	CNpcAnemone3Enemy : public CNpcAnemoneEnemy
{
protected:
	virtual void		processClose( int _frames );
};

#endif