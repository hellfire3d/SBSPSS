/*=========================================================================

	nbooger.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef	__ENEMY_NBOOGER_H__
#define	__ENEMY_NBOOGER_H__

class CNpcBoogerMonsterEnemy : public CNpcEnemy
{
protected:
	bool				processSensor();
	void				processClose( int _frames );

	enum
	{
		NPC_BOOGER_MONSTER_MAX_EXTENSION = 20,
	};
};

#endif