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

#ifndef __ENEMY_NBOSS_H__
#include "enemy\nboss.h"
#endif

#ifndef __PLAYER_PLAYER_H__
#include "player\player.h"
#endif


class CNpcSubSharkEnemy : public CNpcBossEnemy
{
public:
	void				postInit();
	void				render();
	virtual void		renderOnMapScreen( DVECTOR drawPos );
	void				shutdown();
protected:
	//virtual void		processClose( int _frames );
	s32					getFrameShift( int _frames );
	void				processMovement( int _frames );
	void				processShot( int _frames );
	void				collidedWith(CThing *_thisThing);
	void				setupWaypoints( sThingActor *ThisActor );

	enum NPC_SUB_SHARK_STATE
	{
		SUB_SHARK_MINE_1 = 0,
		SUB_SHARK_GOTO_CHARGE = 1,
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
	DVECTOR				m_targetPos;
	u8					m_carryPlayer;
	PLAYER_MODE			m_oldPlayerMode;
};

#endif