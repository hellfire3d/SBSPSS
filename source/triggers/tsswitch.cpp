/*=========================================================================

	tsswitch.cpp

	Author:		CRB
	Created:	
	Project:	Spongebob
	Purpose:	

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__TRIGGERS_TSSWITCH_H__
#include "triggers\tsswitch.h"
#endif

#ifndef __GAME_GAME_H__
#include	"game\game.h"
#endif

#ifndef __ENEMY_NPC_H__
#include "enemy\npc.h"
#endif

#include "fx\fxsteam.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void	CSteamSwitchEmitterTrigger::setPositionAndSize(int _x,int _y,int _w,int _h)
{
	CTrigger::setPositionAndSize( _x, _y, _w, _h );

	m_effect = CFX::Create( CFX::FX_TYPE_STEAM, Pos );
	m_active = true;

	CFXSteam *steam = (CFXSteam *) m_effect;
	DVECTOR velocity;
	velocity.vx = 0;
	velocity.vy = -1;
	steam->SetVel( velocity );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void	CSteamSwitchEmitterTrigger::toggleVisible()
{
	m_active = !m_active;

	CFXSteam *steam = (CFXSteam *) m_effect;
	DVECTOR velocity;
	velocity.vx = 0;
	velocity.vy = -4 * m_active;
	if ( m_active )
	{
		steam->SetSize( 4 * ONE );
	}
	else
	{
		steam->SetSize( ONE );
	}
	steam->SetVel( velocity );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void	CSteamSwitchEmitterTrigger::collidedWith(CThing *_thisThing)
{
	if ( m_active )
	{
		switch(_thisThing->getThingType())
		{
			case TYPE_ENEMY:
			{
				CNpcEnemy *enemy = (CNpcEnemy *) _thisThing;

				enemy->hasBeenSteamed( Pos );

				break;
			}

			default:
				break;
		}
	}
}
