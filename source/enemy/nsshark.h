/*=========================================================================

	nsshark.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef	__ENEMY_NSSHARK_H__
#define	__ENEMY_NSSHARK_H__

#include "fx\fx.h"
#include "fx\fxnrgbar.h"


class CNpcSubSharkEnemy : public CNpcEnemy
{
public:
	virtual void		think( int _frames );
	virtual void		postInit();
	void				render();
	virtual void		shutdown();
protected:
	//virtual void		processClose( int _frames );
	virtual s32			getFrameShift( int _frames );
	virtual void		processMovement( int _frames );
	virtual void		processShot( int _frames );
	virtual void		collidedWith(CThing *_thisThing);
	virtual void		setupWaypoints( sThingActor *ThisActor );

	enum NPC_SUB_SHARK_STATE
	{
		SUB_SHARK_MINE_1 = 0,
		SUB_SHARK_MINE_2 = 1,
		SUB_SHARK_GOTO_CHARGE,
		SUB_SHARK_DROP,
		SUB_SHARK_START_CHARGE,
		SUB_SHARK_CHARGE,
		SUB_SHARK_END_CHARGE,
	};

	enum
	{
		NPC_SUB_SHARK_HIGH_SPEED = 6,
	};

	u8					m_salvoCount;
	bool				m_meterOn;
	s32					m_invulnerableTimer;
	DVECTOR				m_targetPos;
	CFXNRGBar			*m_energyBar;
};

#endif