/*=========================================================================

	hcheck.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __HAZARD_HCHECK_H__
#include "hazard\hcheck.h"
#endif

#ifndef	__PLAYER_PLAYER_H__
#include	"player\player.h"
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcCheckpointHazard::init()
{
	CNpcHazard::init();

	m_triggered = false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcCheckpointHazard::render()
{
	CNpcHazard::render();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcCheckpointHazard::collidedWith(CThing *_thisThing)
{
	if ( m_isActive && !m_triggered )
	{
		switch(_thisThing->getThingType())
		{
			case TYPE_PLAYER:
			{
				CRECT collisionArea=getCollisionArea();
				DVECTOR respawnPos;
				respawnPos.vx=collisionArea.x1+((collisionArea.x2-collisionArea.x1)/2);
				respawnPos.vy=collisionArea.y2;
				((CPlayer*)_thisThing)->setRespawnPosAndRingTelephone(respawnPos);
				m_triggered = true;

				break;
			}

			default:
				break;
		}
	}
}
