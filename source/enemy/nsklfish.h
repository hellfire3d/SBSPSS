/*=========================================================================

	nsklfish.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef	__ENEMY_NSKLFISH_H__
#define	__ENEMY_NSKLFISH_H__

class CNpcSkeletalFishEnemy : public CNpcEnemy
{
protected:
	bool				processSensor();
	void				processClose( int _frames );
	s32					getFrameShift( int _frames );

	s32					m_chargeTime;

	enum
	{
		SKELETAL_FISH_CHARGE_VELOCITY = 6,
	};
};

#endif