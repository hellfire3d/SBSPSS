/*=========================================================================

	hcswitch.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __HAZARD_HCSWITCH_H__
#include "hazard\hcswitch.h"
#endif

#ifndef __TRIGGERS_THAZARD_H__
#include "triggers\thazard.h"
#endif

#ifndef	__UTILS_HEADER__
#include	"utils\utils.h"
#endif

#ifndef __GAME_GAME_H__
#include	"game\game.h"
#endif

#ifndef _ANIMTEX_HEADER_
#include "gfx\animtex.h"
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcConveyorSwitchHazard::init()
{
	CNpcHazard::init();

	m_reversed = false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcConveyorSwitchHazard::render()
{
	if ( m_isActive )
	{
		CHazardThing::render();

		if (canRender())
		{
			DVECTOR &renderPos=getRenderPos();

			VECTOR flip;

			if ( m_reversed )
			{
				flip.vx = ONE;
			}
			else
			{
				flip.vx = -ONE;
			}

			flip.vy = ONE;
			flip.vz = ONE;

			m_modelGfx->RenderNoClip( renderPos, NULL, &flip );
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcConveyorSwitchHazard::setWaypoints( sThingHazard *ThisHazard )
{
	int pointNum;

	u16	*PntList=(u16*)MakePtr(ThisHazard,sizeof(sThingHazard));

	u16 newXPos, newYPos;

	m_npcPath.setWaypointCount( ThisHazard->PointCount - 1 );

	newXPos = (u16) *PntList;
	setWaypointPtr( PntList );
	PntList++;
	newYPos = (u16) *PntList;
	PntList++;

	DVECTOR startPos;
	startPos.vx = ( newXPos << 4 ) + 8;
	startPos.vy = ( newYPos << 4 ) + 16;

	Pos = startPos;
	m_base = Pos;

	CHazardTrigger *trigger;

	trigger=(CHazardTrigger*)CTrigger::Create(CTrigger::TRIGGER_HAZARD);
	trigger->setPositionAndSize( ( newXPos << 4 ) + 8 - 50, ( newYPos << 4 ) + 16, 100, 0 );
	trigger->setHazard( this );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcConveyorSwitchHazard::trigger()
{
	m_reversed = !m_reversed;

	CLevel &level = GameScene.GetLevel();
	level.reverseMapConveyor();

	CAnimTex::SetSpeed( -CAnimTex::GetSpeed() );

	CSoundMediator::playSfx(CSoundMediator::SFX_ANEMONE_DEFEATED_LEVEL2);
}