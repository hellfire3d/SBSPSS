/*=========================================================================

	nsshark.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef __ENEMY_NPC_H__
#include "enemy\npc.h"
#endif

#ifndef	__ENEMY_NSSHARK_H__
#include "enemy\nsshark.h"
#endif

#ifndef __GAME_GAME_H__
#include	"game\game.h"
#endif

#ifndef	__PLAYER_PLAYER_H__
#include	"player\player.h"
#endif

#ifndef __PROJECTL_PROJECTL_H__
#include "projectl\projectl.h"
#endif

#ifndef	__ANIM_SHARKSUB_HEADER__
#include <ACTOR_SHARKSUB_ANIM.h>
#endif

#ifndef __VID_HEADER_
#include "system\vid.h"
#endif

#ifndef	__UTILS_HEADER__
#include	"utils\utils.h"
#endif

#ifndef __SPR_SPRITES_H__
#include <sprites.h>
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcSubSharkEnemy::postInit()
{
	m_state = SUB_SHARK_MINE_1;
	m_extendDir = EXTEND_RIGHT;
	m_npcPath.setPathType( CNpcPath::PONG_PATH );
	m_salvoCount = 0;

	if ( CLevel::getIsBossRespawn() )
	{
		m_health = CLevel::getBossHealth();
		m_speed = m_data[m_type].speed + ( ( 3 * ( m_data[m_type].initHealth - m_health ) ) / m_data[m_type].initHealth );
	}

	m_timerTimer = 0;
	m_salvoCount = 5;
	m_carryPlayer = false;
	m_movementTimer = GameState::getOneSecondInFrames() * ( 1 + ( ( 7 * m_health ) / m_data[m_type].initHealth ) );

	CNpcBossEnemy::postInit();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcSubSharkEnemy::processMovement( int _frames )
{
	if ( m_movementTimer > 0 )
	{
		m_movementTimer -= _frames;
	}

	switch( m_state )
	{
		case SUB_SHARK_MINE_1:
		{
			if ( m_soundId == NOT_PLAYING )
			{
				m_soundId = (int) CSoundMediator::playSfx( m_data[m_type].moveSfx, true );
			}

			if ( !m_animPlaying )
			{
				m_animPlaying = true;
				m_animNo = ANIM_SHARKSUB_SWIM;
				m_frame = 0;
			}

			if ( m_timerTimer <= 0 )
			{
				if ( m_salvoCount > 0 )
				{
					// drop mine

					CProjectile *projectile;
					projectile = CProjectile::Create();
					projectile->init( Pos, 1024, CProjectile::PROJECTILE_MINE, CProjectile::PROJECTILE_FINITE_LIFE );
					projectile->setGraphic( FRM__SHARKMINE );

					m_salvoCount--;

					m_timerTimer = ( GameState::getOneSecondInFrames() >> 2 ) * ( 1 + ( ( 3 * m_health ) / m_data[m_type].initHealth ) );
				}
			}

			if ( m_movementTimer <= 0 && m_salvoCount <= 0 )
			{
				m_state++;
				m_timerTimer = 0;
				m_movementTimer = GameState::getOneSecondInFrames() * ( 1 + ( ( 7 * m_health ) / m_data[m_type].initHealth ) );
				m_salvoCount = 5;

				if ( m_state == SUB_SHARK_GOTO_CHARGE )
				{
					s32 minX, maxX, minY, maxY;

					m_npcPath.getPathXExtents( &minX, &maxX );
					m_npcPath.getPathYExtents( &minY, &maxY );

					m_targetPos.vx = minX;
					m_targetPos.vy = minY;
				}
			}

			s32 moveX = 0, moveY = 0;
			s32 moveVel = 0;
			s32 moveDist = 0;

			processGenericFixedPathMove( _frames, &moveX, &moveY, &moveVel, &moveDist );

			if ( moveX > 0 )
			{
				m_extendDir = EXTEND_RIGHT;
			}
			else
			{
				m_extendDir = EXTEND_LEFT;
			}

			Pos.vx += moveX;
			Pos.vy += moveY;

			break;
		}

		case SUB_SHARK_GOTO_CHARGE:
		{
			if ( m_soundId == NOT_PLAYING )
			{
				m_soundId = (int) CSoundMediator::playSfx( m_data[m_type].moveSfx, true );
			}

			if ( !m_animPlaying )
			{
				m_animPlaying = true;
				m_animNo = ANIM_SHARKSUB_SWIM;
				m_frame = 0;
			}

			s32 distX, distY;

			distX = m_targetPos.vx - Pos.vx;
			distY = m_targetPos.vy - Pos.vy;

			if ( distX > 0 )
			{
				m_extendDir = EXTEND_RIGHT;
			}
			else
			{
				m_extendDir = EXTEND_LEFT;
			}

			if( abs( distX ) < 10 && abs( distY ) < 10 )
			{
				s32 minX, maxX, minY, maxY;

				m_npcPath.getPathXExtents( &minX, &maxX );
				m_npcPath.getPathYExtents( &minY, &maxY );

				m_targetPos.vx = minX;
				m_targetPos.vy = maxY;

				m_state = SUB_SHARK_DROP;
			}
			else
			{
				processGenericGotoTarget( _frames, distX, distY, m_speed );
			}

			break;
		}

		case SUB_SHARK_DROP:
		{
			if ( m_soundId == NOT_PLAYING )
			{
				m_soundId = (int) CSoundMediator::playSfx( m_data[m_type].moveSfx, true );
			}

			if ( !m_animPlaying )
			{
				m_animPlaying = true;
				m_animNo = ANIM_SHARKSUB_SWIM;
				m_frame = 0;
			}

			s32 distX, distY;

			distX = m_targetPos.vx - Pos.vx;
			distY = m_targetPos.vy - Pos.vy;

			m_extendDir = EXTEND_RIGHT;

			if( abs( distY ) == 0 )
			{
				m_state = SUB_SHARK_START_CHARGE;
				m_timerTimer = GameState::getOneSecondInFrames() >> 8;
				m_movementTimer = GameState::getOneSecondInFrames();
			}
			else
			{
				processGenericGotoTarget( _frames, distX, distY, m_speed );
			}

			break;
		}

		case SUB_SHARK_START_CHARGE:
		{
			if ( m_soundId == NOT_PLAYING )
			{
				m_soundId = (int) CSoundMediator::playSfx( CSoundMediator::SFX_SHARK___CREAKING_ATTACK_SOUND, true );
			}

			if ( !m_animPlaying )
			{
				m_animPlaying = true;
				m_animNo = ANIM_SHARKSUB_SPRINTOPEN;
				m_frame = 0;
			}

			if ( m_movementTimer <= 0 )
			{
				s32 minX, maxX, minY, maxY;

				m_npcPath.getPathXExtents( &minX, &maxX );
				m_npcPath.getPathYExtents( &minY, &maxY );

				m_targetPos.vx = maxX;
				m_targetPos.vy = maxY;

				m_state = SUB_SHARK_CHARGE;
			}

			if ( m_timerTimer > 0 )
			{
				m_timerTimer -= _frames;
			}
			else
			{
				DVECTOR bubblePos = Pos;
				bubblePos.vx -= 20 + ( getRnd() % 30 );
				bubblePos.vy -= getRnd() % 50;
				CFX::Create( CFX::FX_TYPE_BUBBLE_WATER, bubblePos );

				if ( m_soundId == NOT_PLAYING )
				{
					m_soundId = (int) CSoundMediator::playSfx( CSoundMediator::SFX_SPLASH, true );
				}

				m_timerTimer = GameState::getOneSecondInFrames() >> 8;
			}

			break;
		}

		case SUB_SHARK_CHARGE:
		{
			if ( m_soundId == NOT_PLAYING )
			{
				m_soundId = (int) CSoundMediator::playSfx( m_data[m_type].moveSfx, true );
			}

			if ( !m_carryPlayer && abs( playerXDist ) < 200 )
			{
				if ( m_animNo != ANIM_SHARKSUB_CHOMP || !m_animPlaying )
				{
					m_animPlaying = true;
					m_animNo = ANIM_SHARKSUB_CHOMP;
					m_frame = 0;
					CSoundMediator::playSfx( CSoundMediator::SFX_SHARK___CHOMP );
				}
			}
			else
			{
				if ( !m_animPlaying )
				{
					m_animPlaying = true;
					m_animNo = ANIM_SHARKSUB_SPRINTOPEN;
					m_frame = 0;
				}
			}

			s32 distX, distY;

			distX = m_targetPos.vx - Pos.vx;
			distY = m_targetPos.vy - Pos.vy;

			if( abs( distX ) < 10 && abs( distY ) < 10 )
			{
				s32 minX, maxX, minY, maxY;

				m_npcPath.getPathXExtents( &minX, &maxX );
				m_npcPath.getPathYExtents( &minY, &maxY );

				m_targetPos.vx = maxX;
				m_targetPos.vy = minY;

				m_state = SUB_SHARK_END_CHARGE;
			}
			else
			{
				processGenericGotoTarget( _frames, distX, distY, m_speed << 2 );
			}

			break;
		}

		case SUB_SHARK_END_CHARGE:
		{
			if ( m_soundId == NOT_PLAYING )
			{
				m_soundId = (int) CSoundMediator::playSfx( m_data[m_type].moveSfx, true );
			}

			if ( !m_animPlaying )
			{
				m_animPlaying = true;
				m_animNo = ANIM_SHARKSUB_SWIM;
				m_frame = 0;
			}

			s32 distX, distY;

			distX = m_targetPos.vx - Pos.vx;
			distY = m_targetPos.vy - Pos.vy;

			if( abs( distX ) < 10 && abs( distY ) < 10 )
			{
				m_state = SUB_SHARK_MINE_1;
				m_movementTimer = GameState::getOneSecondInFrames() * ( 1 + ( ( 7 * m_health ) / m_data[m_type].initHealth ) );
			}
			else
			{
				processGenericGotoTarget( _frames, distX, distY, m_speed );
			}

			if ( m_carryPlayer )
			{
				// spit out player

				CPlayer *player = GameScene.getPlayer();
				player->setMode( m_oldPlayerMode );
				m_carryPlayer = false;

				DVECTOR move;
				move.vx = 16 * _frames;
				move.vy = -16 * _frames;

				player->shove( move );
				player->setMoveVelocity( &move );
			}

			break;
		}
	}

	if ( m_carryPlayer )
	{
		CPlayer *player = GameScene.getPlayer();

		player->setPos( Pos );
	}


	/*if ( !m_animPlaying )
	{
		if ( playerXDistSqr + playerYDistSqr < 100 && !m_salvoCount )
		{
			m_animPlaying = true;
			m_animNo = ANIM_SHARKSUB_SWIPE;
			m_frame = 0;
		}
		else
		{
			m_animPlaying = true;
			m_animNo = ANIM_SHARKSUB_SWIM;
			m_frame = 0;
		}
	}

	if ( m_timerTimer <= 0 )
	{
		if ( m_salvoCount > 0 )
		{
			// drop mine

			CProjectile *projectile;
			projectile = CProjectile::Create();
			projectile->init( Pos, 1024, CProjectile::PROJECTILE_MINE, CProjectile::PROJECTILE_FINITE_LIFE );
			projectile->setGraphic( FRM__SHARKMINE );

			m_salvoCount--;

			m_timerTimer = ( GameState::getOneSecondInFrames() >> 2 ) * ( 1 + ( ( 3 * m_health ) / m_data[m_type].initHealth ) );
		}
	}

	if ( m_movementTimer > 0 )
	{
		m_movementTimer -= _frames;
	}

	s32 moveX = 0, moveY = 0;
	s32 moveVel = 0;
	s32 moveDist = 0;

	processGenericFixedPathMove( _frames, &moveX, &moveY, &moveVel, &moveDist );

	if ( moveX > 0 )
	{
		m_extendDir = EXTEND_RIGHT;
	}
	else
	{
		m_extendDir = EXTEND_LEFT;
	}

	Pos.vx += moveX;
	Pos.vy += moveY;

	if ( m_movementTimer <= 0 && m_salvoCount < 1 )
	{
		m_controlFunc = NPC_CONTROL_CLOSE;
	}*/



	/*if ( m_extendDir == EXTEND_RIGHT )
	{
		s32 xDist = 600 - Pos.vx;
		s32 xDistSqr = xDist * xDist;
		s32 yDist = m_base.vy - Pos.vy;
		s32 yDistSqr = yDist * yDist;

		if ( ( xDistSqr + yDistSqr ) > 100 )
		{
			processGenericGotoTarget( _frames, xDist, yDist, m_speed );
		}
		else
		{
			m_extendDir = EXTEND_LEFT;

			if ( m_movementTimer <= 0 )
			{
				m_controlFunc = NPC_CONTROL_CLOSE;
			}
		}
	}
	else
	{
		s32 xDist = 100 - Pos.vx;
		s32 xDistSqr = xDist * xDist;
		s32 yDist = m_base.vy - Pos.vy;
		s32 yDistSqr = yDist * yDist;

		if ( ( xDistSqr + yDistSqr ) > 100 )
		{
			processGenericGotoTarget( _frames, xDist, yDist, m_speed );
		}
		else
		{
			m_extendDir = EXTEND_RIGHT;

			if ( m_movementTimer <= 0 )
			{
				m_controlFunc = NPC_CONTROL_CLOSE;
			}
		}
	}*/
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*void CNpcSubSharkEnemy::processClose( int _frames )
{
	if ( m_state != SUB_SHARK_SWALLOW )
	{
		if ( playerXDist > 0 )
		{
			m_extendDir = EXTEND_RIGHT;
		}
		else
		{
			m_extendDir = EXTEND_LEFT;
		}
	}

	switch( m_state )
	{
		case SUB_SHARK_MINE_1:
		case SUB_SHARK_MINE_2:
		{
			if ( !m_animPlaying )
			{
				m_animPlaying = true;
				m_animNo = ANIM_SHARKSUB_SWIM;
				m_frame = 0;
			}

			processGenericGotoTarget( _frames, playerXDist, 0, m_speed );

			s32 minX, maxX;
			m_npcPath.getPathXExtents( &minX, &maxX );

			if ( Pos.vx < minX || Pos.vx > maxX || playerXDistSqr < 100 )
			{
				// fire at player

				m_salvoCount = 5;
				m_state++;
				m_movementTimer = GameState::getOneSecondInFrames() * ( 1 + ( ( 7 * m_health ) / m_data[m_type].initHealth ) );
				m_controlFunc = NPC_CONTROL_MOVEMENT;
			}

			break;
		}

		case SUB_SHARK_CYCLE:
		{
			// charge player

			if ( !m_animPlaying )
			{
				m_animPlaying = true;
				m_animNo = ANIM_SHARKSUB_SPRINTOPEN;
				m_frame = 0;
			}

			processGenericGotoTarget( _frames, playerXDist, 0, NPC_SUB_SHARK_HIGH_SPEED );

			s32 minX, maxX;
			m_npcPath.getPathXExtents( &minX, &maxX );

			if ( Pos.vx < minX || Pos.vx > maxX || playerXDistSqr < 10000 )
			{
				m_animPlaying = true;
				m_animNo = ANIM_SHARKSUB_CHOMP;
				m_frame = 0;

				m_state = SUB_SHARK_SWALLOW;
			}

			break;
		}

		case SUB_SHARK_SWALLOW:
		{
			// if ( collision )
			// else

			s32 minX, maxX;
			m_npcPath.getPathXExtents( &minX, &maxX );

			if ( m_extendDir == EXTEND_RIGHT )
			{
				//s32 xDist = 600 - Pos.vx;
				s32 xDist = maxX - Pos.vx;
				s32 xDistSqr = xDist * xDist;

				if ( xDistSqr > 100 )
				{
					processGenericGotoTarget( _frames, xDist, 0, NPC_SUB_SHARK_HIGH_SPEED );
				}
				else
				{
					m_extendDir = EXTEND_LEFT;
				}
			}
			else
			{
				//s32 xDist = 100 - Pos.vx;
				s32 xDist = minX - Pos.vx;
				s32 xDistSqr = xDist * xDist;

				if ( xDistSqr > 100 )
				{
					processGenericGotoTarget( _frames, xDist, 0, NPC_SUB_SHARK_HIGH_SPEED );
				}
				else
				{
					m_extendDir = EXTEND_RIGHT;
				}
			}

			if ( !m_animPlaying )
			{
				m_animPlaying = true;
				m_animNo = ANIM_SHARKSUB_SWIM;
				m_frame = 0;
				m_controlFunc = NPC_CONTROL_MOVEMENT;
				m_movementTimer = GameState::getOneSecondInFrames() * ( 1 + ( ( 7 * m_health ) / m_data[m_type].initHealth ) );
				m_state = SUB_SHARK_MINE_1;
			}

			break;
		}
	}
}*/

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcSubSharkEnemy::processShot( int _frames )
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
			if ( m_carryPlayer )
			{
				// spit out player

				CPlayer *player = GameScene.getPlayer();
				player->setMode( m_oldPlayerMode );
				m_carryPlayer = false;

				DVECTOR move;
				move.vx = 16 * _frames;
				move.vy = -16 * _frames;

				player->shove( move );
				player->setMoveVelocity( &move );
			}

			switch ( m_state )
			{
				case NPC_GENERIC_HIT_CHECK_HEALTH:
				{
					m_health--;

					if ( m_health <= 0 )
					{
						m_state = NPC_GENERIC_HIT_DEATH_START;
						m_isDying = true;
						CGameScene::setBossHasBeenKilled();
					}
					else
					{
						m_state = NPC_GENERIC_HIT_RECOIL;

						m_animPlaying = true;
						m_animNo = m_data[m_type].recoilAnim;
						m_frame = 0;
						m_speed = m_data[m_type].speed + ( ( 3 * ( m_data[m_type].initHealth - m_health ) ) / m_data[m_type].initHealth );
					}

					break;
				}

				case NPC_GENERIC_HIT_RECOIL:
				{
					m_invulnerableTimer = 2 * GameState::getOneSecondInFrames();

					if ( !m_animPlaying )
					{
						m_state = 0;
						m_controlFunc = NPC_CONTROL_MOVEMENT;
					}

					break;
				}

				case NPC_GENERIC_HIT_DEATH_START:
				{
					CNpcEnemy::processShotDeathStart( _frames );

					break;
				}

				case NPC_GENERIC_HIT_DEATH_END:
				{
					if ( !m_animPlaying )
					{
						CNpcEnemy::processShotDeathEnd( _frames );
					}

					break;
				}
			}

			break;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcSubSharkEnemy::shutdown()
{
	CLevel::setIsBossRespawn( true );

	if ( m_state != NPC_GENERIC_HIT_DEATH_END )
	{
		CLevel::setBossHealth( m_health );
	}
	else
	{
		CLevel::setBossHealth( 0 );
	}

	CNpcBossEnemy::shutdown();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcSubSharkEnemy::render()
{
	SprFrame = NULL;

	if ( m_isActive )
	{
		CEnemyThing::render();

		if (canRender())
		{
			DVECTOR &renderPos=getRenderPos();

			SprFrame = m_actorGfx->Render(renderPos,m_animNo,( m_frame >> 8 ),m_reversed);
			m_actorGfx->RotateScale( SprFrame, renderPos, 0, 4096, 4096, true );

			sBBox boundingBox = m_actorGfx->GetBBox();
			setCollisionSize( ( boundingBox.XMax - boundingBox.XMin ), ( boundingBox.YMax - boundingBox.YMin ) );
			setCollisionCentreOffset( ( boundingBox.XMax + boundingBox.XMin ) >> 1, ( boundingBox.YMax + boundingBox.YMin ) >> 1 );
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcSubSharkEnemy::renderOnMapScreen( DVECTOR drawPos )
{
	drawPos.vy += 50;

	SprFrame = NULL;

	SprFrame = m_actorGfx->Render(drawPos,3,0,false);
	//m_actorGfx->RotateScale( SprFrame, renderPos, 0, 4096, 4096 );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcSubSharkEnemy::collidedWith(CThing *_thisThing)
{
	if ( m_isActive && !m_isCaught && !m_isDying )
	{
		switch(_thisThing->getThingType())
		{
			case TYPE_PLAYER:
			{
				CPlayer *player = (CPlayer *) _thisThing;

				ATTACK_STATE playerState = player->getAttackState();

				if(playerState==ATTACK_STATE__NONE)
				{
					if ( !player->isRecoveringFromHit() )
					{
						CPlayer *player = GameScene.getPlayer();

						if ( !player->isRecoveringFromHit() && !m_carryPlayer )
						{
							player->takeDamage( m_data[m_type].damageToUserType,REACT__GET_DIRECTION_FROM_THING,(CThing*)this );
						}

						if ( m_state == SUB_SHARK_CHARGE &&
								player->getMode() != PLAYER_MODE_SWALLOW &&
								player->getMode() != PLAYER_MODE_DEAD )
						{
							m_carryPlayer = true;
							m_oldPlayerMode = player->getMode();
							player->setMode( PLAYER_MODE_SWALLOW );
						}
					}
				}
				else if ( m_invulnerableTimer <= 0 )
				{
					// player is attacking, respond appropriately

					if ( m_controlFunc != NPC_CONTROL_SHOT )
					{
						if(playerState==ATTACK_STATE__BUTT_BOUNCE)
						{
							player->justButtBouncedABadGuy();
						}
						m_controlFunc = NPC_CONTROL_SHOT;
						m_state = NPC_GENERIC_HIT_CHECK_HEALTH;

						drawAttackEffect();
					}
				}

				break;
			}

			case TYPE_ENEMY:
			{
				CNpcEnemy *enemy = (CNpcEnemy *) _thisThing;

				if ( canCollideWithEnemy() && enemy->canCollideWithEnemy() )
				{
					processEnemyCollision( _thisThing );
				}

				break;
			}

			default:
				ASSERT(0);
				break;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

s32 CNpcSubSharkEnemy::getFrameShift( int _frames )
{
	if ( m_state == SUB_SHARK_START_CHARGE )
	{
		return( ( _frames << 8 ) << 1 );
	}
	else
	{
		return( ( _frames << 8 ) >> 1 );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcSubSharkEnemy::setupWaypoints( sThingActor *ThisActor )
{
	u16	*PntList=(u16*)MakePtr(ThisActor,sizeof(sThingActor));

	u16 newXPos, newYPos;
	u16 startXPos, startYPos;

	m_npcPath.setWaypointCount( ThisActor->PointCount - 1 );

	startXPos = newXPos = (u16) *PntList;
	setWaypointPtr( PntList );
	PntList++;
	startYPos = newYPos = (u16) *PntList;
	PntList++;

	setStartPos( newXPos, newYPos );

	if ( ThisActor->PointCount > 1 )
	{
		newXPos = (u16) *PntList;
		PntList++;
		newYPos = (u16) *PntList;
		PntList++;

		setHeading( newXPos, newYPos, startXPos, startYPos );
	}

	s32 minX, maxX, minY, maxY;

	m_npcPath.getPathXExtents( &minX, &maxX );
	m_npcPath.getPathYExtents( &minY, &maxY );

	m_thinkArea.x1 = minX;
	m_thinkArea.x2 = maxX;
	m_thinkArea.y1 = minY;
	m_thinkArea.y2 = maxY;

	m_npcPath.setWaypointCount( ThisActor->PointCount - 2 );
}
