/*=========================================================================

	nshell.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __ENEMY_NSHELL_H__
#define __ENEMY_NSHELL_H__

#ifndef __ENEMY_NPC_H__
#include "enemy\npc.h"
#endif

class CNpcShellEnemy : public CNpcEnemy
{
public:
	void				postInit();
	void				render();
	int					getFrameCount()							{return( 1 );}
	int					getFrame()								{return( m_shellType );}
	u8					hasBeenAttacked()						{return( false );}
	virtual u8			canCollideWithEnemy()					{return( false );}
protected:
	virtual void		collidedWith(CThing *_thisThing);

	u8					m_shellType;
};

#endif
