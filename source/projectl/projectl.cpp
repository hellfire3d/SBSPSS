/*=========================================================================

	projectl.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef	__PROJECTL_PROJECTL_H__
#include "projectl\projectl.h"
#endif

#ifndef __GFX_SPRBANK_H__
#include "gfx\sprbank.h"
#endif

#ifndef __LEVEL_LEVEL_H__
#include "level\level.h"
#endif

#ifndef __FILE_EQUATES_H__
#include <biglump.h>
#endif

#ifndef __SPR_SPRITES_H__
#include <sprites.h>
#endif

#ifndef __VID_HEADER_
#include "system\vid.h"
#endif

#ifndef __GAME_GAME_H__
#include "game\game.h"
#endif

#ifndef	__PLAYER_PLAYER_H__
#include "player\player.h"
#endif

#ifndef	__ENEMY_NPC_H__
#include "enemy\npc.h"
#endif

#include "gfx\otpos.h"
#include "fx\fx.h"
#include "fx\fxbaseanim.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CProjectile	*CProjectile::Create()
{
	CProjectile *projectile;

	projectile = (CProjectile*)CThingManager::GetThing(TYPE_ENEMYPROJECTILE,0);
	if ( !projectile )
	{
		projectile = new ("enemy projectile") CProjectile;
	}

	ASSERT(projectile);
	projectile->setThingSubType(0);

	return( projectile );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CProjectile::init()
{
	CEnemyProjectileThing::init();

	m_spriteFrame = FRM__SPIKE;

	m_initHeading = m_heading = 0;
	m_lifetime = GameState::getOneSecondInFrames() * 2;
	m_movementType = PROJECTILE_DUMBFIRE;
	m_lifetimeType = PROJECTILE_FINITE_LIFE;
	m_state = PROJECTILE_ATTACK;
	m_turnSpeed = 256;
	m_extension = 0;
	m_ot = OTPOS__ACTOR_POS;
	m_speed = 3;
	m_xScale = ONE;
	m_yScale = ONE;
	m_shock = false;
	updateCollisionArea();
	m_highResX = 0;
	m_highResY = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CProjectile::init( DVECTOR initPos, s16 initHeading )
{
	init();

	m_initHeading = m_heading = initHeading;
	m_initPos = Pos = initPos;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CProjectile::init( DVECTOR initPos, s16 initHeading, PROJECTILE_MOVEMENT_TYPE initMoveType, PROJECTILE_LIFETIME_TYPE initLifeType )
{
	init( initPos, initHeading );

	m_movementType = initMoveType;
	m_lifetimeType = initLifeType;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CProjectile::init( DVECTOR initPos, s16 initHeading, PROJECTILE_MOVEMENT_TYPE initMoveType, PROJECTILE_LIFETIME_TYPE initLifeType, s32 initLifetime )
{
	init( initPos, initHeading, initMoveType, initLifeType );

	m_lifetime = initLifetime;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CProjectile::shutdown()
{
	CEnemyProjectileThing::shutdown();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CProjectile::setGraphic( int frame )
{
	m_spriteFrame = frame;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool CProjectile::processTargetSeek( int _frames, DVECTOR targetPos )
{
	s32 moveX = 0, moveY = 0;

	s16 moveDist = 0;

	s32 moveVel = 0;

	s32 xDist, yDist;
	s32 xDistSqr, yDistSqr;

	xDist = targetPos.vx - this->Pos.vx;
	xDistSqr = xDist * xDist;

	yDist = targetPos.vy - this->Pos.vy;
	yDistSqr = yDist * yDist;

	//if ( xDistSqr + yDistSqr > 22500 )
	//{
		//this->m_controlFunc = NPC_CONTROL_MOVEMENT;
	//}
	//else
	{
		s16 headingToTarget = ratan2( yDist, xDist );
		s16 maxTurnRate = m_turnSpeed;
		s16 decDir, incDir;

		decDir = m_heading - headingToTarget;

		if ( decDir < 0 )
		{
			decDir += ONE;
		}

		incDir = headingToTarget - m_heading;

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

		m_heading += moveDist;

		m_heading &= 4095;
		
		s32 preShiftX = _frames * m_speed * rcos( m_heading );
		s32 preShiftY = _frames * m_speed * rsin( m_heading );

		moveX = preShiftX >> 12;
		if ( !moveX && preShiftX )
		{
			moveX = preShiftX / abs( preShiftX );
		}

		moveY = preShiftY >> 12;
		if ( !moveY && preShiftY )
		{
			moveY = preShiftY / abs( preShiftY );
		}

		Pos.vx += moveX;
		Pos.vy += moveY;
	}

	xDist = targetPos.vx - this->Pos.vx;
	xDistSqr = xDist * xDist;

	yDist = targetPos.vy - this->Pos.vy;
	yDistSqr = yDist * yDist;

	if ( xDistSqr + yDistSqr < 100 )
	{
		return( true );
	}
	else
	{
		return( false );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CProjectile::setMovementType( PROJECTILE_MOVEMENT_TYPE moveType )
{
	m_movementType = moveType;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CProjectile::PROJECTILE_MOVEMENT_TYPE CProjectile::getMovementType()
{
	return( m_movementType );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CProjectile::setState( PROJECTILE_STATE newState )
{
	m_state = newState;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CProjectile::setLifeTime( PROJECTILE_LIFETIME_TYPE lifeType )
{
	m_lifetimeType = lifeType;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CProjectile::think(int _frames)
{
	if ( _frames > 2 )
	{
		_frames = 2;
	}

	CEnemyProjectileThing::think( _frames );

	switch( m_movementType )
	{
		case PROJECTILE_FIXED:
		{
			// don't move at all

			break;
		}

		case PROJECTILE_USER_SEEK:
		{
			if ( CGameScene::getCollision()->Get( Pos.vx >> 4, Pos.vy >> 4 ) )
			{
				setToShutdown();
			}
			else
			{
				switch( m_state )
				{
					case PROJECTILE_RETURN:
					{
						if ( processTargetSeek( _frames, m_initPos ) )
						{
							CThing	*Parent=getParent();
							Parent->processEvent( PROJECTILE_RETURNED_TO_SOURCE_EVENT, this );
							m_heading = m_initHeading;
						}

						break;
					}

					case PROJECTILE_ATTACK:
					default:
					{
						CPlayer *player = GameScene.getPlayer();
						DVECTOR const &playerPos = player->getPos();

						if ( processTargetSeek( _frames, playerPos ) )
						{
							m_state = PROJECTILE_RETURN;
						}

						break;
					}
				}
			}

			break;
		}

		case PROJECTILE_GAS_CLOUD:
		{
			u16 targetExtension = 100 << 8;

			if ( m_extension < targetExtension )
			{
				m_extension += ( ( targetExtension - m_extension ) * _frames ) >> 8;

				Pos = m_initPos;
				Pos.vx += ( m_extension * rcos( m_heading ) ) >> 20;
				Pos.vy += ( m_extension * rsin( m_heading ) ) >> 20;
			}
			else
			{
				// expand
			}

			break;
		}

		case PROJECTILE_MINE:
		{
			s32 moveY = _frames * m_speed;

			s32 groundHeight = CGameScene::getCollision()->getHeightFromGround( Pos.vx, Pos.vy + moveY, 16 );

			if ( groundHeight < 0 )
			{
				int		blockType;

				blockType = CGameScene::getCollision()->getCollisionBlock( Pos.vx, Pos.vy + moveY ) & COLLISION_TYPE_MASK;

				if ( blockType == COLLISION_TYPE_FLAG_DESTRUCTABLE_FLOOR )
				{
					DVECTOR newPos;
					newPos.vx = Pos.vx;
					newPos.vy = ( ( ( Pos.vy + moveY ) >> 4 ) + 1 ) << 4;

					GameScene.GetLevel().destroyMapTile( newPos );
				}
				else
				{
					moveY = groundHeight;
				}
			}

			Pos.vy += moveY;
		}

		case PROJECTILE_DUMBFIRE:
		default:
		{
			if ( CGameScene::getCollision()->Get( Pos.vx >> 4, Pos.vy >> 4 ) )
			{
				setToShutdown();
			}
			else
			{
				m_highResX += ( _frames * ( m_speed << 8 ) * rcos( m_heading ) ) >> 12;
				m_highResY += ( _frames * ( m_speed << 8 ) * rsin( m_heading ) ) >> 12;

				if ( abs( m_highResX ) > 256 )
				{
					Pos.vx += m_highResX >> 8;
					m_highResX -= ( m_highResX >> 8 ) << 8;
				}

				if ( abs( m_highResY ) > 256 )
				{
					Pos.vy += m_highResY >> 8;
					m_highResY -= ( m_highResY >> 8 ) << 8;
				}
			}

			break;
		}
	}

	CLevel &level = GameScene.GetLevel();
	DVECTOR const &MapSize=level.getMapSize16();
	if ( Pos.vx < 0 || Pos.vx >= MapSize.vx ||
			Pos.vy < 0 || Pos.vy >= MapSize.vy )
	{
		setToShutdown();
	}

	if ( m_lifetimeType == PROJECTILE_FINITE_LIFE )
	{
		m_lifetime -= _frames;

		if ( m_lifetime <= 0 )
		{
			setToShutdown();

			if ( m_movementType == PROJECTILE_MINE )
			{
				if (canRender())
				{
					CFX *newFX = CFX::Create( CFX::FX_TYPE_EXPLODE, Pos );
					((CFXBaseAnim*)newFX)->SetScale(ONE*2);
				}

				CGameScene::setCameraShake(0,8);
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CProjectile::render()
{
	CEnemyProjectileThing::render();

	sFrameHdr	*frameHdr;

	int		x,y;
	int		scrnWidth = VidGetScrW();
	int		scrnHeight = VidGetScrH();
	int		spriteWidth = CGameScene::getSpriteBank()->getFrameWidth( m_spriteFrame );
	int		spriteHeight = CGameScene::getSpriteBank()->getFrameHeight( m_spriteFrame );

	DVECTOR const &offset = CLevel::getCameraPos();

	x = Pos.vx - offset.vx /*+ ( scrnWidth >> 1 ) - ( spriteWidth >> 1 )*/;
	y = Pos.vy - offset.vy /*+ ( scrnHeight >> 1 ) - ( spriteHeight >> 1 )*/;

	if ( x < -spriteWidth || y < -spriteHeight || x > scrnWidth || y > scrnHeight )
	{
		return;
	}

	//CGameScene::getSpriteBank()->printFT4(FRM__SPIKE,x,y,0,0,0);

	frameHdr = CGameScene::getSpriteBank()->getFrameHeader( m_spriteFrame );

	CGameScene::getSpriteBank()->printRotatedScaledSprite( frameHdr, x, y, m_xScale, m_yScale, m_heading, m_ot );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CProjectile::processEvent( GAME_EVENT evt, CThing *sourceThing )
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CProjectile::collidedWith(CThing *_thisThing)
{
	switch(_thisThing->getThingType())
	{
		case TYPE_PLAYER:
		{
			CPlayer *player = (CPlayer *) _thisThing;

			if ( m_shock )
			{
				player->takeDamage( DAMAGE__SHOCK_ENEMY );
			}
			else
			{
				player->takeDamage( DAMAGE__HIT_ENEMY );
			}

			if ( m_lifetimeType != PROJECTILE_INFINITE_LIFE )
			{
				setToShutdown();

				if ( m_movementType == PROJECTILE_MINE )
				{
					CFX::Create( CFX::FX_TYPE_EXPLODE, Pos );
				}
			}

			break;
		}

		default:
			ASSERT(0);
			break;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CPlayerProjectile	*CPlayerProjectile::Create()
{
	CPlayerProjectile *projectile;

	projectile = (CPlayerProjectile*)CThingManager::GetThing(TYPE_PLAYERPROJECTILE,0);
	if ( !projectile )
	{
		projectile = new ("player projectile") CPlayerProjectile;
	}

	ASSERT(projectile);
	projectile->setThingSubType(0);

	return( projectile );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CPlayerProjectile::init()
{
	CPlayerProjectileThing::init();
	m_heading = 0;
	m_lifetime = GameState::getOneSecondInFrames() * 2;
	m_movementType = PLAYER_PROJECTILE_DUMBFIRE;
	m_lifetimeType = PLAYER_PROJECTILE_FINITE_LIFE;
	m_turnSpeed = 256;
	m_extension = 0;
	m_frame = 0;
	m_reversed = 0;
	m_highResX = 0;
	m_highResY = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CPlayerProjectile::init( DVECTOR initPos, s16 initHeading )
{
	init();

	m_heading = initHeading;
	m_initPos = Pos = initPos;
	m_vertVelocity = 0;
	m_bounceCount = 0;
	m_hitTarget = false;

	if ( m_heading > 1024 && m_heading < 3072 )
	{
		m_reversed = 1;
	}

	m_RGB.r = 0;
	m_RGB.g = 0;
	m_RGB.b = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CPlayerProjectile::init( DVECTOR initPos, s16 initHeading, PLAYER_PROJECTILE_MOVEMENT_TYPE initMoveType, PLAYER_PROJECTILE_LIFETIME_TYPE initLifeType )
{
	init( initPos, initHeading );

	m_movementType = initMoveType;
	m_lifetimeType = initLifeType;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CPlayerProjectile::init( DVECTOR initPos, s16 initHeading, PLAYER_PROJECTILE_MOVEMENT_TYPE initMoveType, PLAYER_PROJECTILE_LIFETIME_TYPE initLifeType, s32 initLifetime )
{
	init( initPos, initHeading, initMoveType, initLifeType );

	m_lifetime = initLifetime;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CPlayerProjectile::shutdown()
{

	CPlayerProjectileThing::shutdown();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CPlayerProjectile::setMovementType( PLAYER_PROJECTILE_MOVEMENT_TYPE moveType )
{
	m_movementType = moveType;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CPlayerProjectile::PLAYER_PROJECTILE_MOVEMENT_TYPE CPlayerProjectile::getMovementType()
{
	return( m_movementType );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CPlayerProjectile::setLifeTime( PLAYER_PROJECTILE_LIFETIME_TYPE lifeType )
{
	m_lifetimeType = lifeType;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CPlayerProjectile::think(int _frames)
{
	if ( _frames > 2 )
	{
		_frames = 2;
	}

	CPlayerProjectileThing::think( _frames );

	/*m_frame += _frames;

	if ( m_frame > ( FRM_JELLYFISH1_SWIM6 - FRM_JELLYFISH1_SWIM1 ) )
	{
		m_frame = 0;
	}*/

	switch( m_movementType )
	{
		case PLAYER_PROJECTILE_BOUNCING:
		{
			if ( !m_hitTarget )
			{
				Pos.vx += ( _frames * 5 * rcos( m_heading ) ) >> 12;

				m_vertVelocity += 3 << 7;

				if ( m_vertVelocity > ( 7 << 8 ) )
				{
					m_vertVelocity = 7 << 8;
				}
				else if ( m_vertVelocity < -( 7 << 8 ) )
				{
					m_vertVelocity = -( 7 << 8 );
				}

				s16 moveY = ( m_vertVelocity >> 8 ) * _frames;

				s32 groundHeight = CGameScene::getCollision()->getHeightFromGroundAmmo( Pos.vx, Pos.vy, moveY );

				if ( groundHeight < moveY )
				{
					// destroy destructable tiles

					CLevel &level = GameScene.GetLevel();

					if ( groundHeight < -16 )
					{
						m_hitTarget = true;
						m_lifetimeType = PLAYER_PROJECTILE_FINITE_LIFE;
						m_lifetime = GameState::getOneSecondInFrames() >> 2;
					}
					else
					{
						// reverse vertical velocity

						m_vertVelocity = -m_vertVelocity;

						if ( m_bounceCount > 2 )
						{
							m_hitTarget = true;
							m_lifetimeType = PLAYER_PROJECTILE_FINITE_LIFE;
							m_lifetime = GameState::getOneSecondInFrames() >> 2;
						}
						else
						{
							m_bounceCount++;
						}
					}

					CSoundMediator::playSfx( CSoundMediator::SFX_JELLYFISH_BOUNCE );
				}
				else
				{
					Pos.vy += moveY;
				}
			}

			break;
		}

		case PLAYER_PROJECTILE_DUMBFIRE:
		case PLAYER_PROJECTILE_DUMBFIRE_CORAL:
		default:
		{
			if ( CGameScene::getCollision()->Get( Pos.vx >> 4, Pos.vy >> 4 ) )
			{
				// destroy destructable tiles

				CLevel &level = GameScene.GetLevel();

				if ( m_movementType == PLAYER_PROJECTILE_DUMBFIRE_CORAL )
				{
					level.destroyMapArea( Pos );
				}

				setToShutdown();
			}
			else
			{
				m_highResX += ( _frames * ( 5 << 8 ) * rcos( m_heading ) ) >> 12;
				m_highResY += ( _frames * ( 5 << 8 ) * rsin( m_heading ) ) >> 12;

				if ( abs( m_highResX ) > 256 )
				{
					Pos.vx += m_highResX >> 8;
					m_highResX -= ( m_highResX >> 8 ) << 8;
				}

				if ( abs( m_highResY ) > 256 )
				{
					Pos.vy += m_highResY >> 8;
					m_highResY -= ( m_highResY >> 8 ) << 8;
				}
			}

			break;
		}
	}

	CLevel &level = GameScene.GetLevel();
	DVECTOR const &MapSize=level.getMapSize16();
	if ( Pos.vx < 0 || Pos.vx >= MapSize.vx ||
			Pos.vy < 0 || Pos.vy >= MapSize.vy )
	{
		setToShutdown();
	}

	if ( m_lifetimeType == PLAYER_PROJECTILE_FINITE_LIFE )
	{
		m_lifetime -= _frames;

		if ( m_lifetime <= 0 )
		{
			setToShutdown();
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CPlayerProjectile::render()
{
	CPlayerProjectileThing::render();

	int		x,y;
	int		scrnWidth = VidGetScrW();
	int		scrnHeight = VidGetScrH();

	DVECTOR const &offset = CLevel::getCameraPos();

	if ( m_hitTarget )
	{
		x = Pos.vx - offset.vx;
		y = Pos.vy - offset.vy;

		SprFrame = CGameScene::getSpriteBank()->printRotatedScaledSprite( FRM__BALLOONBURST, x, y, 4096 << 1, 4096 << 1, 0, 0 );
		setRGB0( SprFrame, m_RGB.r, m_RGB.g, m_RGB.b );
	}
	else
	{
		int		spriteWidth = CGameScene::getSpriteBank()->getFrameWidth(m_frame);
		int		spriteHeight = CGameScene::getSpriteBank()->getFrameHeight(m_frame);

		if ( m_reversed )
		{
			x = Pos.vx - offset.vx + ( spriteWidth >> 1 );
		}
		else
		{
			x = Pos.vx - offset.vx - ( spriteWidth >> 1 );
		}

		y = Pos.vy - offset.vy - ( spriteHeight >> 1 );

		if ( x < -spriteWidth || y < -spriteHeight || x > scrnWidth || y > scrnHeight )
		{
			return;
		}

		SprFrame = CGameScene::getSpriteBank()->printFT4(FRM_JELLYFISH1_SWIM1 + m_frame,x,y,m_reversed,0,0);
		setRGB0( SprFrame, m_RGB.r, m_RGB.g, m_RGB.b );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CPlayerProjectile::processEvent( GAME_EVENT evt, CThing *sourceThing )
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CPlayerProjectile::collidedWith(CThing *_thisThing)
{
	switch(_thisThing->getThingType())
	{
		case TYPE_ENEMY:
		{
			CNpcEnemy *enemy = (CNpcEnemy *) _thisThing;

			if ( enemy->hasBeenAttacked() )
			{
				setToShutdown();
			}

			break;
		}

		default:
			ASSERT(0);
			break;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CPlayerProjectile::setRGB( u32 new_RGB )
{
	m_RGB.r = new_RGB & 255;
	m_RGB.g = ( new_RGB >> 8 ) & 255;
	m_RGB.b = ( new_RGB >> 16 ) & 255;
}

/*****************************************************************************/
