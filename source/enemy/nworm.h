/*=========================================================================

	nworm.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__ENEMY_NWORM_H__
#define	__ENEMY_NWORM_H__

class CNpcParasiticWormEnemy : public CNpcEnemy
{
public:
	virtual void		postInit();
	virtual void		shutdown();
	virtual void		render();
protected:
	virtual bool		processSensor();
	virtual void		processClose( int _frames );
	virtual void		processMovement( int _frames );
	void				resetParasiticWormHeadToTail();
	virtual void		processEnemyCollision( CThing *thisThing );

	enum
	{
		NPC_PARASITIC_WORM_SPACING = 4,
		NPC_PARASITIC_WORM_LENGTH = 10,
	};
};

class CNpcParasiticWormSegment : public CNpcEnemy
{
public:
	virtual void		render();
	virtual void		think( int _frames );
	virtual void		postInit();
	virtual void		processEnemyCollision( CThing *thisThing );
	virtual void		setScale( u16 scale )							{m_scale = scale;}

	u16					m_scale;
};

#endif