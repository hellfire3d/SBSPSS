/*=========================================================================

	nscrab.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef __ENEMY_NPC_H__
#include "enemy\npc.h"
#endif

#ifndef __GAME_GAME_H__
#include	"game\game.h"
#endif

#ifndef	__PLAYER_PLAYER_H__
#include	"player\player.h"
#endif

#ifndef __ANIM_SPIDERCRAB_HEADER__
#include <ACTOR_SPIDERCRAB_ANIM.h>
#endif


void CNpcEnemy::processCloseSpiderCrabAttack( int _frames )
{
	s32 velocity;
	DVECTOR newPos = Pos;

	if ( m_animNo != ANIM_SPIDERCRAB_JUMP )
	{
		m_animPlaying = true;
		m_animNo = ANIM_SPIDERCRAB_JUMP;
		m_frame = 0;
	}

	velocity = m_velocity * _frames;

	if ( m_extendDir == EXTEND_RIGHT )
	{
		m_extension += velocity;
		m_heading = 0;
	}
	else
	{
		m_extension -= velocity;
		m_heading = 2048;
	}

	bool completed = false;

	if ( m_extension > 128 )
	{
		m_extension = 128;
		completed = true;
	}
	else if ( m_extension < -128 )
	{
		m_extension = -128;
		completed = true;
	}

	newPos.vx = m_base.vx + m_extension;
	newPos.vy = m_base.vy - ( ( 20 * rsin( abs( m_extension ) << 4 ) ) >> 12 );

	s32 minX, maxX;

	m_npcPath.getPathXExtents( &minX, &maxX );

	if ( newPos.vx < minX )
	{
		newPos.vx = minX;
	}
	else if ( newPos.vx > maxX )
	{
		newPos.vx = maxX;
	}

	// check for collision with ground

	if ( m_layerCollision->getHeightFromGround( newPos.vx, newPos.vy ) < 0 )
	{
		// abort jump

		m_controlFunc = NPC_CONTROL_MOVEMENT;
		m_timerFunc = NPC_TIMER_ATTACK_DONE;
		m_timerTimer = GameState::getOneSecondInFrames();
		m_sensorFunc = NPC_SENSOR_NONE;

		m_extension = 0;
		completed = false;
	}
	else
	{
		Pos = newPos;
	}

	if ( completed )
	{
		m_controlFunc = NPC_CONTROL_MOVEMENT;
		m_timerFunc = NPC_TIMER_ATTACK_DONE;
		m_timerTimer = GameState::getOneSecondInFrames();
		m_sensorFunc = NPC_SENSOR_NONE;

		m_extension = 0;
	}
}

void CNpcEnemy::processSpiderCrabCollision()
{
	if ( m_oldControlFunc == NPC_CONTROL_CLOSE )
	{
		// bite player

		if ( m_animNo != ANIM_SPIDERCRAB_BITE )
		{
			CPlayer *player = GameScene.getPlayer();

			player->takeDamage( m_data[m_type].damageToUserType );

			m_animNo = ANIM_SPIDERCRAB_BITE;
			m_animPlaying = true;
			m_frame = 0;
		}
		else if ( !m_animPlaying )
		{
			m_controlFunc = NPC_CONTROL_MOVEMENT;
			m_timerFunc = NPC_TIMER_ATTACK_DONE;
			m_timerTimer = GameState::getOneSecondInFrames();
			m_sensorFunc = NPC_SENSOR_NONE;

			m_extension = 0;
		}
	}
	else
	{
		CPlayer *player = GameScene.getPlayer();

		player->takeDamage( m_data[m_type].damageToUserType );

		m_controlFunc = m_oldControlFunc;
	}
}

void CNpcEnemy::processSpiderCrabInitJumpMovement( int _frames )
{
	s32 velocity;
	bool completed = false;
	bool collision = false;

	if ( m_animNo != ANIM_SPIDERCRAB_JUMP )
	{
		m_animPlaying = true;
		m_animNo = ANIM_SPIDERCRAB_JUMP;
		m_frame = 0;
	}

	velocity = m_velocity * _frames;

	m_extension += velocity;

	if ( m_extension > 128 )
	{
		m_extension = 128;
		completed = true;
	}

	Pos.vy = m_base.vy - ( ( 50 * rsin( abs( m_extension ) << 4 ) ) >> 12 );

	if ( m_extension > 64 )
	{
		// check for collision on the way back down

		if ( m_layerCollision->getHeightFromGround( Pos.vx, Pos.vy ) < 0 )
		{
			collision = true;
		}
	}

	if ( completed || collision )
	{
		m_movementFunc = m_data[m_type].movementFunc;
	}
}

void CNpcEnemy::processSpiderCrabSpawnerMovement( int _frames )
{
	if ( getNumChildren() < 3 )
	{
		m_movementTimer -= _frames;

		if ( m_movementTimer < 0 )
		{
			m_movementTimer = 3 * GameState::getOneSecondInFrames();

			CNpcEnemy *enemy;
			enemy = new( "spider crab" ) CNpcEnemy;
			ASSERT(enemy);
			enemy->setType( CNpcEnemy::NPC_SPIDER_CRAB );
			enemy->init();
			enemy->setLayerCollision( m_layerCollision );
			enemy->setStartPos( Pos.vx >> 4, Pos.vy >> 4 );

			CNpcWaypoint *sourceWaypoint = m_npcPath.getWaypointList();

			if ( sourceWaypoint )
			{
				// skip first waypoint

				sourceWaypoint = sourceWaypoint->nextWaypoint;

				while( sourceWaypoint )
				{
					enemy->addWaypoint( sourceWaypoint->pos.vx >> 4, sourceWaypoint->pos.vy >> 4 );
					sourceWaypoint = sourceWaypoint->nextWaypoint;
				}
			}

			enemy->setPathType( m_npcPath.getPathType() );

			enemy->postInit();

			addChild( enemy );
		}
	}
}