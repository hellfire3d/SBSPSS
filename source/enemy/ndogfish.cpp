/*=========================================================================

	ndogfish.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef __ENEMY_NPC_H__
#include "enemy\npc.h"
#endif

#ifndef	__ENEMY_NDOGFISH_H__
#include "enemy\ndogfish.h"
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

#ifndef __ANIM_IRONDOGFISH_HEADER__
#include <ACTOR_IRONDOGFISH_ANIM.h>
#endif

#ifndef __VID_HEADER_
#include "system\vid.h"
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcIronDogfishEnemy::postInit()
{
	m_state = IRON_DOGFISH_THUMP_1;
	m_extendDir = EXTEND_RIGHT;
	m_npcPath.setPathType( CNpcPath::PONG_PATH );
	m_steamTimer = 0;
	m_vulnerableTimer = 0;

	if ( CLevel::getIsBossRespawn() )
	{
		m_health = CLevel::getBossHealth();
		//m_speed = m_data[m_type].speed + ( ( 3 * ( m_data[m_type].initHealth - m_health ) ) / m_data[m_type].initHealth );
	}

	CNpcBossEnemy::postInit();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool CNpcIronDogfishEnemy::processSensor()
{
	switch( m_sensorFunc )
	{
		case NPC_SENSOR_NONE:
			return( false );

		default:
		{
			if ( m_movementTimer > 0 && playerXDistSqr + playerYDistSqr < 5000 )
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

void CNpcIronDogfishEnemy::processMovement( int _frames )
{
	if ( m_vulnerableTimer > 0 )
	{
		m_vulnerableTimer -= _frames;

		if ( m_animNo != ANIM_IRONDOGFISH_IDLE )
		{
			m_animPlaying = true;
			m_animNo = ANIM_IRONDOGFISH_IDLE;
			m_frame = 0;
		}
	}
	else
	{
		s32 moveX = 0, moveY = 0;
		s32 moveVel = 0;
		s32 moveDist = 0;

		if ( m_movementTimer > 0 )
		{
			if ( m_soundId == NOT_PLAYING && m_data[m_type].moveSfx < CSoundMediator::NUM_SFXIDS )
			{
				m_soundId = (int) CSoundMediator::playSfx( m_data[m_type].moveSfx, true );
			}

			if ( m_animNo != ANIM_IRONDOGFISH_WALK || !m_animPlaying )
			{
				m_animPlaying = true;
				m_animNo = ANIM_IRONDOGFISH_WALK;
				m_frame = 0;
			}

			processGenericFixedPathWalk( _frames, &moveX, &moveY );

			Pos.vx += moveX;
			Pos.vy += moveY;

			m_movementTimer -= _frames;
		}
		else
		{
			processStandardIronDogfishAttack( _frames );
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcIronDogfishEnemy::processWalkToUser( int _frames, int speed )
{
	s32 minX, maxX;
	m_npcPath.getPathXExtents( &minX, &maxX );

	if ( playerXDist > 0 )
	{
		m_heading = 0;
		Pos.vx += _frames * speed;
	}
	else
	{
		m_heading = 2048;
		Pos.vx -= _frames * speed;
	}

	s32 fallSpeed = 3;
	s8 yMovement = fallSpeed * _frames;
	s32 groundHeight = CGameScene::getCollision()->getHeightFromGround( Pos.vx, Pos.vy, yMovement + 16 );

	if ( groundHeight <= yMovement )
	{
		// move to ground level

		Pos.vx += groundHeight;
	}
	else
	{
		// fall

		Pos.vx += yMovement;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcIronDogfishEnemy::processStandardIronDogfishAttack( int _frames )
{
	if ( m_state != IRON_DOGFISH_LASER_EYE_1_WAIT && m_state != IRON_DOGFISH_LASER_EYE_2_WAIT )
	{
		if ( playerXDist > 0 )
		{
			m_extendDir = EXTEND_RIGHT;
			m_heading = 0;
		}
		else
		{
			m_extendDir = EXTEND_LEFT;
			m_heading = 2048;
		}
	}

	switch( m_state )
	{
		case IRON_DOGFISH_THUMP_1:
		case IRON_DOGFISH_THUMP_2:
		{
			s32 minX, maxX;
			m_npcPath.getPathXExtents( &minX, &maxX );

			u8 thump = false;

			if ( playerXDist > 0 )
			{
				if ( Pos.vx + playerXDist > maxX )
				{
					thump = true;
				}
			}
			else
			{
				if ( Pos.vx + playerXDist < minX )
				{
					thump = true;
				}
			}

			if ( playerXDistSqr > 100 && !thump )
			{
				if ( !m_animPlaying )
				{
					m_animPlaying = true;
					m_animNo = ANIM_IRONDOGFISH_WALK;
					m_frame = 0;
				}

				processWalkToUser( _frames, m_speed );
			}
			else
			{
				// thump player

				if ( m_animNo != ANIM_IRONDOGFISH_PUNCH )
				{
					m_animPlaying = true;
					m_animNo = ANIM_IRONDOGFISH_PUNCH;
					m_frame = 0;

					CSoundMediator::playSfx( CSoundMediator::SFX_DOGFISH_THUMP );
				}
				else if ( !m_animPlaying )
				{
					m_state++;
					m_movementTimer = GameState::getOneSecondInFrames() * 3;
				}
			}

			break;
		}

		case IRON_DOGFISH_LASER_EYE_1:
		case IRON_DOGFISH_LASER_EYE_2:
		{
			if ( playerYDistSqr > 50000 )
			{
				m_movementTimer = GameState::getOneSecondInFrames() * 3;
				m_state = IRON_DOGFISH_THUMP_2;

				break;
			}

			if ( m_animNo != ANIM_IRONDOGFISH_IDLE || !m_animPlaying )
			{
				m_animPlaying = true;
				m_animNo = ANIM_IRONDOGFISH_IDLE;
				m_frame = 0;
			}

			// fire at user

			s16 headingToPlayer = ratan2( playerYDist, playerXDist ) & 4095;

			CProjectile *projectile;
			projectile = CProjectile::Create();
			DVECTOR startPos = Pos;
			if ( m_heading == 0 )
			{
				startPos.vx += 60;
			}
			else
			{
				startPos.vx -= 60;
			}
			startPos.vy -= 45;

			projectile->init( startPos, headingToPlayer );
			projectile->setSpeed( 6 );

			m_laserTimer = GameState::getOneSecondInFrames() >> 1;

			CSoundMediator::playSfx( CSoundMediator::SFX_DOGFISH_LASER );
			/*m_effect = (CFXLaser*) CFX::Create( CFX::FX_TYPE_LASER, this );

			DVECTOR offsetPos;
			if ( m_heading == 0 )
			{
				offsetPos.vx = 60;
			}
			else
			{
				offsetPos.vx = -60;
			}
			offsetPos.vy = -45;
			m_effect->setOffset( offsetPos );

			DVECTOR targetPos = GameScene.getPlayer()->getPos();
			targetPos.vy -= 45;
			m_effect->setTarget( targetPos );

			m_effect->setRGB( 255, 0, 0 );*/

			m_state++;

			break;
		}

		case IRON_DOGFISH_LASER_EYE_1_WAIT:
		case IRON_DOGFISH_LASER_EYE_2_WAIT:
		{
			if ( !m_animPlaying )
			{
				m_animPlaying = true;
				m_animNo = ANIM_IRONDOGFISH_IDLE;
				m_frame = 0;
			}

			if ( m_laserTimer > 0 )
			{
				m_laserTimer -= _frames;
			}
			else
			{
				//m_effect->killFX();

				m_state++;

				if ( m_state == IRON_DOGFISH_LASER_EYE_2_WAIT )
				{
					m_movementTimer = GameState::getOneSecondInFrames() * 3;
				}
			}

			break;
		}

		case IRON_DOGFISH_ROLL:
		{
			s32 minX, maxX;
			m_npcPath.getPathXExtents( &minX, &maxX );

			u8 thump = false;

			if ( playerXDist > 0 )
			{
				if ( Pos.vx + playerXDist > maxX )
				{
					thump = true;
				}
			}
			else
			{
				if ( Pos.vx + playerXDist < minX )
				{
					thump = true;
				}
			}

			if ( playerXDistSqr > 100 && !thump )
			{
				if ( !m_animPlaying )
				{
					m_animPlaying = true;
					m_animNo = ANIM_IRONDOGFISH_WALK;
					m_frame = 0;
				}

				processWalkToUser( _frames, m_speed );
			}
			else
			{
				if ( m_animNo != ANIM_IRONDOGFISH_TAILSMASH )
				{
					m_animPlaying = true;
					m_animNo = ANIM_IRONDOGFISH_TAILSMASH;
					m_frame = 0;

					CSoundMediator::playSfx( CSoundMediator::SFX_DOGFISH_GROWL );
				}
				else if ( !m_animPlaying )
				{
					m_movementTimer = GameState::getOneSecondInFrames() * 3;

					// return to first state

					m_state = IRON_DOGFISH_THUMP_1;
				}
			}

			break;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcIronDogfishEnemy::processClose( int _frames )
{
	// swipe at player

	if ( m_animNo != ANIM_IRONDOGFISH_PUNCH )
	{
		m_animPlaying = true;
		m_animNo = ANIM_IRONDOGFISH_PUNCH;
		m_frame = 0;
	}
	else if ( !m_animPlaying )
	{
		m_controlFunc = NPC_CONTROL_MOVEMENT;
		m_timerFunc = NPC_TIMER_ATTACK_DONE;
		m_timerTimer = GameState::getOneSecondInFrames();
		m_sensorFunc = NPC_SENSOR_NONE;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcIronDogfishEnemy::processAttackCollision()
{
	switch( m_animNo )
	{
		case ANIM_IRONDOGFISH_PUNCH:
		case ANIM_IRONDOGFISH_TAILSMASH:
		{
			// only detect collision if in attack mode

			if ( m_controlFunc != NPC_CONTROL_COLLISION )
			{
				m_oldControlFunc = m_controlFunc;
				m_controlFunc = NPC_CONTROL_COLLISION;
			}

			break;
		}

		default:
			break;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcIronDogfishEnemy::hasBeenSteamed( DVECTOR &steamPos )
{
	if ( m_steamTimer <= 0 )
	{
		m_controlFunc = NPC_CONTROL_MOVEMENT;

		s16 second = GameState::getOneSecondInFrames();
		m_vulnerableTimer = ( 2 * second ) + ( 2 * second * ( ( m_data[m_type].initHealth - m_health ) / m_data[m_type].initHealth ) );

		//hasBeenAttacked();
		m_steamTimer = 4 * GameState::getOneSecondInFrames();
		//m_effect->killFX();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcIronDogfishEnemy::processTimer(int _frames)
{
	if ( m_steamTimer > 0 )
	{
		m_steamTimer -= _frames;
	}

	CNpcEnemy::processTimer( _frames );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcIronDogfishEnemy::processShotDeathEnd( int _frames )
{
	if ( !m_animPlaying )
	{
		CNpcEnemy::processShotDeathEnd( _frames );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcIronDogfishEnemy::collidedWith( CThing *_thisThing )
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
						switch( m_data[m_type].detectCollision )
						{
							case DETECT_NO_COLLISION:
							{
								// ignore

								break;
							}

							case DETECT_ALL_COLLISION:
							{
								if ( m_controlFunc != NPC_CONTROL_COLLISION )
								{
									m_oldControlFunc = m_controlFunc;
									m_controlFunc = NPC_CONTROL_COLLISION;
								}

								break;
							}

							case DETECT_ATTACK_COLLISION_GENERIC:
							{
								processAttackCollision();

								break;
							}
						}
					}
				}
				else if ( m_vulnerableTimer > 0 && m_invulnerableTimer <= 0 )
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
					//processEnemyCollision( _thisThing );
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

void CNpcIronDogfishEnemy::shutdown()
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

void CNpcIronDogfishEnemy::render()
{
	SprFrame = NULL;

	if ( m_isActive )
	{
		CEnemyThing::render();

		if (canRender())
		{
			DVECTOR &renderPos=getRenderPos();

			SprFrame = m_actorGfx->Render(renderPos,m_animNo,( m_frame >> 8 ),m_reversed);
			m_actorGfx->RotateScale( SprFrame, renderPos, 0, 4096, 4096 );

			if ( m_vulnerableTimer > 0 && m_invulnerableTimer <= 0 )
			{
				setShadeTex(SprFrame,0);
				setRGB0( SprFrame, 255, 255, 255 );
			}

			sBBox boundingBox = m_actorGfx->GetBBox();
			setCollisionSize( ( boundingBox.XMax - boundingBox.XMin ), ( boundingBox.YMax - boundingBox.YMin ) );
			setCollisionCentreOffset( ( boundingBox.XMax + boundingBox.XMin ) >> 1, ( boundingBox.YMax + boundingBox.YMin ) >> 1 );
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcIronDogfishEnemy::renderOnMapScreen( DVECTOR drawPos )
{
	drawPos.vy += 10;

	SprFrame = NULL;

	SprFrame = m_actorGfx->Render(drawPos,0,0,false);

	// initialise some stuff so that boss is always to left when player reads description
	m_state = IRON_DOGFISH_THUMP_1;
	m_extendDir = EXTEND_RIGHT;
	m_steamTimer = 0;
	m_vulnerableTimer = 0;

	s32 xMin, xMax;
	m_npcPath.getPathXExtents( &xMin, &xMax );
	Pos.vx = xMin;
	m_npcPath.resetPath();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcIronDogfishEnemy::processCollision()
{
	CPlayer *player = GameScene.getPlayer();
	player->takeDamage( m_data[m_type].damageToUserType,REACT__GET_DIRECTION_FROM_THING,(CThing*)this );
	m_controlFunc = m_oldControlFunc;
}
