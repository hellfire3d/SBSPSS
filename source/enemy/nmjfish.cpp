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

#ifndef	__UTILS_HEADER__
#include	"utils\utils.h"
#endif

#ifndef __GAME_GAME_H__
#include	"game\game.h"
#endif

#ifndef	__PLAYER_PLAYER_H__
#include	"player\player.h"
#endif

#include "fx\fx.h"
#include "fx\fxjfish.h"
#include "fx\fxnrgbar.h"



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcMotherJellyfishEnemy::postInit()
{
	m_state = MOTHER_JELLYFISH_RETURN_TO_START_1;
	m_spawnTimer = 0;
	m_meterOn=false;

	if ( CLevel::getIsBossRespawn() )
	{
		m_health = CLevel::getBossHealth();
	}

	CFXJellyFishLegs	*T=(CFXJellyFishLegs*)CFX::Create(CFX::FX_TYPE_JELLYFISH_LEGS,this);
	T->Setup(80,-5,0);

	T=(CFXJellyFishLegs*)CFX::Create(CFX::FX_TYPE_JELLYFISH_LEGS,this);
	T->Setup(40,0,0);

	T=(CFXJellyFishLegs*)CFX::Create(CFX::FX_TYPE_JELLYFISH_LEGS,this);
	T->Setup(-40,-5,1);

	T=(CFXJellyFishLegs*)CFX::Create(CFX::FX_TYPE_JELLYFISH_LEGS,this);
	T->Setup(-80,0,1);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcMotherJellyfishEnemy::setupWaypoints( sThingActor *ThisActor )
{
	u16	*PntList=(u16*)MakePtr(ThisActor,sizeof(sThingActor));

	u16 newXPos, newYPos;

	s32 startX = 0;

	m_npcPath.setWaypointCount( ThisActor->PointCount - 1 );

	newXPos = (u16) *PntList;
	setWaypointPtr( PntList );
	PntList++;
	newYPos = (u16) *PntList;
	PntList++;

	setStartPos( newXPos, newYPos );

	startX = newXPos << 4;

	if ( ThisActor->PointCount > 1 )
	{
		for (int pointNum = 1 ; pointNum < ThisActor->PointCount ; pointNum++ )
		{
			newXPos = (u16) *PntList;
			PntList++;
			newYPos = (u16) *PntList;
			PntList++;

			if ( pointNum == 1 )
			{
				setHeading( newXPos, newYPos );
			}

			if ( pointNum == ThisActor->PointCount - 2 )
			{
				startX = newXPos << 4;
				m_base.vx = startX;
				m_base.vy = newYPos << 4;
			}
			else if ( pointNum == ThisActor->PointCount - 1 )
			{
				m_cycleWidth = abs( startX - ( newXPos << 4 ) );
				m_halfCycleWidth = m_cycleWidth >> 1;
				m_base.vx += m_halfCycleWidth;
			}
		}
	}
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
			xDist = m_base.vx - this->Pos.vx - m_halfCycleWidth;
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
				m_extension = -m_halfCycleWidth;
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
				if ( m_extension < m_halfCycleWidth )
				{
					m_extension += 3 * _frames;

					xExtension = ( m_halfCycleWidth * rsin( ( m_extension << 10 ) / m_halfCycleWidth ) ) >> 12;

					Pos.vx = m_base.vx + xExtension;
					Pos.vy = m_base.vy - ( ( 50 * rsin( ( xExtension << 12 ) / m_cycleWidth ) ) >> 12 );

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
				if ( m_extension > -m_halfCycleWidth )
				{
					m_extension -= 3 * _frames;

					xExtension = ( m_halfCycleWidth * rsin( ( m_extension << 10 ) / m_halfCycleWidth ) ) >> 12;

					Pos.vx = m_base.vx + xExtension;
					Pos.vy = m_base.vy + ( ( 50 * rsin( ( xExtension << 12 ) / m_cycleWidth ) ) >> 12 );

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
				if ( m_extension < m_halfCycleWidth )
				{
					m_extension += 3 * _frames;

					xExtension = ( m_halfCycleWidth * rsin( ( m_extension << 10 ) / m_halfCycleWidth ) ) >> 12;

					Pos.vx = m_base.vx + xExtension;
					Pos.vy = m_base.vy - ( ( 50 * rcos( ( xExtension << 11 ) / m_cycleWidth ) ) >> 12 );

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
				if ( m_extension > -m_halfCycleWidth )
				{
					m_extension -= 3 * _frames;

					xExtension = ( m_halfCycleWidth * rsin( ( m_extension << 10 ) / m_halfCycleWidth ) ) >> 12;

					Pos.vx = m_base.vx + xExtension;
					Pos.vy = m_base.vy + ( ( 50 * rcos( ( xExtension << 11 ) / m_cycleWidth ) ) >> 12 );

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
			enemy->setStartPos( Pos.vx >> 4, ( Pos.vy + 20 ) >> 4 );

			enemy->setWaypointCount( m_npcPath.getWaypointCount() );
			enemy->setWaypointPtr( m_npcPath.getWaypointPtr() );

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

		if (canRender())
		{
			if (!m_meterOn)
			{
				CFXNRGBar	*T=(CFXNRGBar*)CFX::Create(CFX::FX_TYPE_NRG_BAR,this);
				T->SetMax(m_health);
				m_meterOn=true;
			}

			DVECTOR &renderPos=getRenderPos();

			SprFrame = m_actorGfx->Render(renderPos,m_animNo,( m_frame >> 8 ),false);
			m_actorGfx->RotateScale( SprFrame, renderPos, 0, 8192, 8192 );

			sBBox boundingBox = m_actorGfx->GetBBox();
			setCollisionSize( ( boundingBox.XMax - boundingBox.XMin ), ( boundingBox.YMax - boundingBox.YMin ) );
			setCollisionCentreOffset( ( boundingBox.XMax + boundingBox.XMin ) >> 1, ( boundingBox.YMax + boundingBox.YMin ) >> 1 );
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcMotherJellyfishEnemy::processUserCollision( CThing *thisThing )
{
}
