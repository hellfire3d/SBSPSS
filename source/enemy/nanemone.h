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
public:
	virtual void		postInit();
protected:
	virtual void		processEnemyCollision( CThing *thisThing );
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
	virtual void		shutdown();
	virtual void		render();
	virtual void		processShot( int _frames );
protected:
	virtual void		processClose( int _frames );
	virtual void		processMovementModifier( int _frames, s32 distX, s32 distY, s32 dist, s16 headingChange );

	u16					m_scaleX, m_scaleY;

	enum
	{
		SPIKE_RADIUS = 20,
	};
};

class	CNpcAnemone3Enemy : public CNpcAnemoneEnemy
{
protected:
	virtual void		processClose( int _frames );
};

#endif