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

#ifndef __SPR_SPRITES_H__
#include <sprites.h>
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcAnemoneEnemy::postInit()
{
	CNpcEnemy::postInit();
	m_drawRotation = ( m_heading + 1024 ) & 4095;
	m_fireHeading = m_heading;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcAnemoneEnemy::setHeading( s32 xPos, s32 yPos, s32 xOrig, s32 yOrig )
{
	m_heading = ( ratan2( yPos - yOrig, xPos - xOrig ) ) & 4095;

	int newHeading = ( m_heading - 1024 ) & 4095;

	Pos.vy -= 8;

	int offset = 8; // initial y offset

	DVECTOR adjust;
	adjust.vx = ( offset * rsin( newHeading ) ) >> 12;
	adjust.vy = -( offset * rcos( newHeading ) ) >> 12;

	Pos.vx += adjust.vx;
	Pos.vy += adjust.vy;
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
			if ( playerXDistSqr + playerYDistSqr < 50000 )
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

	s16 headingToPlayer = ratan2( playerYDist, playerXDist ) & 4095;

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

	if ( moveDist )
	{
		m_heading += moveDist;

		m_heading &= 4095;

		decDir = m_fireHeading - m_heading;

		if ( decDir < 0 )
		{
			decDir += ONE;
		}

		incDir = m_heading - m_fireHeading;

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

		// check user is within 45 degrees either way

		if ( moveDist > 512 )
		{
			m_heading = m_fireHeading + 512;
			m_heading &= 4095;
		}
		else if ( moveDist < -512 )
		{
			m_heading = m_fireHeading - 512;
			m_heading &= 4095;
		}
		else
		{
			if ( m_soundId == NOT_PLAYING )
			{
				m_soundId = (int) CSoundMediator::playSfx( CSoundMediator::SFX_ANEMONE_MOVE, true );
			}
		}

		m_drawRotation = ( m_heading + 1024 ) & 4095;
	}

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

	if ( withinRange )
	{
		if ( m_timerTimer <= 0 && !m_animPlaying )
		{
			if ( m_animNo == ANIM_ANENOME_FIRE )
			{
				// if firing anim is complete and user is still in range, fire projectile

				if( m_soundId != NOT_PLAYING )
				{
					CSoundMediator::stopAndUnlockSfx( (xmPlayingId) m_soundId );
				}

				m_soundId = (int) CSoundMediator::playSfx( CSoundMediator::SFX_ANEMONE_ATTACK_LEVEL1, true );

				DVECTOR projPos;
				projPos = Pos;

				projPos.vx += rcos( m_heading ) >> 9;
				projPos.vy += rsin( m_heading ) >> 9;

				CProjectile *projectile;
				projectile = CProjectile::Create();
				projectile->init( projPos, m_heading );
				projectile->setSpeed( 2 );
				projectile->setXScale( 6144 );
				projectile->setYScale( 6144 );

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
	m_drawRotation = ( m_heading + 1024 ) & 4095;

	m_scaleX = ONE;
	m_scaleY = ONE;

	m_radius = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcAnemone2Enemy::shutdown()
{
	CNpcEnemy::shutdown();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcAnemone2Enemy::processShotDeathStart( int _frames )
{
	CNpcEnemy::processShotDeathStart( _frames );

	deleteAllChild();

	if (m_data[m_type].skelType)
	{
		m_actorGfx->SetOtPos( 0 );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcAnemone2Enemy::processShotDeathEnd( int _frames )
{
	m_heading += 64 * _frames;
	m_heading &= 4095;

	CNpcEnemy::processShotDeathEnd( _frames );
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
		if ( m_soundId == NOT_PLAYING )
		{
			m_soundId = (int) CSoundMediator::playSfx( CSoundMediator::SFX_ANEMONE_ATTACK_LEVEL2, true );
		}

		CProjectile *projectile;
		s16 heading;

		// fire off spikes

		m_radius = 0;

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
			
			spikePos.vx += ( SPIKE_DIST * rcos( m_heading ) ) >> 12;
			spikePos.vy += ( SPIKE_DIST * rsin( m_heading ) ) >> 12;

			// move appropriate to scaling (anemone origin is 90 degrees off, hence:)

			xDiff = ( m_scaleY * m_radius ) >> 12;
			yDiff = ( m_scaleX * m_radius ) >> 12;

			offset.vx = ( xDiff * rcos( relativeHeading ) ) >> 12;
			offset.vy = ( yDiff * rsin( relativeHeading ) ) >> 12;

			ApplyMatrix( &mtx, &offset, &result );

			spikePos.vx += result.vx;
			spikePos.vy += result.vy;

			projectile = CProjectile::Create();
			projectile->init( spikePos, heading, CProjectile::PROJECTILE_DUMBFIRE, CProjectile::PROJECTILE_FINITE_LIFE );
			projectile->setState( CProjectile::PROJECTILE_ATTACK );
			projectile->setSpeed( 2 );
			projectile->setXScale( 6144 );
			projectile->setYScale( 6144 );
		}

		m_controlFunc = NPC_CONTROL_MOVEMENT;
		m_timerFunc = NPC_TIMER_ATTACK_DONE;
		m_timerTimer = 4 * GameState::getOneSecondInFrames();
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

	if ( m_radius < SPIKE_RADIUS )
	{
		m_radius++;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcAnemone2Enemy::render()
{
	SprFrame = NULL;

	if ( m_isActive )
	{
		CEnemyThing::render();

		if (canRender())
		{
			s32 maxTimer = GameState::getOneSecondInFrames() >> 1;

			s32 sineVal = ( m_movementTimer * ONE ) / maxTimer;

			m_scaleX = ONE + ( rsin( sineVal ) >> 2 );
			m_scaleY = ONE + ( rcos( sineVal ) >> 2 );

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
				SVECTOR posOffset;
				VECTOR result;

				// move base position
				
				spikePos.vx += ( SPIKE_DIST * rcos( m_heading ) ) >> 12;
				spikePos.vy += ( SPIKE_DIST * rsin( m_heading ) ) >> 12;

				// move appropriate to scaling (anemone origin is 90 degrees off, hence:)

				xDiff = ( m_scaleY * m_radius ) >> 12;
				yDiff = ( m_scaleX * m_radius ) >> 12;

				posOffset.vx = ( xDiff * rcos( relativeHeading ) ) >> 12;
				posOffset.vy = ( yDiff * rsin( relativeHeading ) ) >> 12;

				ApplyMatrix( &mtx, &posOffset, &result );

				spikePos.vx += result.vx;
				spikePos.vy += result.vy;

				sFrameHdr	*frameHdr;
				
				int		x,y;
				int		scrnWidth = VidGetScrW();
				int		scrnHeight = VidGetScrH();
				int		spriteWidth = CGameScene::getSpriteBank()->getFrameWidth(FRM__SPIKE);
				int		spriteHeight = CGameScene::getSpriteBank()->getFrameHeight(FRM__SPIKE);

				DVECTOR const &offset = CLevel::getCameraPos();

				x = spikePos.vx - offset.vx;
				y = spikePos.vy - offset.vy;

				frameHdr = CGameScene::getSpriteBank()->getFrameHeader( FRM__SPIKE );
				CGameScene::getSpriteBank()->printRotatedScaledSprite( frameHdr, x, y, 4096, 4096, heading, 10 );
			}

			DVECTOR &renderPos=getRenderPos();

			SprFrame = m_actorGfx->Render(renderPos,m_animNo,( m_frame >> 8 ),m_reversed);
			m_actorGfx->RotateScale( SprFrame, renderPos, m_drawRotation, m_scaleX, m_scaleY );

			sBBox boundingBox = m_actorGfx->GetBBox();
			setCollisionSize( ( boundingBox.XMax - boundingBox.XMin ), ( boundingBox.YMax - boundingBox.YMin ) );
			setCollisionCentreOffset( ( boundingBox.XMax + boundingBox.XMin ) >> 1, ( boundingBox.YMax + boundingBox.YMin ) >> 1 );
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
		if ( m_soundId == NOT_PLAYING )
		{
			m_soundId = (int) CSoundMediator::playSfx( CSoundMediator::SFX_ANEMONE_ATTACK_LEVEL3, true );
		}

		CProjectile *projectile;
		u8 lifetime = 8;

		m_fireHeading = m_heading & 4095;

		DVECTOR projPos;
		projPos = Pos;

		projPos.vx += rcos( m_heading ) >> 9;
		projPos.vy += rsin( m_heading ) >> 9;

		projectile = CProjectile::Create();
		projectile->init(	projPos,
							m_fireHeading,
							CProjectile::PROJECTILE_GAS_CLOUD,
							CProjectile::PROJECTILE_FINITE_LIFE,
							lifetime * GameState::getOneSecondInFrames() );
		projectile->setSpeed( 3 );

		m_controlFunc = NPC_CONTROL_MOVEMENT;
		m_timerFunc = NPC_TIMER_ATTACK_DONE;
		m_timerTimer = ( lifetime + 4 ) * GameState::getOneSecondInFrames();
		m_sensorFunc = NPC_SENSOR_NONE;

		m_animPlaying = true;
		m_animNo = ANIM_ANENOME_FIRE;
		m_frame = 0;
	}
}