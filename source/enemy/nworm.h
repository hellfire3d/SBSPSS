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
protected:
	virtual bool		processSensor();
	virtual void		processClose( int _frames );
	virtual void		processMovement( int _frames );
	void				resetParasiticWormHeadToTail();

	enum
	{
		NPC_PARASITIC_WORM_SPACING = 6,
		NPC_PARASITIC_WORM_LENGTH = 10,
	};
};

class CNpcParasiticWormSegment : public CNpcParasiticWormEnemy
{
public:
	virtual void		postInit();
};

#endif