/*=========================================================================

	neyeball.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef __ENEMY_NPC_H__
#include "enemy\npc.h"
#endif

#ifndef	__ENEMY_NEYEBALL_H__
#include "enemy\neyeball.h"
#endif

#ifndef __GAME_GAME_H__
#include "game\game.h"
#endif

#ifndef	__PLAYER_PLAYER_H__
#include "player\player.h"
#endif

#ifndef __PROJECTL_PROJECTL_H__
#include "projectl\projectl.h"
#endif

#ifndef __SPR_SPRITES_H__
#include <sprites.h>
#endif

#ifndef __VID_HEADER_
#include "system\vid.h"
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcEyeballEnemy::processEnemyCollision( CThing *thisThing )
{
	// do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcEyeballEnemy::postInit()
{
	DVECTOR eyeballPos = Pos;

	m_fireHeading = m_heading;
	m_drawRotation = ( m_heading + 1024 ) & 4095;

	eyeballPos.vx += ( EYEBALL_DIST * rcos( m_heading ) ) >> 12;
	eyeballPos.vy += ( EYEBALL_DIST * rsin( m_heading ) ) >> 12;

	CProjectile *projectile;
	projectile = CProjectile::Create();
	projectile->init( eyeballPos, m_fireHeading, CProjectile::PROJECTILE_FIXED, CProjectile::PROJECTILE_INFINITE_LIFE );
	projectile->setGraphic( FRM_EYEBALL_STATIC );

	addChild( projectile );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcEyeballEnemy::shutdown()
{
	deleteAllChild();
	CNpcEnemy::shutdown();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool CNpcEyeballEnemy::processSensor()
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

void CNpcEyeballEnemy::processClose( int _frames )
{
CThing	*Next=getNext();

	if ( Next )
	{
		CProjectile *projectile;

		projectile = (CProjectile *) Next;

		if ( projectile->getMovementType() == CProjectile::PROJECTILE_FIXED )
		{
			projectile->setMovementType( CProjectile::PROJECTILE_USER_SEEK );
			projectile->setState( CProjectile::PROJECTILE_ATTACK );

			// sound

			if ( m_soundId == NOT_PLAYING )
			{
				m_soundId = (int) CSoundMediator::playSfx( CSoundMediator::SFX_EYEBALL_SENTRY_ATTACK, true );
			}
		}
	}
	else
	{
		DVECTOR eyeballPos = Pos;

		eyeballPos.vx += ( EYEBALL_DIST * rcos( m_heading ) ) >> 12;
		eyeballPos.vy += ( EYEBALL_DIST * rsin( m_heading ) ) >> 12;

		CProjectile *projectile;
		projectile = CProjectile::Create();
		projectile->init( eyeballPos, m_fireHeading, CProjectile::PROJECTILE_USER_SEEK, CProjectile::PROJECTILE_INFINITE_LIFE );
		projectile->setGraphic( FRM_EYEBALL_STATIC );
		projectile->setState( CProjectile::PROJECTILE_ATTACK );

		addChild( projectile );

		// sound

		if ( m_soundId == NOT_PLAYING )
		{
			m_soundId = (int) CSoundMediator::playSfx( CSoundMediator::SFX_EYEBALL_SENTRY_ATTACK, true );
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcEyeballEnemy::render()
{
	SprFrame = NULL;

	if ( m_isActive )
	{
		CEnemyThing::render();

		if (canRender())
		{
			DVECTOR &renderPos=getRenderPos();

			SprFrame = CGameScene::getSpriteBank()->printRotatedScaledSprite( FRM_EYEBALL_STALK, renderPos.vx, renderPos.vy, 4096, 4096, m_drawRotation, 10);

			// get xmax, xmin, ymax, ymin

			s32 XMax;
			s32 XMin;

			s32 YMax;
			s32 YMin;

			XMin=SprFrame->x0;
			if (XMin>SprFrame->x1) XMin=SprFrame->x1;
			if (XMin>SprFrame->x2) XMin=SprFrame->x2;
			if (XMin>SprFrame->x3) XMin=SprFrame->x3;
			XMax=SprFrame->x0;
			if (XMax<SprFrame->x1) XMax=SprFrame->x1;
			if (XMax<SprFrame->x2) XMax=SprFrame->x2;
			if (XMax<SprFrame->x3) XMax=SprFrame->x3;
			YMin=SprFrame->y0;
			if (YMin>SprFrame->y1) YMin=SprFrame->y1;
			if (YMin>SprFrame->y2) YMin=SprFrame->y2;
			if (YMin>SprFrame->y3) YMin=SprFrame->y3;
			YMax=SprFrame->y0;
			if (YMax<SprFrame->y1) YMax=SprFrame->y1;
			if (YMax<SprFrame->y2) YMax=SprFrame->y2;
			if (YMax<SprFrame->y3) YMax=SprFrame->y3;

			XMax -= renderPos.vx;
			XMin -= renderPos.vx;
			YMax -= renderPos.vy;
			YMin -= renderPos.vy;

			setCollisionSize( ( XMax - XMin ), ( YMax - YMin ) );
			setCollisionCentreOffset( ( XMax + XMin ) >> 1, ( YMax + YMin ) >> 1 );
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcEyeballEnemy::processEvent( GAME_EVENT evt, CThing *sourceThing )
{
	switch( evt )
	{
		case PROJECTILE_RETURNED_TO_SOURCE_EVENT:
		{
			m_controlFunc = NPC_CONTROL_MOVEMENT;
			m_timerFunc = NPC_TIMER_ATTACK_DONE;
			m_timerTimer = GameState::getOneSecondInFrames();
			m_sensorFunc = NPC_SENSOR_NONE;

			//removeChild( sourceThing );
			//sourceThing->shutdown();
			//delete sourceThing;

			CProjectile *projectile;
			projectile = (CProjectile *) sourceThing;
			projectile->setMovementType( CProjectile::PROJECTILE_FIXED );

			DVECTOR eyeballPos = Pos;

			eyeballPos.vx += ( EYEBALL_DIST * rcos( m_heading ) ) >> 12;
			eyeballPos.vy += ( EYEBALL_DIST * rsin( m_heading ) ) >> 12;

			projectile->setPosition( eyeballPos );

			break;
		}

		default:
			break;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcEyeballEnemy::processShotDeathStart( int _frames )
{
	CNpcEnemy::processShotDeathStart( _frames );

	CThing	*Next=getNext();
	if ( Next )
	{
		CProjectile *projectile;

		projectile = (CProjectile *) Next;

		if ( projectile->getMovementType() != CProjectile::PROJECTILE_FIXED )
		{
			projectile->setMovementType( CProjectile::PROJECTILE_DUMBFIRE );
			projectile->setLifeTime( CProjectile::PROJECTILE_FINITE_LIFE );

			removeChild( Next );
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcEyeballEnemy::processShotDeathEnd( int _frames )
{
	CNpcEnemy::processShotDeathEnd( _frames );

	m_heading = m_drawRotation - 1024;
	m_heading &= 4095;

	CThing	*Next=getNext();
	if ( Next )
	{
		CProjectile *projectile;

		projectile = (CProjectile *) Next;

		DVECTOR eyeballPos = Pos;

		eyeballPos.vx += ( EYEBALL_DIST * rcos( m_heading ) ) >> 12;
		eyeballPos.vy += ( EYEBALL_DIST * rsin( m_heading ) ) >> 12;

		projectile->setPosition( eyeballPos );
		projectile->setHeading( m_heading );
	}
}
