/*=========================================================================

	ndustdev.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef	__ENEMY_NDUSTDEV_H__
#define	__ENEMY_NDUSTDEV_H__

class CNpcDustDevilEnemy : public CNpcEnemy
{
public:
	void				postInit();
	void				render();
	u8					canCollideWithEnemy()									{return( false );}
protected:
	s32					getFrameShift( int _frames );
	void				processMovement( int _frames );
	void				processMovementModifier( int _frames, s32 distX, s32 distY, s32 dist, s16 headingChange );

	s16					m_fadeVal;
	u8					m_fadeOut;
};

#endif