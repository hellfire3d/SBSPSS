/*=========================================================================

	nfskull.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef	__ENEMY_NFSKULL_H__
#define	__ENEMY_NFSKULL_H__

class CNpcFlamingSkullEnemy : public CNpcEnemy
{
	void				postInit();
protected:
	void				processEnemyCollision( CThing *thisThing );
	bool				processSensor();
	void				processClose( int _frames );

	s32					m_fireHeading;

	enum NPC_FLAMING_SKULL_STATE
	{
		FLAMING_SKULL_ATTACK = 0,
		FLAMING_SKULL_RETURN = 1,
	};
};

#endif