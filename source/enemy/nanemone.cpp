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

#ifndef	__ENEMY_NANEMONE_H__
#include "enemy\nanemone.h"
#endif

#ifndef __PROJECTL_PROJECTL_H__
#include "projectl\projectl.h"
#endif

#ifndef __GAME_GAME_H__
#include "game\game.h"
#endif

#ifndef __VID_HEADER_
#include "system\vid.h"
#endif

#ifndef	__UTILS_HEADER__
#include	"utils\utils.h"
#endif

#ifndef	__PLAYER_PLAYER_H__
#include "player\player.h"
#endif

#ifndef	__ANIM_ANENOME_HEADER__
#include <ACTOR_ANENOME_ANIM.h>
#endif

#ifndef __ANIM_SPIKEYANENOME_HEADER__
#include <ACTOR_SPIKEYANENOME_ANIM.h>
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcAnemoneEnemy::postInit()
{
	CNpcEnemy::postInit();
	m_drawRotation = m_heading + 1024;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcAnemoneEnemy::processEnemyCollision( CThing *thisThing )
{
	// do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool CNpcAnemoneEnemy::processSensor()
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

void CNpcAnemone1Enemy::processClose( int _frames )
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
			moveDist = -decDir;
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

		m_heading &= 4095;

		m_drawRotation = m_heading + 1024;

		if ( withinRange )
		{
			// can fire, start firing anim

			if ( m_timerTimer <= 0 && !m_animPlaying )
			{
				if ( m_animNo != ANIM_ANENOME_FIRE )
				{
					m_animPlaying = true;
					m_animNo = ANIM_ANENOME_FIRE;
					m_frame = 0;
				}
			}
		}
	}
	else
	{
		if ( !m_animPlaying || m_animNo != ANIM_ANENOME_FIRE )
		{
			m_animPlaying = true;
			m_animNo = ANIM_ANENOME_FIRE;
			m_frame = 0;
		}
	}

	if ( withinRange )
	{
		if ( m_timerTimer <= 0 && !m_animPlaying )
		{
			if ( m_animNo == ANIM_ANENOME_FIRE )
			{
				// if firing anim is complete and user is still in range, fire projectile

				CProjectile *projectile;
				projectile = new( "test projectile" ) CProjectile;
				projectile->init( Pos, m_heading );
				projectile->setLayerCollision( m_layerCollision );

				m_controlFunc = NPC_CONTROL_MOVEMENT;
				m_timerTimer = GameState::getOneSecondInFrames();
				m_timerFunc = NPC_TIMER_ATTACK_DONE;
				m_sensorFunc = NPC_SENSOR_NONE;

				m_animPlaying = true;
				m_animNo = ANIM_ANENOME_FIRE;
				m_frame = 0;
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcAnemone2Enemy::postInit()
{
	CProjectile *projectile;
	s16 heading;

	MATRIX mtx;
	SetIdentNoTrans(&mtx );
	RotMatrixZ( m_heading, &mtx );

	for ( int fireLoop = 0 ; fireLoop < 5 ; fireLoop++ )
	{
		DVECTOR spikePos;

		s16 relativeHeading = -1024 + ( fireLoop * 512 );

		heading = m_heading + relativeHeading;
		heading &= 4095;

		spikePos = Pos;

		//s16 multiplier, multiplier2;
		s16 xDiff, yDiff;
		SVECTOR offset;
		VECTOR result;

		// move base position
		
		spikePos.vx += ( 10 * rcos( m_heading ) ) >> 12;
		spikePos.vy += ( 10 * rsin( m_heading ) ) >> 12;

		// move appropriate to scaling (anemone origin is 90 degrees off, hence:)

		xDiff = ( m_scaleY * SPIKE_RADIUS ) >> 12;
		yDiff = ( m_scaleX * SPIKE_RADIUS ) >> 12;

		offset.vx = ( xDiff * rcos( relativeHeading ) ) >> 12;
		offset.vy = ( yDiff * rsin( relativeHeading ) ) >> 12;

		ApplyMatrix( &mtx, &offset, &result );

		spikePos.vx += result.vx;
		spikePos.vy += result.vy;

		projectile = new( "anemone lev2 projectile" ) CProjectile;
		projectile->init( spikePos, heading, CProjectile::PROJECTILE_FIXED, CProjectile::PROJECTILE_INFINITE_LIFE );
		projectile->setLayerCollision( m_layerCollision );

		addChild( projectile );
	}

	m_drawRotation = m_heading + 1024;

	m_scaleX = ONE;
	m_scaleY = ONE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcAnemone2Enemy::shutdown()
{
	CNpcEnemy::shutdown();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcAnemone2Enemy::processShot( int _frames )
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
			switch ( m_state )
			{
				case NPC_GENERIC_HIT_CHECK_HEALTH:
				{
					if ( CLevel::getCurrentChapter() == 1 && CLevel::getCurrentChapterLevel() == 1 )
					{
						m_state = NPC_GENERIC_HIT_DEATH_START;
					}
					else
					{
						m_health -= 5;

						if ( m_health < 0 )
						{
							m_state = NPC_GENERIC_HIT_DEATH_START;
						}
						else
						{
							m_state = NPC_GENERIC_HIT_RECOIL;

							m_animPlaying = true;
							m_animNo = m_data[m_type].recoilAnim;
							m_frame = 0;
						}
					}

					break;
				}

				case NPC_GENERIC_HIT_RECOIL:
				{
					if ( !m_animPlaying )
					{
						m_state = 0;
						m_controlFunc = NPC_CONTROL_MOVEMENT;
					}

					break;
				}

				case NPC_GENERIC_HIT_DEATH_START:
				{
					m_animPlaying = true;
					m_animNo = m_data[m_type].dieAnim;
					m_frame = 0;
					m_state = NPC_GENERIC_HIT_DEATH_END;

					deleteAllChild();
					m_isDying = true;
					m_speed = -5;

					if (m_data[m_type].skelType)
					{
						m_actorGfx->SetOtPos( 0 );
					}

					break;
				}

				case NPC_GENERIC_HIT_DEATH_END:
				{
					m_drawRotation += 64 * _frames;
					m_drawRotation &= 4095;

					Pos.vy += m_speed * _frames;

					if ( m_speed < 5 )
					{
						m_speed++;
					}

					DVECTOR	offset = CLevel::getCameraPos();

					if ( Pos.vy - offset.vy > VidGetScrH() )
					{
						if ( m_data[m_type].respawning )
						{
							m_isActive = false;

							m_timerFunc = NPC_TIMER_RESPAWN;
							m_timerTimer = 4 * GameState::getOneSecondInFrames();
						}
						else
						{
							setToShutdown();
						}
					}

					break;
				}
			}

			break;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcAnemone2Enemy::processClose( int _frames )
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

		removeAllChild();

		// attach new spikes

		MATRIX mtx;
		SetIdentNoTrans(&mtx );
		RotMatrixZ( m_heading, &mtx );

		for ( fireLoop = 0 ; fireLoop < 5 ; fireLoop++ )
		{
			DVECTOR spikePos;

			s16 relativeHeading = -1024 + ( fireLoop * 512 );

			heading = m_heading + relativeHeading;
			heading &= 4095;

			spikePos = Pos;

			//s16 multiplier, multiplier2;
			s16 xDiff, yDiff;
			SVECTOR offset;
			VECTOR result;

			// move base position
			
			spikePos.vx += ( 10 * rcos( m_heading ) ) >> 12;
			spikePos.vy += ( 10 * rsin( m_heading ) ) >> 12;

			// move appropriate to scaling (anemone origin is 90 degrees off, hence:)

			xDiff = ( m_scaleY * SPIKE_RADIUS ) >> 12;
			yDiff = ( m_scaleX * SPIKE_RADIUS ) >> 12;

			offset.vx = ( xDiff * rcos( relativeHeading ) ) >> 12;
			offset.vy = ( yDiff * rsin( relativeHeading ) ) >> 12;

			ApplyMatrix( &mtx, &offset, &result );

			spikePos.vx += result.vx;
			spikePos.vy += result.vy;

			projectile = new( "anemone lev2 projectile" ) CProjectile;
			projectile->init( spikePos, heading, CProjectile::PROJECTILE_FIXED, CProjectile::PROJECTILE_INFINITE_LIFE );
			projectile->setLayerCollision( m_layerCollision );

			addChild( projectile );
		}

		m_controlFunc = NPC_CONTROL_MOVEMENT;
		m_timerFunc = NPC_TIMER_ATTACK_DONE;
		m_timerTimer = GameState::getOneSecondInFrames();
		m_sensorFunc = NPC_SENSOR_NONE;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcAnemone2Enemy::processMovementModifier( int _frames, s32 distX, s32 distY, s32 dist, s16 headingChange )
{
	s32 maxTimer = GameState::getOneSecondInFrames() >> 1;

	m_movementTimer += _frames;

	if ( m_movementTimer > maxTimer )
	{
		m_movementTimer -= maxTimer;
	}

	s32 sineVal = ( m_movementTimer * ONE ) / maxTimer;

	m_scaleX = ONE + ( rsin( sineVal ) >> 2 );
	m_scaleY = ONE + ( rcos( sineVal ) >> 2 );

	CProjectile *projectile;
	projectile = (CProjectile *) Next;

	MATRIX mtx;
	SetIdentNoTrans(&mtx );
	RotMatrixZ( m_heading, &mtx );

	for ( int fireLoop = 0 ; fireLoop < 5 ; fireLoop++ )
	{
		DVECTOR spikePos;

		s16 relativeHeading = -1024 + ( fireLoop * 512 );
		s16 heading = m_heading + relativeHeading;
		heading &= 4095;

		spikePos = Pos;

		//s16 multiplier, multiplier2;
		s16 xDiff, yDiff;
		SVECTOR offset;
		VECTOR result;

		// move base position
		
		spikePos.vx += ( 10 * rcos( m_heading ) ) >> 12;
		spikePos.vy += ( 10 * rsin( m_heading ) ) >> 12;

		// move appropriate to scaling (anemone origin is 90 degrees off, hence:)

		xDiff = ( m_scaleY * SPIKE_RADIUS ) >> 12;
		yDiff = ( m_scaleX * SPIKE_RADIUS ) >> 12;

		offset.vx = ( xDiff * rcos( relativeHeading ) ) >> 12;
		offset.vy = ( yDiff * rsin( relativeHeading ) ) >> 12;

		ApplyMatrix( &mtx, &offset, &result );

		spikePos.vx += result.vx;
		spikePos.vy += result.vy;

		if ( projectile )
		{
			projectile->setPos( spikePos );
			projectile = (CProjectile *) projectile->getNext();
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcAnemone2Enemy::render()
{
	SprFrame = NULL;

	if ( m_isActive )
	{
		CEnemyThing::render();

		// Render
		DVECTOR renderPos;
		DVECTOR	offset = CLevel::getCameraPos();

		renderPos.vx = Pos.vx - offset.vx;
		renderPos.vy = Pos.vy - offset.vy;

		CRECT collisionRect = getCollisionArea();
		collisionRect.x1 -= Pos.vx;
		collisionRect.x2 -= Pos.vx;
		collisionRect.y1 -= Pos.vy;
		collisionRect.y2 -= Pos.vy;

		if ( renderPos.vx + collisionRect.x2 >= 0 && renderPos.vx + collisionRect.x1 <= VidGetScrW() )
		{
			if ( renderPos.vy + collisionRect.y2 >= 0 && renderPos.vy + collisionRect.y1 <= VidGetScrH() )
			{
				SprFrame = m_actorGfx->Render(renderPos,m_animNo,( m_frame >> 8 ),m_reversed);
				m_actorGfx->RotateScale( SprFrame, renderPos, m_drawRotation, m_scaleX, m_scaleY );

				sBBox boundingBox = m_actorGfx->GetBBox();
				setCollisionSize( ( boundingBox.XMax - boundingBox.XMin ), ( boundingBox.YMax - boundingBox.YMin ) );
				setCollisionCentreOffset( ( boundingBox.XMax + boundingBox.XMin ) >> 1, ( boundingBox.YMax + boundingBox.YMin ) >> 1 );
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcAnemone3Enemy::processClose( int _frames )
{
	if ( m_animNo != ANIM_ANENOME_FIRE )
	{
		m_animPlaying = true;
		m_animNo = ANIM_ANENOME_FIRE;
		m_frame = 0;
	}
	else if ( !m_animPlaying )
	{
		CProjectile *projectile;
		u8 lifetime = 8;

		m_fireHeading = m_heading & 4095;

		projectile = new( "test projectile" ) CProjectile;
		projectile->init(	Pos,
							m_fireHeading,
							CProjectile::PROJECTILE_GAS_CLOUD,
							CProjectile::PROJECTILE_FINITE_LIFE,
							lifetime * GameState::getOneSecondInFrames() );
		projectile->setLayerCollision( m_layerCollision );

		m_controlFunc = NPC_CONTROL_MOVEMENT;
		m_timerFunc = NPC_TIMER_ATTACK_DONE;
		m_timerTimer = ( lifetime + 4 ) * GameState::getOneSecondInFrames();
		m_sensorFunc = NPC_SENSOR_NONE;

		m_animPlaying = true;
		m_animNo = ANIM_ANENOME_FIRE;
		m_frame = 0;
	}
}