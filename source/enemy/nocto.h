/*=========================================================================

	nocto.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef	__ENEMY_NOCTO_H__
#define	__ENEMY_NOCTO_H__

class CNpcBabyOctopusEnemy : public CNpcEnemy
{
protected:
	bool				processSensor();
	void				processClose( int _frames );
	void				processMovementModifier( int _frames, s32 distX, s32 distY, s32 dist, s16 headingChange );

	enum
	{
		NPC_OCTOPUS_RESISTANCE = 64,
	};
};

#endif