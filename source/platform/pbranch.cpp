/*=========================================================================

	pbranch.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PBRANCH_H__
#include "platform\pbranch.h"
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcBranchPlatform::postInit()
{
	m_state = NPC_BRANCH_STOP;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcBranchPlatform::processMovement( int _frames )
{
	s16 tilt;
	s16 maxTilt = 3 * _frames;

	switch( m_state )
	{
		case NPC_BRANCH_STOP:
		{
			if ( m_contact )
			{
				m_state = NPC_BRANCH_DIP;
			}
			else
			{
				if ( getCollisionAngle() )
				{
					// go to zero bend

					tilt = -getCollisionAngle();

					if ( tilt > maxTilt )
					{
						tilt = maxTilt;
					}
					else if ( tilt < -maxTilt )
					{
						tilt = -maxTilt;
					}

					setCollisionAngle( getCollisionAngle() + tilt );
				}
			}

			break;
		}

		case NPC_BRANCH_DIP:
		{
			if ( m_contact )
			{
				// bend

				tilt = 3 * _frames;

				s16 newAngle = getCollisionAngle() + tilt;

				if ( newAngle > 256 )
				{
					// if bent beyond certain limit, spring

					m_state = NPC_BRANCH_SPRING;
				}

				setCollisionAngle( newAngle );
			}
			else
			{
				m_state = NPC_BRANCH_STOP;
			}

			break;
		}

		case NPC_BRANCH_SPRING:
		{
			if ( m_contact )
			{
				// spring off player
			}

			m_state = NPC_BRANCH_STOP;

			break;
		}
	}
}
