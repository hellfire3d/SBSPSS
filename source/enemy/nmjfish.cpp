/*=========================================================================

	nmjfish.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef __ENEMY_NPC_H__
#include "enemy\npc.h"
#endif

#ifndef	__ENEMY_NMJFISH_H__
#include "enemy\nmjfish.h"
#endif

#ifndef __ENEMY_NPROJJF_H__
#include "enemy\nprojjf.h"
#endif

#ifndef __VID_HEADER_
#include "system\vid.h"
#endif

#ifndef __GAME_GAME_H__
#include	"game\game.h"
#endif

#ifndef	__PLAYER_PLAYER_H__
#include	"player\player.h"
#endif

#include "fx\fx.h"
#include "fx\fxjfish.h"


#define MJ_CYCLE_WIDTH			400
#define MJ_HALF_CYCLE_WIDTH		( MJ_CYCLE_WIDTH >> 1 )


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcMotherJellyfishEnemy::postInit()
{
	m_state = MOTHER_JELLYFISH_RETURN_TO_START_1;
	m_spawnTimer = 0;

	if ( CLevel::getIsBossRespawn() )
	{
		m_health = CLevel::getBossHealth();
	}

	CFXJellyFishLegs	*T=(CFXJellyFishLegs*)CFX::Create(CFX::FX_TYPE_JELLYFISH_LEGS,this);
	T->SetUp(128,4,8,16);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcMotherJellyfishEnemy::processMovement( int _frames )
{
	s32 xDist, yDist;
	s32 xDistSqr, yDistSqr;

	switch( m_state )
	{
		case MOTHER_JELLYFISH_RETURN_TO_START_1:
		case MOTHER_JELLYFISH_RETURN_TO_START_2:
		case MOTHER_JELLYFISH_RETURN_TO_START_3:
		{
			xDist = m_base.vx - this->Pos.vx - MJ_HALF_CYCLE_WIDTH;
			xDistSqr = xDist * xDist;
			yDist = m_base.vy - this->Pos.vy;
			yDistSqr = yDist * yDist;

			if ( xDistSqr + yDistSqr > 100 )
			{
				processGenericGotoTarget( _frames, xDist, yDist, m_speed );
			}
			else
			{
				// have arrived at base position

				m_movementTimer = GameState::getOneSecondInFrames() * 10;
				m_state++;
				m_extension = -MJ_HALF_CYCLE_WIDTH;
				m_extendDir = EXTEND_RIGHT;
			}

			break;
		}

		case MOTHER_JELLYFISH_CYCLE_1:
		case MOTHER_JELLYFISH_CYCLE_2:
		case MOTHER_JELLYFISH_CYCLE_3:
		{
			m_movementTimer -= _frames;

			s32 xExtension;

			if ( m_extendDir == EXTEND_RIGHT )
			{
				if ( m_extension < MJ_HALF_CYCLE_WIDTH )
				{
					m_extension += 3 * _frames;

					xExtension = ( MJ_HALF_CYCLE_WIDTH * rsin( ( m_extension << 10 ) / MJ_HALF_CYCLE_WIDTH ) ) >> 12;

					Pos.vx = m_base.vx + xExtension;
					Pos.vy = m_base.vy - ( ( 50 * rsin( ( xExtension << 12 ) / MJ_CYCLE_WIDTH ) ) >> 12 );

					m_heading = 0;
				}
				else
				{
					m_extendDir = EXTEND_LEFT;

					if ( m_movementTimer < 0 )
					{
						m_controlFunc = NPC_CONTROL_CLOSE;
						m_state++;
						m_jellyfishCount = 3;
					}
				}
			}
			else
			{
				if ( m_extension > -MJ_HALF_CYCLE_WIDTH )
				{
					m_extension -= 3 * _frames;

					xExtension = ( MJ_HALF_CYCLE_WIDTH * rsin( ( m_extension << 10 ) / MJ_HALF_CYCLE_WIDTH ) ) >> 12;

					Pos.vx = m_base.vx + xExtension;
					Pos.vy = m_base.vy + ( ( 50 * rsin( ( xExtension << 12 ) / MJ_CYCLE_WIDTH ) ) >> 12 );

					m_heading = 2048;
				}
				else
				{
					m_extendDir = EXTEND_RIGHT;

					if ( m_movementTimer < 0 )
					{
						m_controlFunc = NPC_CONTROL_CLOSE;
						m_state++;
						m_jellyfishCount = 3;
					}
				}
			}

			break;
		}

		default:
			break;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcMotherJellyfishEnemy::processClose( int _frames )
{
	switch( m_state )
	{
		case MOTHER_JELLYFISH_ATTACK_PLAYER_SHOCK:
		{
			// seek position above user

			CPlayer *player = GameScene.getPlayer();
			DVECTOR playerPos = player->getPos();
			DVECTOR seekPos;
			s32 xDist, yDist;
			s32 xDistSqr, yDistSqr;

			seekPos = playerPos;
			seekPos.vy -= 100;

			xDist = seekPos.vx - this->Pos.vx;
			xDistSqr = xDist * xDist;
			yDist = seekPos.vy - this->Pos.vy;
			yDistSqr = yDist * yDist;

			if ( xDistSqr + yDistSqr > 400 )
			{
				processGenericGotoTarget( _frames, xDist, yDist, m_speed );
			}
			else
			{
				// fire at user

				m_controlFunc = NPC_CONTROL_MOVEMENT;
				m_state = MOTHER_JELLYFISH_RETURN_TO_START_1;
			}

			break;
		}

		default:
		{
			s32 xExtension;

			if ( m_extendDir == EXTEND_RIGHT )
			{
				if ( m_extension < MJ_HALF_CYCLE_WIDTH )
				{
					m_extension += 3 * _frames;

					xExtension = ( MJ_HALF_CYCLE_WIDTH * rsin( ( m_extension << 10 ) / MJ_HALF_CYCLE_WIDTH ) ) >> 12;

					Pos.vx = m_base.vx + xExtension;
					Pos.vy = m_base.vy - ( ( 50 * rcos( ( xExtension << 11 ) / MJ_CYCLE_WIDTH ) ) >> 12 );

					m_heading = 0;

					spawnJellyfish( _frames );
				}
				else
				{
					m_controlFunc = NPC_CONTROL_MOVEMENT;
					m_state++;
				}
			}
			else
			{
				if ( m_extension > -MJ_HALF_CYCLE_WIDTH )
				{
					m_extension -= 3 * _frames;

					xExtension = ( MJ_HALF_CYCLE_WIDTH * rsin( ( m_extension << 10 ) / MJ_HALF_CYCLE_WIDTH ) ) >> 12;

					Pos.vx = m_base.vx + xExtension;
					Pos.vy = m_base.vy + ( ( 50 * rcos( ( xExtension << 11 ) / MJ_CYCLE_WIDTH ) ) >> 12 );

					m_heading = 2048;

					spawnJellyfish( _frames );
				}
				else
				{
					m_controlFunc = NPC_CONTROL_MOVEMENT;
					m_state++;
				}
			}

			break;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcMotherJellyfishEnemy::shutdown()
{
	if ( m_isActive )
	{
		CLevel::setIsBossRespawn( true );
		CLevel::setBossHealth( m_health );
	}

	CNpcEnemy::shutdown();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcMotherJellyfishEnemy::spawnJellyfish( int _frames )
{
	if ( m_jellyfishCount )
	{
		if ( m_spawnTimer > 0 )
		{
			m_spawnTimer -= _frames;
		}
		else
		{
			CNpcEnemy *enemy;
			enemy = new( "jellyfish projectile" ) CNpcSmallJellyfishProjectileEnemy;
			ASSERT(enemy);
			enemy->setType( CNpcEnemy::NPC_PROJECTILE_JELLYFISH );
			enemy->init();
			enemy->setLayerCollision( m_layerCollision );
			enemy->setStartPos( Pos.vx >> 4, ( Pos.vy + 20 ) >> 4 );

			CNpcWaypoint *sourceWaypoint = m_npcPath.getWaypointList();

			if ( sourceWaypoint )
			{
				while( sourceWaypoint )
				{
					enemy->addWaypoint( sourceWaypoint->pos.vx >> 4, sourceWaypoint->pos.vy >> 4 );
					sourceWaypoint = sourceWaypoint->nextWaypoint;
				}
			}

			enemy->setPathType( CNpcPath::PONG_PATH );
			enemy->postInit();

			m_jellyfishCount--;

			m_spawnTimer = 1 * GameState::getOneSecondInFrames();
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcMotherJellyfishEnemy::render()
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
				SprFrame = m_actorGfx->Render(renderPos,m_animNo,( m_frame >> 8 ),false);
				m_actorGfx->RotateScale( SprFrame, renderPos, m_drawRotation, 8192, 8192 );

				sBBox boundingBox = m_actorGfx->GetBBox();
				setCollisionSize( ( boundingBox.XMax - boundingBox.XMin ), ( boundingBox.YMax - boundingBox.YMin ) );
				setCollisionCentreOffset( ( boundingBox.XMax + boundingBox.XMin ) >> 1, ( boundingBox.YMax + boundingBox.YMin ) >> 1 );
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcMotherJellyfishEnemy::processUserCollision( CThing *thisThing )
{
}
