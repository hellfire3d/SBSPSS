/*=========================================================================

	trigger source file

	Author:		Davey Boy
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __TRIGGER_TRIGGER_HEADER__
#include "triggers\trigger.h"
#endif

#ifndef	__TRIGGERS_TLEVEXIT_H__
#include "triggers\tlevexit.h"
#endif

#ifndef	__TRIGGERS_TRESTART_H__
#include "triggers\trestart.h"
#endif

#ifndef	__TRIGGERS_TTELEPRT_H__
#include "triggers\tteleprt.h"
#endif

#ifndef	__TRIGGERS_TCAMLOCK_H__
#include "triggers\tcamlock.h"
#endif

#ifndef	__TRIGGERS_TWATER_H__
#include "triggers\twater.h"
#endif

#ifndef __GAME_GAME_H__
#include "game\game.h"
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CTrigger	*CTrigger::Create(int Type)
{
CTrigger	*trigger;

	trigger = (CTrigger*)CThingManager::GetThing(CThing::TYPE_TRIGGER,Type);
	if (!trigger)
	switch( Type )
	{
		// Exit trigger
		case TRIGGER_EXIT:
			trigger=(CTrigger*)new ("LevelExitTrigger") CLevelExitTrigger();
			break;

		// Level respawn trigger
		case TRIGGER_RESPAWN:
			trigger=(CRestartPointTrigger*)new ("RestartTrigger") CRestartPointTrigger();
			break;

		// Teleport trigger
		case TRIGGER_TELEPORT:
			trigger=(CTeleportTrigger*)new ("TeleportTrigger") CTeleportTrigger();
			break;

		// Camera lock trigger
		case TRIGGER_CAMLOCK:
			trigger=(CCameraLockTrigger*)new ("CameraLockTrigger") CCameraLockTrigger();
			break;

		// In/Out of water triggers
		case TRIGGER_INWATER:
			trigger=(CInWaterTrigger*)new ("InWaterTrigger") CInWaterTrigger();
			break;
		case TRIGGER_OUTWATER:
			trigger=(COutOfWaterTrigger*)new ("OutOfWaterTrigger") COutOfWaterTrigger();
			break;
		default:
			trigger=NULL;
	}
	ASSERT( trigger);

	trigger->setThingSubType(Type);
	trigger->init();

	return( trigger);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CTrigger	*CTrigger::Create(sThingTrigger *ThisTrigger)
{
CTrigger	*trigger=Create(ThisTrigger->Type);
			trigger->setPositionAndSize(ThisTrigger->Pos.X<<4,ThisTrigger->Pos.Y<<4,ThisTrigger->Width<<4,ThisTrigger->Height<<4);
			trigger->setTargetBox(ThisTrigger->TargetPos.X<<4,ThisTrigger->TargetPos.Y<<4,ThisTrigger->TargetSize.X<<4,ThisTrigger->TargetSize.Y<<4);

		return( trigger);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void	CTrigger::init()
{
	CTriggerThing::init();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void	CTrigger::shutdown()
{
	CTriggerThing::shutdown();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void	CTrigger::think(int _frames)
{
		CTriggerThing::think(_frames);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void	CTrigger::render()
{
		CTriggerThing::render();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void	CTrigger::setPositionAndSize(int _x,int _y,int _w,int _h)
{
	Pos.vx=_x+(_w/2);
	Pos.vy=_y+(_h/2);
	setCollisionSize(_w,_h);
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CTrigger::setTargetBox(int _x,int _y,int _w,int _h)
{
	m_boxX1=_x;
	m_boxY1=_y;
	m_boxX2=_x+_w;
	m_boxY2=_y+_h;
}


