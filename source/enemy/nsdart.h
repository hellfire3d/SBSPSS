/*=========================================================================

	nsdart.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __ENEMY_NSDART_H__
#define __ENEMY_NSDART_H__

#ifndef __SPR_SPRITES_H__
#include <sprites.h>
#endif

class CNpcSquidDartEnemy : public CNpcEnemy
{
public:
	void				render();
	int					getFrameCount()							{return( FRM_SQUIDDART_SWIM0004 - FRM_SQUIDDART_SWIM0001 + 1 );}
	u8					canCollideWithEnemy()					{return( false );}
	void				fireAsProjectile( s16 heading );
protected:
	s32					getFrameShift( int _frames );
	bool				processSensor();
	void				processClose( int _frames );

	s32					m_xPos;
	u8					m_attack;
};

#endif
