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
	virtual void		setHeading( s32 xPos, s32 yPos );
protected:
	virtual void		processEnemyCollision( CThing *thisThing );
	virtual bool		processSensor();

	s32					m_fireHeading;
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

	u8					m_radius;

	enum
	{
		SPIKE_RADIUS = 15,
		SPIKE_DIST = 5,
	};
};

class	CNpcAnemone3Enemy : public CNpcAnemoneEnemy
{
protected:
	virtual void		processClose( int _frames );
};

#endif