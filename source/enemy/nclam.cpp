/*=========================================================================

	nclam.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef __ENEMY_NPC_H__
#include "enemy\npc.h"
#endif

#ifndef	__ENEMY_NCLAM_H__
#include "enemy\nclam.h"
#endif

#ifndef __PLATFORM_PLATFORM_H__
#include "platform\platform.h"
#endif

#ifndef __PLATFORM_PCLAM_H__
#include "platform\pclam.h"
#endif

#ifndef __GAME_GAME_H__
#include	"game\game.h"
#endif

#ifndef	__PLAYER_PLAYER_H__
#include	"player\player.h"
#endif

#ifndef	__UTILS_HEADER__
#include	"utils\utils.h"
#endif

#ifndef	__ANIM_CLAM_HEADER__
#include <ACTOR_CLAM_ANIM.h>
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcClamEnemy::processEnemyCollision( CThing *thisThing )
{
	// do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcClamEnemy::processUserCollision( CThing *thisThing )
{
	// do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool CNpcClamEnemy::processSensor()
{
	switch( m_sensorFunc )
	{
		case NPC_SENSOR_NONE:
			return( false );

		default:
			{
				if ( playerXDistSqr + playerYDistSqr < 40000 )
				{
					m_controlFunc = NPC_CONTROL_CLOSE;
					m_extendDir = EXTEND_UP;
					m_extension = 0;

					if ( m_soundId == NOT_PLAYING )
					{
						m_soundId = (int) CSoundMediator::playSfx( CSoundMediator::SFX_CLAM_MOVE, true );
					}

					return( true );
				}
				else
				{
					return( false );
				}
			}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcJumpingClamEnemy::postInit()
{
	CNpcClamEnemy::postInit();

	m_drawRotation = ( m_heading + 1024 ) & 4095;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcJumpingClamEnemy::setupWaypoints( sThingActor *ThisActor )
{
	u16	*PntList=(u16*)MakePtr(ThisActor,sizeof(sThingActor));

	u16 newXPos, newYPos;
	u16 startXPos, startYPos, endXPos, endYPos;

	m_npcPath.setWaypointCount( ThisActor->PointCount - 1 );

	startXPos = newXPos = (u16) *PntList;
	setWaypointPtr( PntList );
	PntList++;
	startYPos = newYPos = (u16) *PntList;
	PntList++;

	setStartPos( newXPos, newYPos );

	m_maxExtension = 10;

	if ( ThisActor->PointCount > 1 )
	{
		newXPos = (u16) *PntList;
		PntList++;
		newYPos = (u16) *PntList;
		PntList++;

		endXPos = newXPos;
		endYPos = newYPos;

		setHeading( newXPos, newYPos, startXPos, startYPos );

		s32 xDist = ( endXPos - startXPos ) << 4;
		s32 yDist = ( endYPos - startYPos ) << 4;

		m_maxExtension = isqrt2( ( xDist * xDist ) + ( yDist * yDist ) );
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

void CNpcJumpingClamEnemy::processClose( int _frames )
{
	s32 velocity;

	if ( m_extendDir == EXTEND_UP )
	{
		velocity = m_speed * _frames;

		m_extension += velocity;

		if ( m_extension > m_maxExtension )
		{
			m_extension = m_maxExtension;
			m_extendDir = EXTEND_DOWN;
		}

		Pos = m_base;
		Pos.vx += ( m_extension * rcos( m_heading ) ) >> 12;
		Pos.vy += ( m_extension * rsin( m_heading ) ) >> 12;

		if ( !m_animPlaying )
		{
			m_animPlaying = true;
			m_animNo = ANIM_CLAM_SNAPUP;
			m_frame = 0;
		}
	}
	else if ( m_extendDir == EXTEND_DOWN )
	{
		velocity = -_frames;

		m_extension += velocity;

		if ( m_extension < 0 )
		{
			m_extension = 0;

			if ( !m_animPlaying )
			{
				m_controlFunc = NPC_CONTROL_MOVEMENT;
				m_timerFunc = NPC_TIMER_ATTACK_DONE;
				m_timerTimer = GameState::getOneSecondInFrames();
				m_sensorFunc = NPC_SENSOR_NONE;
			}
		}

		Pos = m_base;
		Pos.vx += ( m_extension * rcos( m_heading ) ) >> 12;
		Pos.vy += ( m_extension * rsin( m_heading ) ) >> 12;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcStaticClamEnemy::postInit()
{
	CNpcClamEnemy::postInit();

	m_isStunned = 0;

	// create platform in same place

	CNpcPlatform *platform;
	platform = CNpcPlatform::Create( CNpcPlatform::NPC_CLAM_PLATFORM );

	platform->setThingSubType(CNpcPlatform::NPC_CLAM_PLATFORM);
	platform->setGraphic( (u8) 0 );
	platform->init( Pos );
	platform->setTiltable( false );
	platform->postInit();

	CNpcClamPlatform *clamPlatform;
	clamPlatform = (CNpcClamPlatform *) platform;

	clamPlatform->setClam( this );

	m_animPlaying = false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcStaticClamEnemy::stoodOn()
{
	if ( m_frame < ( 5 << 8 ) )
	{
		if ( m_frame != 0 )
		{
			m_frame = 0;

			if ( m_soundId == NOT_PLAYING )
			{
				m_soundId = (int) CSoundMediator::playSfx( CSoundMediator::SFX_CLAM_ATTACK, true );
			}
		}

		m_isStunned = 2 * GameState::getOneSecondInFrames();
		m_animPlaying = false;
	}
	else
	{
		m_oldControlFunc = m_controlFunc;
		m_controlFunc = NPC_CONTROL_COLLISION;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcStaticClamEnemy::think( int _frames )
{
	if ( m_isStunned )
	{
		m_isStunned -= _frames;

		if ( m_isStunned < 0 )
		{
			m_isStunned = 0;

			m_controlFunc = NPC_CONTROL_MOVEMENT;
			m_movementTimer = GameState::getOneSecondInFrames();
		}
	}

	CNpcEnemy::think( _frames );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcStaticClamEnemy::processMovement( int _frames )
{
	if ( !m_animPlaying && m_isStunned == 0 )
	{
		if ( m_movementTimer > 0 )
		{
			m_movementTimer -= _frames;
		}
		else
		{
			m_animPlaying = true;
			m_animNo = ANIM_CLAM_SIDESNAP;

			m_movementTimer = GameState::getOneSecondInFrames();
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcStaticClamEnemy::collidedWith( CThing *_thisThing )
{
	if ( m_isActive && !m_isCaught && !m_isDying )
	{
		switch(_thisThing->getThingType())
		{
			case TYPE_PLAYER:
			{
				CPlayer *player = (CPlayer *) _thisThing;

				if ( m_isStunned <= 0 )
				{
					if ( !player->isRecoveringFromHit() )
					{
						if ( m_frame >= ( 5 << 8 ) )
						{
							if ( m_controlFunc != NPC_CONTROL_COLLISION )
							{
								m_oldControlFunc = m_controlFunc;
								m_controlFunc = NPC_CONTROL_COLLISION;
							}
						}
					}
				}

				break;
			}

			default:
				break;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcStaticClamEnemy::processCollision()
{
	CPlayer *player = GameScene.getPlayer();

	player->takeDamage( m_data[m_type].damageToUserType,REACT__GET_DIRECTION_FROM_THING,(CThing*)this );
	processUserCollision( (CThing *) player );

	m_controlFunc = m_oldControlFunc;

	if ( !m_animPlaying && m_isStunned == 0 )
	{
		m_animPlaying = true;
		m_animNo = ANIM_CLAM_SIDESNAP;
		m_frame = 0;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

s32 CNpcStaticClamEnemy::getFrameShift( int _frames )
{
	return( ( _frames << 8 ) >> 2 );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcStaticClamEnemy::processAnimFrames( int _frames )
{
	if ( m_animPlaying && ( ( m_isDying && m_data[m_type].playDeathAnim ) || !m_isDying ) )
	{
		s32 frameCount;

		frameCount = getFrameCount();

		s32 frameShift = getFrameShift( _frames );

		if ( ( frameCount << 8 ) - m_frame > frameShift )
		{
			m_frame += frameShift;
		}
		else
		{
			m_frame = 0;
			m_animPlaying = false;

			if ( m_soundId == NOT_PLAYING )
			{
				m_soundId = (int) CSoundMediator::playSfx( CSoundMediator::SFX_CLAM_ATTACK, true );
			}
		}
	}
}
