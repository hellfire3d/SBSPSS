/*=========================================================================

	nsshark.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef	__ENEMY_NSSHARK_H__
#define	__ENEMY_NSSHARK_H__

class CNpcSubSharkEnemy : public CNpcEnemy
{
public:
	virtual void		postInit();
protected:
	virtual void		processClose( int _frames );
	virtual void		processMovement( int _frames );

	enum NPC_SUB_SHARK_STATE
	{
		SUB_SHARK_MINE_1 = 0,
		SUB_SHARK_MINE_2 = 1,
		SUB_SHARK_CYCLE,
		SUB_SHARK_SWALLOW,
	};

	enum
	{
		NPC_SUB_SHARK_HIGH_SPEED = 6,
	};
};

#endif