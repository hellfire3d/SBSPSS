/*=========================================================================

	nboss.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__ENEMY_NBOSS_H__
#define	__ENEMY_NBOSS_H__

#ifndef __ENEMY_NPC_H__
#include "enemy\npc.h"
#endif

#include "fx\fx.h"
#include "fx\fxnrgbar.h"

class CNpcBossEnemy : public CNpcEnemy
{
public:
	virtual void		postInit();
	virtual void		shutdown();
	void				think( int _frames );
	virtual void		renderOnMapScreen( DVECTOR drawPos )		{;}
	bool				allowOffMap()								{return (true);}

protected:
	void				addHealthMeter();
	virtual void		processShot( int _frames );

	bool				m_meterOn;
	CFXNRGBar			*m_energyBar;
	s32					m_invulnerableTimer;
};

#endif