/*=========================================================================

	psswitch.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PSSWITCH_H__
#include "platform\psswitch.h"
#endif

#ifndef __GAME_GAME_H__
#include	"game\game.h"
#endif

#ifndef	__UTILS_HEADER__
#include	"utils\utils.h"
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcSteamSwitchPlatform::postInit()
{
	CNpcPlatform::postInit();

	m_state = NPC_STEAM_SWITCH_STOP;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcSteamSwitchPlatform::processMovement( int _frames )
{
	switch( m_state )
	{
		case NPC_STEAM_SWITCH_STOP:
		{
			if ( m_contact )
			{
				CPlayer *player = GameScene.getPlayer();
				ATTACK_STATE playerState = player->getAttackState();

				if ( playerState == ATTACK_STATE__BUTT_BOUNCE )
				{
					m_state = NPC_STEAM_SWITCH_DEPRESS;
				}
			}

			break;
		}

		case NPC_STEAM_SWITCH_DEPRESS:
		{
			s32 extension = m_maxExtension - m_extension;
			s32 maxMove = m_speed * _frames;

			if ( extension > maxMove )
			{
				extension = maxMove;
			}
			else if ( extension < -maxMove )
			{
				extension = -maxMove;
			}

			if ( extension )
			{
				m_extension += extension;
			}
			else
			{
				m_state = NPC_STEAM_SWITCH_RETURN;
			}

			break;
		}

		case NPC_STEAM_SWITCH_RETURN:
		{
			s32 extension = -m_extension;
			s32 maxMove = m_speed * _frames;

			if ( extension > maxMove )
			{
				extension = maxMove;
			}
			else if ( extension < -maxMove )
			{
				extension = -maxMove;
			}

			if ( extension )
			{
				m_extension += extension;
			}
			else
			{
				m_state = NPC_STEAM_SWITCH_STOP;
			}

			break;
		}
	}

	Pos.vy = m_base.vy + m_extension;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcSteamSwitchPlatform::setWaypoints( sThingPlatform *ThisPlatform )
{
	int pointNum;

	u16	*PntList=(u16*)MakePtr(ThisPlatform,sizeof(sThingPlatform));

	u16 newXPos, newYPos;

	newXPos = (u16) *PntList;
	PntList++;
	newYPos = (u16) *PntList;
	PntList++;

	DVECTOR startPos;
	startPos.vx = ( newXPos << 4 ) + 8;
	startPos.vy = ( newYPos << 4 ) + 16;

	init( startPos );

	if ( ThisPlatform->PointCount > 1 )
	{
		newXPos = (u16) *PntList;
		PntList++;
		newYPos = (u16) *PntList;
		PntList++;

		m_maxExtension = ( ( newYPos << 4 ) + 16 ) - startPos.vy;
	}
	else
	{
		m_maxExtension = 100;
	}
}
