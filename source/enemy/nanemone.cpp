/*=========================================================================

	nanemone.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef __ENEMY_NPC_H__
#include "enemy\npc.h"
#endif

#ifndef __PROJECTL_PROJECTL_H__
#include "projectl\projectl.h"
#endif

#ifndef __GAME_GAME_H__
#include "game\game.h"
#endif

#ifndef	__PLAYER_PLAYER_H__
#include "player\player.h"
#endif

#ifndef	__ANIM_ANENOMELVL1_HEADER__
#include <ACTOR_ANENOMELVL1_ANIM.h>
#endif

#ifndef __ANIM_SPIKEYANENOME_HEADER__
#include <ACTOR_SPIKEYANENOME_ANIM.h>
#endif

#ifndef	__ANIM_ANENOMELVL3_HEADER__
#include <ACTOR_ANENOMELVL3_ANIM.h>
#endif


void CNpcEnemy::processCloseAnemone1Attack( int _frames )
{
	s32 moveX, moveY;
	s16 decDir, incDir, moveDist;
	s16 maxTurnRate = m_data[m_type].turnSpeed;
	bool withinRange = false;

	s16 headingToPlayer = ratan2( playerYDist, playerXDist );

	decDir = m_fireHeading - headingToPlayer;

	if ( decDir < 0 )
	{
		decDir += ONE;
	}

	incDir = headingToPlayer - m_fireHeading;

	if ( incDir < 0 )
	{
		incDir += ONE;
	}

	if ( decDir < incDir )
	{
		moveDist = decDir;
	}
	else
	{
		moveDist = incDir;
	}

	// check user is within 45 degrees either way

	if ( moveDist < 512 )
	{
		decDir = m_heading - headingToPlayer;

		if ( decDir < 0 )
		{
			decDir += ONE;
		}

		incDir = headingToPlayer - m_heading;

		if ( incDir < 0 )
		{
			incDir += ONE;
		}

		if ( decDir < incDir )
		{
			moveDist = decDir;
		}
		else
		{
			moveDist = incDir;
		}

		if ( moveDist < -maxTurnRate )
		{
			moveDist = -maxTurnRate;
		}
		else if ( moveDist > maxTurnRate )
		{
			moveDist = maxTurnRate;
		}
		else
		{
			withinRange = true;
		}

		m_heading += moveDist;

		m_heading = m_heading % ONE;

		if ( withinRange )
		{
			// can fire

			if ( m_timerTimer <= 0 && !m_animPlaying )
			{
				if ( m_animNo != ANIM_ANENOMELVL1_FIRE )
				{
					m_animPlaying = true;
					m_animNo = ANIM_ANENOMELVL1_FIRE;
					m_frame = 0;
				}
				else
				{
					CProjectile *projectile;
					projectile = new( "test projectile" ) CProjectile;
					projectile->init( Pos, m_heading );

					m_controlFunc = NPC_CONTROL_MOVEMENT;
					m_timerTimer = GameState::getOneSecondInFrames();

					m_animPlaying = true;
					m_animNo = ANIM_ANENOMELVL1_BEND;
					m_frame = 0;
				}
			}
		}
	}
}

void CNpcEnemy::processCloseAnemone2Attack( int _frames )
{
	int fireLoop;
	CProjectile *projectile;
	s16 heading;

	if ( m_animNo != ANIM_SPIKEYANENOME_BODY )
	{
		m_animPlaying = true;
		m_animNo = ANIM_SPIKEYANENOME_BODY;
		m_frame = 0;
	}
	else
	{
		CProjectile *projectile;
		s16 heading;

		// fire off attached spikes

		CThing *nextThing = Next;

		while ( nextThing )
		{
			CProjectile *projectile;

			projectile = (CProjectile *) nextThing;

			if ( projectile->getMovementType() == CProjectile::PROJECTILE_FIXED )
			{
				projectile->setMovementType( CProjectile::PROJECTILE_DUMBFIRE );
				projectile->setLifeTime( CProjectile::PROJECTILE_FINITE_LIFE );
				projectile->setState( CProjectile::PROJECTILE_ATTACK );
			}

			nextThing = nextThing->getNext();
		}

		// attach new spikes

		for ( fireLoop = 0 ; fireLoop < 5 ; fireLoop++ )
		{
			DVECTOR spikePos;

			heading = m_heading - 1024 + ( fireLoop * 512 );
			heading %= 4096;

			spikePos = Pos;
			spikePos.vx += ( 10 * rcos( heading ) ) >> 12;
			spikePos.vy += ( 10 * rsin( heading ) ) >> 12;

			projectile = new( "anemone lev2 projectile" ) CProjectile;
			projectile->init( spikePos, heading, CProjectile::PROJECTILE_FIXED, CProjectile::PROJECTILE_INFINITE_LIFE );

			addChild( projectile );
		}

		m_controlFunc = NPC_CONTROL_MOVEMENT;
		m_timerFunc = NPC_TIMER_ATTACK_DONE;
		m_timerTimer = GameState::getOneSecondInFrames();
		m_sensorFunc = NPC_SENSOR_NONE;
	}
}

void CNpcEnemy::processCloseAnemone3Attack( int _frames )
{
	if ( m_animNo != ANIM_ANENOMELVL3_FIRE )
	{
		m_animPlaying = true;
		m_animNo = ANIM_ANENOMELVL3_FIRE;
		m_frame = 0;
	}
	else if ( !m_animPlaying )
	{
		CProjectile *projectile;
		u8 lifetime = 8;

		projectile = new( "test projectile" ) CProjectile;
		projectile->init(	Pos,
							m_fireHeading,
							CProjectile::PROJECTILE_GAS_CLOUD,
							CProjectile::PROJECTILE_FINITE_LIFE,
							lifetime * GameState::getOneSecondInFrames() );

		m_controlFunc = NPC_CONTROL_MOVEMENT;
		m_timerFunc = NPC_TIMER_ATTACK_DONE;
		m_timerTimer = ( lifetime + 4 ) * GameState::getOneSecondInFrames();
		m_sensorFunc = NPC_SENSOR_NONE;

		m_animPlaying = true;
		m_animNo = ANIM_ANENOMELVL3_BEND;
		m_frame = 0;
	}
}