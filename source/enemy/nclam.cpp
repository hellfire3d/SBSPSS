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

#ifndef __PLATFORM_PCLAM_H__
#include "platform\pclam.h"
#endif

#ifndef __PLATFORM_PLATFORM_H__
#include "platform\platform.h"
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


void CNpcClamEnemy::processEnemyCollision( CThing *thisThing )
{
	// do nothing
}

void CNpcClamEnemy::processUserCollision( CThing *thisThing )
{
	// do nothing
}

bool CNpcClamEnemy::processSensor()
{
	switch( m_sensorFunc )
	{
		case NPC_SENSOR_NONE:
			return( false );

		default:
			{
				if ( playerXDistSqr + playerYDistSqr < 10000 )
				{
					m_controlFunc = NPC_CONTROL_CLOSE;
					m_extendDir = EXTEND_UP;
					m_extension = 0;
					m_movementTimer = GameState::getOneSecondInFrames() >> 3;
					m_velocity = ( getRnd() % 6 ) + 1;

					return( true );
				}
				else
				{
					return( false );
				}
			}
	}
}

void CNpcJumpingClamEnemy::postInit()
{
	CNpcClamEnemy::postInit();

	m_drawRotation = m_heading + 1024;
}

void CNpcJumpingClamEnemy::processClose( int _frames )
{
	s32 velocity;

	if ( m_extendDir == EXTEND_UP )
	{
		m_movementTimer -= _frames;

		if ( m_movementTimer > 0 )
		{
			// extend

			velocity = m_velocity * _frames;

			m_extension += velocity;

			Pos.vx += ( velocity * rcos( m_heading ) ) >> 12;
			Pos.vy += ( velocity * rsin( m_heading ) ) >> 12;

			if ( !m_animPlaying )
			{
				m_animPlaying = true;
				m_animNo = ANIM_CLAM_SNAPUP;
				m_frame = 0;
			}
		}
		else
		{
			m_extendDir = EXTEND_DOWN;
		}
	}
	else if ( m_extendDir == EXTEND_DOWN )
	{
		// retract

		if ( m_extension > 0 )
		{
			velocity = -_frames;

			if ( m_extension < _frames )
			{
				velocity = m_extension - _frames;
			}
			
			m_extension += velocity;


			Pos.vx += ( velocity * rcos( m_heading ) ) >> 12;
			Pos.vy += ( velocity * rsin( m_heading ) ) >> 12;
		}
		else
		{
			if ( !m_animPlaying )
			{
				m_controlFunc = NPC_CONTROL_MOVEMENT;
				m_timerFunc = NPC_TIMER_ATTACK_DONE;
				m_timerTimer = GameState::getOneSecondInFrames();
				m_sensorFunc = NPC_SENSOR_NONE;
			}
		}
	}
}

void CNpcStaticClamEnemy::postInit()
{
	CNpcClamEnemy::postInit();

	m_isStunned = false;
	m_isAnimating = false;

	// create platform in same place

	CNpcClamPlatform *platform = new ("clam platform") CNpcClamPlatform;

	platform->setType( CNpcClamPlatform::NPC_CLAM_PLATFORM );
	platform->setGraphic( (u8) 0 );
	platform->init( Pos );
	platform->setTiltable( false );
	platform->setBBox();
}

void CNpcStaticClamEnemy::processClose( int _frames )
{
	if ( !m_isAnimating && !m_isStunned )
	{
		m_animPlaying = true;
		m_animNo = ANIM_CLAM_SIDESNAP;
		m_frame = 0;
		m_isAnimating = true;
	}
	else if ( !m_animPlaying )
	{
		m_controlFunc = NPC_CONTROL_MOVEMENT;
		m_animNo = m_data[m_type].initAnim;
		m_frame = 0;
		m_isAnimating = false;
	}

	m_isStunned = false;
}

/*void CNpcStaticClamEnemy::processShot( int _frames )
{
	if ( !m_isStunned )
	{
		switch( m_data[m_type].shotFunc )
		{
			case NPC_SHOT_NONE:
			{
				// do nothing

				break;
			}

			case NPC_SHOT_GENERIC:
			{
				m_isStunned = true;

				break;
			}
		}
	}

	m_controlFunc = NPC_CONTROL_MOVEMENT;
}*/

void CNpcStaticClamEnemy::collidedWith( CThing *_thisThing )
{
	if ( m_isActive && !m_isCaught && !m_isDying )
	{
		switch(_thisThing->getThingType())
		{
			case TYPE_PLAYER:
			{
				m_isStunned = true;

				break;
			}

			default:
				break;
		}
	}
}

/*int CNpcStaticClamEnemy::checkCollisionAgainst( CThing *_thisThing, int _frames )
{
	DVECTOR	pos,thisThingPos;
	int		radius;
	int		collided;

	pos = getCollisionCentre();
	thisThingPos=_thisThing->getCollisionCentre();

	radius=getCollisionRadius()+_thisThing->getCollisionRadius();
	collided=false;
	if(abs(pos.vx-thisThingPos.vx)<radius&&
	   abs(pos.vy-thisThingPos.vy)<radius)
	{
		CRECT	thisRect,thatRect;

		thisRect=getCollisionArea();

		thatRect=_thisThing->getCollisionArea();

		if(((thisRect.x1>=thatRect.x1&&thisRect.x1<=thatRect.x2)||(thisRect.x2>=thatRect.x1&&thisRect.x2<=thatRect.x2)||(thisRect.x1<=thatRect.x1&&thisRect.x2>=thatRect.x2))&&
		   ((thisRect.y1>=thatRect.y1&&thisRect.y1<=thatRect.y2)||(thisRect.y2>=thatRect.y1&&thisRect.y2<=thatRect.y2)||(thisRect.y1<=thatRect.y1&&thisRect.y2>=thatRect.y2)))
		{
			switch(_thisThing->getThingType())
			{
				case TYPE_PLAYER:
				{
					if ( m_isStunned && m_isActive )
					{
						CPlayer *player = (CPlayer *) _thisThing;

						s32 playerDeltaY = player->getPosDelta().vy;

						if ( thisThingPos.vy - playerDeltaY <= getNewYPos( _thisThing ) )
						{
							player->setPlatform( this );
						}
					}

					break;
				}

				default:
					break;
			}

			collided=true;
		}
	}

	return collided;
}*/