/*=========================================================================

	nsjfish.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef __ENEMY_NSJFISH_H__
#define __ENEMY_NSJFISH_H__

#ifndef __ENEMY_NPC_H__
#include "enemy\npc.h"
#endif

class CNpcSmallJellyfishEnemy : public CNpcEnemy
{
public:
	void				render();
	virtual int			getFrameCount()							{return( 6 );}
	virtual u8			canCollideWithEnemy()					{return( false );}
protected:
	virtual bool		processSensor();
	virtual void		processClose( int _frames );
	virtual void		processMovementModifier( int _frames, s32 distX, s32 distY, s32 dist, s16 headingChange );

	enum
	{
		NPC_JELLYFISH_RESISTANCE = 64,
	};
};

#endif
