/*=========================================================================

	pretract.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PRETRACT_H__
#include "platform\pretract.h"
#endif

#ifndef __GAME_GAME_H__
#include	"game\game.h"
#endif

#ifndef	__UTILS_HEADER__
#include	"utils\utils.h"
#endif

#ifndef __VID_HEADER_
#include "system\vid.h"
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcRetractingPlatform::postInit()
{
	CNpcPlatform::postInit();

	m_timer = NPC_PLATFORM_TIMER_RETRACT;

	m_extension = ONE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcRetractingPlatform::processTimer( int _frames )
{
	switch( m_timerType )
	{
		case NPC_PLATFORM_TIMER_RETRACT:
		{
			if ( m_timer > 0 )
			{
				m_timer -= _frames;
			}
			else
			{
				if ( m_extension > 1 )
				{
					m_extension -= 128 * _frames;

					if ( m_extension <= 1 )
					{
						m_extension = 1;

						m_timer = 4 * GameState::getOneSecondInFrames();
						m_timerType = NPC_PLATFORM_TIMER_EXTEND;
						m_detectCollision = false;
					}
				}
			}

			break;
		}

		case NPC_PLATFORM_TIMER_EXTEND:
		{
			if ( m_timer > 0 )
			{
				m_timer -= _frames;
			}
			else
			{
				if ( m_extension < ONE )
				{
					m_extension += 128 * _frames;

					if ( m_extension >= ONE )
					{
						m_extension = ONE;

						m_timer = 4 * GameState::getOneSecondInFrames();
						m_timerType = NPC_PLATFORM_TIMER_RETRACT;
						m_detectCollision = true;
					}
				}
			}

			break;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcRetractingPlatform::render()
{
	if ( m_isActive )
	{
		CPlatformThing::render();

		// Render
		CPlatformThing::render();

		if (canRender())
		{
			DVECTOR &renderPos=getRenderPos();
			SVECTOR rotation;

			rotation.vx = 0;
			rotation.vy = 0;
			rotation.vz = 0;

			VECTOR scale;
			scale.vx = ONE;
			scale.vy = ONE;
			scale.vz = m_extension;

			m_modelGfx->Render(renderPos,&rotation,&scale);

#if defined (__USER_paul__) || defined (__USER_charles__)
	DVECTOR size;
	DVECTOR	centre;
	int		halfLength;
	int		x1,y1,x2,y2;

	centre=getCollisionCentre();
	size=getCollisionSize();
	halfLength=size.vx>>1;

	x1=-halfLength*mcos(getCollisionAngle()&4095)>>12;
	y1=-halfLength*msin(getCollisionAngle()&4095)>>12;
	x2=+halfLength*mcos(getCollisionAngle()&4095)>>12;
	y2=+halfLength*msin(getCollisionAngle()&4095)>>12;

	centre.vx-=offset.vx;
	centre.vy-=offset.vy;
	x1+=centre.vx;
	y1+=centre.vy;
	x2+=centre.vx;
	y2+=centre.vy;

	DrawLine(x1,y1,x2,y2,0,255,0,0);
#endif
		}
	}
}
