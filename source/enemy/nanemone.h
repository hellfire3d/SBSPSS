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
	void				setHeading( s32 xPos, s32 yPos, s32 xOrig, s32 yOrig );
protected:
	void				processEnemyCollision( CThing *thisThing );
	bool				processSensor();

	s32					m_fireHeading;
};

class	CNpcAnemone1Enemy : public CNpcAnemoneEnemy
{
protected:
	void				processClose( int _frames );
};

class	CNpcAnemone2Enemy : public CNpcAnemoneEnemy
{
public:
	void				postInit();
	void				shutdown();
	void				render();
protected:
	void				processClose( int _frames );
	void				processMovementModifier( int _frames, s32 distX, s32 distY, s32 dist, s16 headingChange );
	void				processShotDeathStart( int _frames );
	void				processShotDeathEnd( int _frames );

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
	void				processClose( int _frames );
};

#endif