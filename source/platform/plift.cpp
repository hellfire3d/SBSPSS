/*=========================================================================

	plift.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PLIFT_H__
#include "platform\plift.h"
#endif

#ifndef	__UTILS_HEADER__
#include	"utils\utils.h"
#endif

#ifndef __GAME_GAME_H__
#include	"game\game.h"
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcLiftPlatform::postInit()
{
	CNpcPlatform::postInit();

	m_isActivated = false;
	m_returning = false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcLiftPlatform::setWaypoints( sThingPlatform *ThisPlatform )
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

	newXPos = (u16) *PntList;
	PntList++;
	newYPos = (u16) *PntList;
	PntList++;

	m_maxExtension = ( ( newYPos << 4 ) + 16 ) - startPos.vy;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcLiftPlatform::processMovement( int _frames )
{
	if ( m_contact )
	{
		if ( !m_isActivated )
		{
			m_isActivated = true;
			m_timer = GameState::getOneSecondInFrames();
		}
	}

	if ( m_isActivated )
	{
		if ( m_timer <= 0 )
		{
			s32 extension;

			if ( m_returning )
			{
				extension = -m_extension;
			}
			else
			{
				extension = m_maxExtension - m_extension;
			}

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
				if ( m_returning )
				{
					m_isActivated = false;
					m_returning = false;
				}
				else
				{
					m_timer = GameState::getOneSecondInFrames();
					m_returning = true;
				}
			}
		}

		Pos.vy = m_base.vy + m_extension;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcLiftPlatform::processTimer( int _frames )
{
	if ( m_timer > 0 )
	{
		m_timer -= _frames;
	}
}
