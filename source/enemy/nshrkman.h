/*=========================================================================

	nshrkman.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef	__ENEMY_NSHRKMAN_H__
#define	__ENEMY_NSHRKMAN_H__

class CNpcSharkManEnemy : public CNpcEnemy
{
public:
	void				postInit();
protected:
	void				processClose( int _frames );
	void				processMovement( int _frames );

	u8					m_fired;
};

#endif