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


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void	CSteamSwitchEmitterTrigger::setPositionAndSize(int _x,int _y,int _w,int _h)
{
	CTrigger::setPositionAndSize( _x, _y, _w, _h );

	m_effect = CFX::Create( CFX::FX_TYPE_STEAM, Pos );
	m_active = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void	CSteamSwitchEmitterTrigger::toggleVisible()
{
	m_effect->toggleVisible();
	m_active = !m_active;
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
