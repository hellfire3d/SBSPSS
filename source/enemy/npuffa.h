/*=========================================================================

	npuffa.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__ENEMY_NPUFFA_H__
#define	__ENEMY_NPUFFA_H__

class CNpcPuffaFishEnemy : public CNpcEnemy
{
protected:
	bool				processSensor();
	void				processClose( int _frames );

	enum NPC_PUFFA_FISH_STATE
	{
		PUFFA_FISH_NO_INFLATE = 0,
		PUFFA_FISH_TURN = 1,
		PUFFA_FISH_INFLATE,
	};
};

#endif