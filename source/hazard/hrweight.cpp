/*=========================================================================

	hrweight.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __HAZARD_HRWEIGHT_H__
#include "hazard\hrweight.h"
#endif

#ifndef	__UTILS_HEADER__
#include	"utils\utils.h"
#endif

#ifndef __VID_HEADER_
#include "system\vid.h"
#endif

#ifndef __GAME_GAME_H__
#include	"game\game.h"
#endif

#ifndef __FRIEND_FRIEND_H__
#include "friend\friend.h"
#endif

#ifndef	__GFX_OTPOS_H__
#include "gfx\otpos.h"
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcRisingWeightHazard::init()
{
	CNpcHazard::init();

	m_triggered = false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcRisingWeightHazard::setWaypoints( sThingHazard *ThisHazard )
{
	ASSERT( ThisHazard->PointCount >= 3 );

	u16	*PntList=(u16*)MakePtr(ThisHazard,sizeof(sThingHazard));

	u16 newXPos, newYPos;

	// get master hazard init pos

	newXPos = (u16) *PntList;
	PntList++;
	newYPos = (u16) *PntList;
	PntList++;

	DVECTOR startPos;
	startPos.vx = ( newXPos << 4 ) + 8;
	startPos.vy = ( newYPos << 4 ) + 16;

	Pos = startPos;
	m_base = Pos;

	// get master hazard max vertical extension

	newXPos = (u16) *PntList;
	PntList++;
	newYPos = (u16) *PntList;
	PntList++;

	m_maxExtension = abs( ( ( newYPos << 4 ) + 16 ) - startPos.vy ) << 8;

	// get slave trigger position

	newXPos = (u16) *PntList;
	PntList++;
	newYPos = (u16) *PntList;
	PntList++;

	m_wheelPos.vx = newXPos;
	m_wheelPos.vy = newYPos;

	if ( ThisHazard->PointCount > 3 )
	{
		newXPos = (u16) *PntList;
		PntList++;
		newYPos = (u16) *PntList;
		PntList++;

		m_pulleyPos.vx = ( newXPos << 4 ) + 8;
		m_pulleyPos.vy = ( newYPos << 4 ) + 16;
	}
	else
	{
		m_pulleyPos.vx = startPos.vx;
		m_pulleyPos.vy = startPos.vy - ( m_maxExtension >> 8 );
	}

	s32 minX, maxX, minY, maxY;

	m_npcPath.getPathXExtents( &minX, &maxX );
	m_npcPath.getPathYExtents( &minY, &maxY );

	m_thinkArea.x1 = minX;
	m_thinkArea.x2 = maxX;
	m_thinkArea.y1 = minY;
	m_thinkArea.y2 = maxY;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcRisingWeightHazard::processMovement( int _frames )
{
	if ( m_triggered )
	{
		m_triggered = false;
		m_extension += ( 16 * _frames ) << 8;
		if ( m_extension > m_maxExtension )
		{
			m_extension = m_maxExtension;
		}
		else
		{
			if ( canRender() && m_soundId == NOT_PLAYING )
			{
				m_soundId = (int) CSoundMediator::playSfx( CSoundMediator::SFX_PULLEY, true, true );
			}
		}
	}
	else
	{
		m_extension -= 64 * _frames;

		if ( m_extension < 0 )
		{
			m_extension = 0;
		}
		else
		{
			if ( canRender() && m_soundId == NOT_PLAYING )
			{
				m_soundId = (int) CSoundMediator::playSfx( CSoundMediator::SFX_PULLEY, true, true );
			}

			m_wheel->weightDrop();
		}
	}

	Pos.vy = m_base.vy - ( m_extension >> 8 );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcRisingWeightHazard::render()
{
	int		x1,y1,x2,y2;
	int		minX, maxX, minY, maxY;

	DVECTOR const &offset = CLevel::getCameraPos();

	if ( m_isActive )
	{
		CHazardThing::render();

		if (canRender())
		{
			DVECTOR &renderPos=getRenderPos();

			m_modelGfx->Render(renderPos);
		}

		x1 = m_base.vx - offset.vx;
		x2 = m_pulleyPos.vx - offset.vx;
		y1 = m_pulleyPos.vy - offset.vy;
		y2 = Pos.vy - offset.vy;

		minX = x1;
		maxX = x2;

		if ( minX > maxX )
		{
			minX = x2;
			maxX = x1;
		}

		minY = y1;
		maxY = y2;

		if ( minY > maxY )
		{
			minY = y2;
			maxY = y1;
		}

		if ( maxX >= 0 && minX <= VidGetScrW() )
		{
			if ( maxY >= 0 && minY <= VidGetScrH() )
			{
				DrawLine( x1, y1, x2, y2, 0, 0, 0, OTPOS__ACTOR_POS+1 );
			}
		}

		x1 = ( m_wheelPos.vx << 4 ) + 8 - offset.vx;
		x2 = m_pulleyPos.vx - offset.vx;
		y1 = ( m_wheelPos.vy << 4 ) + 16 - offset.vy;
		y2 = m_pulleyPos.vy - offset.vy;

		minX = x1;
		maxX = x2;

		if ( minX > maxX )
		{
			minX = x2;
			maxX = x1;
		}

		minY = y1;
		maxY = y2;

		if ( minY > maxY )
		{
			minY = y2;
			maxY = y1;
		}

		if ( maxX >= 0 && minX <= VidGetScrW() )
		{
			if ( maxY >= 0 && minY <= VidGetScrH() )
			{
				DrawLine( x1, y1, x2, y2, 0, 0, 0, OTPOS__ACTOR_POS+1 );
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcRisingWeightHazard::collidedWith( CThing *_thisThing )
{
	if ( m_isActive )
	{
		switch(_thisThing->getThingType())
		{
			case TYPE_PLAYER:
			{
				CPlayer *player = (CPlayer *) _thisThing;

				if ( !player->isRecoveringFromHit() )
				{
					player->takeDamage( DAMAGE__HIT_ENEMY );
				}

				break;
			}

			case TYPE_NPC:
			{
				CNpcFriend *npcFriend = (CNpcFriend *) _thisThing;

				npcFriend->setObstructed();

				break;
			}

			default:
				break;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcRisingWeightWheelHazard::init()
{
	CNpcHazard::init();

	m_rotation = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcRisingWeightWheelHazard::setWaypoints( sThingHazard *ThisHazard )
{
	u16	*PntList=(u16*)MakePtr(ThisHazard,sizeof(sThingHazard));

	u16 newXPos, newYPos;

	// get init pos

	newXPos = (u16) *PntList;
	PntList++;
	newYPos = (u16) *PntList;
	PntList++;

	DVECTOR startPos;
	startPos.vx = ( newXPos << 4 ) + 8;
	startPos.vy = ( newYPos << 4 ) + 16;

	Pos = startPos;
	m_base = Pos;

	m_wheelPos.vx = newXPos;
	m_wheelPos.vy = newYPos;

	s32 minX, maxX, minY, maxY;

	m_npcPath.getPathXExtents( &minX, &maxX );
	m_npcPath.getPathYExtents( &minY, &maxY );

	m_thinkArea.x1 = minX;
	m_thinkArea.x2 = maxX;
	m_thinkArea.y1 = minY;
	m_thinkArea.y2 = maxY;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcRisingWeightWheelHazard::collidedWith( CThing *_thisThing )
{
	if ( m_isActive )
	{
		switch(_thisThing->getThingType())
		{
			case TYPE_PLAYER:
			{
				CPlayer *player = (CPlayer *) _thisThing;

				ATTACK_STATE playerState = player->getAttackState();

				if ( playerState == ATTACK_STATE__BUTT_BOUNCE )
				{
					m_weight->setTriggered();

					m_rotation += 256;
					m_rotation &= 4095;
				}

				break;
			}

			case TYPE_PLAYERPROJECTILE:
			{
				m_weight->setTriggered();

				m_rotation += 256;
				m_rotation &= 4095;

				_thisThing->setToShutdown();
			}

			default:
				break;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcRisingWeightWheelHazard::weightDrop()
{
	m_rotation -= 128;
	m_rotation &= 4095;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcRisingWeightWheelHazard::render()
{
	CHazardThing::render();

	if (canRender())
	{
		DVECTOR &renderPos=getRenderPos();

		SVECTOR rotation;
		rotation.vx = 0;
		rotation.vy = 0;
		rotation.vz = m_rotation;

		VECTOR scale;
		scale.vx = ONE;
		scale.vy = ONE;
		scale.vz = ONE;

		m_modelGfx->Render(renderPos,&rotation,&scale);
	}
}
