/*=========================================================================

	nffolk.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef	__ENEMY_NFFOLK_H__
#define	__ENEMY_NFFOLK_H__

class CNpcFishFolk : public CNpcEnemy
{
public:
	void				postInit();
protected:
	void				processMovementModifier( int _frames, s32 distX, s32 distY, s32 dist, s16 headingChange );
};

#endif