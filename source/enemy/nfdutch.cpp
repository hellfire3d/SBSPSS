/*=========================================================================

	nfdutch.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef __ENEMY_NPC_H__
#include "enemy\npc.h"
#endif

#ifndef	__ENEMY_NFDUTCH_H__
#include "enemy\nfdutch.h"
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

#ifndef __VID_HEADER_
#include "system\vid.h"
#endif

#ifndef	__ANIM_FLYINGDUTCHMAN_HEADER__
#include <ACTOR_FLYINGDUTCHMAN_ANIM.h>
#endif

#ifndef __SPR_SPRITES_H__
#include <sprites.h>
#endif

#ifndef	__UTILS_HEADER__
#include	"utils\utils.h"
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcFlyingDutchmanEnemy::postInit()
{
	m_extendDir = EXTEND_UP;

	s32 minX, maxX;
	m_npcPath.getPathXExtents( &minX, &maxX );
	m_npcPath.getPathYExtents( &m_minY, &m_maxY );

	m_extension = minX;
	m_inRange = false;

	if ( CLevel::getIsBossRespawn() )
	{
		m_health = CLevel::getBossHealth();
	}

	m_fireCount = 0;

	m_fadeVal = 128;
	m_fadeDown = true;

	CNpcBossEnemy::postInit();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcFlyingDutchmanEnemy::think( int _frames )
{
	if ( m_fadeDown )
	{
		if ( m_fadeVal > 0 )
		{
			m_fadeVal -= 4 * _frames;

			if ( m_fadeVal < 0 )
			{
				m_fadeVal = 0;
			}
		}
	}
	else
	{
		if ( m_fadeVal < 128 )
		{
			m_fadeVal += 4 * _frames;

			if ( m_fadeVal > 128 )
			{
				m_fadeVal = 128;
			}
		}
	}

	CNpcBossEnemy::think( _frames );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcFlyingDutchmanEnemy::processMovement( int _frames )
{
	if ( !m_animPlaying )
	{
		m_animPlaying = true;
		m_animNo = m_data[m_type].moveAnim;
		m_frame = 0;
	}

	s32 xDist = m_extension - Pos.vx;
	s32 xDistSqr = xDist * xDist;

	if ( xDistSqr > 100 )
	{
		processGenericGotoTarget( _frames, xDist, 0, m_speed );
	}
	else
	{
		if ( m_movementTimer > 0 )
		{
			m_movementTimer -= _frames;

			if ( m_extendDir == EXTEND_UP )
			{
				s32 yDist = m_minY - Pos.vy;

				if ( abs( yDist ) > 10 )
				{
					processGenericGotoTarget( _frames, 0, yDist, m_speed );
				}
				else
				{
					m_extendDir = EXTEND_DOWN;
				}
			}
			else
			{
				s32 yDist = m_maxY - Pos.vy;

				if ( abs( yDist ) > 10 )
				{
					processGenericGotoTarget( _frames, 0, yDist, m_speed );
				}
				else
				{
					m_extendDir = EXTEND_UP;
				}
			}
		}
		else
		{
			CPlayer *player = GameScene.getPlayer();

			DVECTOR const &playerPos = player->getPos();

			s32 minX, maxX;
			m_npcPath.getPathXExtents( &minX, &maxX );

			if ( playerPos.vx >= minX && playerPos.vx <= maxX &&
					playerPos.vy >= m_minY && playerPos.vy <= m_maxY )
			{
				m_controlFunc = NPC_CONTROL_CLOSE;
				m_state = FLYING_DUTCHMAN_GOTO_PLAYER;
			}
			else
			{
				m_movementTimer = GameState::getOneSecondInFrames() * 2;
			}
		}
	}

	if ( playerXDist > 0 )
	{
		m_heading = 0;
	}
	else
	{
		m_heading = 2048;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcFlyingDutchmanEnemy::processClose( int _frames )
{
	if ( playerYDist > 0 )
	{
		m_extendDir = EXTEND_DOWN;
	}
	else
	{
		m_extendDir = EXTEND_UP;
	}

	CPlayer *player = GameScene.getPlayer();

	DVECTOR const &playerPos = player->getPos();

	s32 minX, maxX;
	m_npcPath.getPathXExtents( &minX, &maxX );

	if ( playerPos.vx < minX || playerPos.vx > maxX ||
			playerPos.vy < m_minY || playerPos.vy > m_maxY )
	{
		if ( m_state != FLYING_DUTCHMAN_RETURN )
		{
			m_state = FLYING_DUTCHMAN_RETURN;
			m_timerTimer = 2 * GameState::getOneSecondInFrames();
		}
	}

	if ( !m_inRange )
	{
		if ( playerYDistSqr > 100 )
		{
			if ( !m_animPlaying )
			{
				m_animPlaying = true;
				m_animNo = m_data[m_type].moveAnim;
				m_frame = 0;
			}

			processGenericGotoTarget( _frames, 0, playerYDist, m_speed );
		}
		else
		{
			m_inRange = true;
		}
	}
	else
	{
		switch( m_state )
		{
			case FLYING_DUTCHMAN_GOTO_PLAYER:
			{
				if ( m_soundId == NOT_PLAYING )
				{
					m_soundId = (int) CSoundMediator::playSfx( m_data[m_type].moveSfx, true );
				}

				if ( !m_animPlaying )
				{
					m_animPlaying = true;
					m_animNo = m_data[m_type].moveAnim;
					m_frame = 0;
				}

				// charge player

				if ( ( playerXDistSqr + playerYDistSqr ) > 31250 )
				{
					processGenericGotoTarget( _frames, playerXDist, playerYDist, 8 );
				}
				else
				{
					int val = getRnd() % 3;

					switch( val )
					{
						case 0:
						{
							m_state = FLYING_DUTCHMAN_ATTACK_PLAYER;
							CSoundMediator::playSpeech( SPEECH_141 );

							break;
						}

						case 1:
						{
							m_state = FLYING_DUTCHMAN_CHARGE_PLAYER_START;
							CSoundMediator::playSpeech( SPEECH_142 );

							break;
						}

						case 2:
						{
							m_state = FLYING_DUTCHMAN_LEVEL_SHAKE;
							CSoundMediator::playSpeech( SPEECH_143 );

							break;
						}
					}

					m_fadeDown = false;
				}

				break;
			}

			case FLYING_DUTCHMAN_ATTACK_PLAYER:
			{
				if ( m_fadeVal == 128 )
				{
					if ( m_timerTimer > 0 )
					{
						if ( !m_animPlaying )
						{
							m_animNo = m_data[m_type].moveAnim;
							m_animPlaying = true;
							m_frame = 0;
						}

						m_timerTimer -= _frames;
					}
					else
					{
						if ( m_animNo != ANIM_FLYINGDUTCHMAN_FIREATTACK )
						{
							m_animNo = ANIM_FLYINGDUTCHMAN_FIREATTACK;
							m_animPlaying = true;
							m_frame = 0;
						}
						//else if ( !m_animPlaying )
						else if ( m_frame > ( getFrameCount() >> 1 ) )
						{
							// fire at player

							s16 heading;

							if ( playerXDist > 0 )
							{
								heading = 0;
							}
							else
							{
								heading = 2048;
							}

							CProjectile *projectile;
							projectile = CProjectile::Create();
							DVECTOR newPos = Pos;
							newPos.vy -= 50;
							projectile->init( newPos, heading );
							projectile->setGraphic( FRM__SNAKEBILE );
							projectile->setSpeed( 6 );

							m_fireCount++;

							if ( m_health < ( m_data[m_type].initHealth >> 2 ) && m_fireCount < 2 )
							{
								m_timerTimer = GameState::getOneSecondInFrames() >> 2;
							}
							else
							{
								m_state = FLYING_DUTCHMAN_RETURN;
								m_timerTimer = 2 * GameState::getOneSecondInFrames();
							}

							CSoundMediator::playSfx( CSoundMediator::SFX_FLYING_DUTCHMAN );
						}
					}
				}
				else
				{
					if ( !m_animPlaying )
					{
						m_animPlaying = true;
						m_animNo = m_data[m_type].moveAnim;
						m_frame = 0;
					}
				}

				break;
			}

			case FLYING_DUTCHMAN_CHARGE_PLAYER_START:
			{
				if ( m_fadeVal == 128 )
				{
					// charge at player

					if ( playerXDist > 0 )
					{
						m_heading = 0;
					}
					else
					{
						m_heading = 2048;
					}

					m_state = FLYING_DUTCHMAN_CHARGE_PLAYER;

					m_timerTimer = GameState::getOneSecondInFrames();
				}
				else
				{
					if ( !m_animPlaying )
					{
						m_animPlaying = true;
						m_animNo = m_data[m_type].moveAnim;
						m_frame = 0;
					}
				}

				break;
			}

			case FLYING_DUTCHMAN_CHARGE_PLAYER:
			{
				if ( m_timerTimer > 0 )
				{
					if ( m_soundId == NOT_PLAYING )
					{
						m_soundId = (int) CSoundMediator::playSfx( m_data[m_type].moveSfx, true );
					}

					if ( !m_animPlaying )
					{
						m_animNo = m_data[m_type].moveAnim;
						m_animPlaying = true;
						m_frame = 0;
					}

					if ( m_heading )
					{
						Pos.vx -= _frames * m_speed * 2;
					}
					else
					{
						Pos.vx += _frames * m_speed * 2;
					}

					m_timerTimer -= _frames;
				}
				else
				{
					m_state = FLYING_DUTCHMAN_RETURN;
				}

				break;
			}

			case FLYING_DUTCHMAN_LEVEL_SHAKE:
			{
				if ( m_fadeVal == 128 )
				{
					CThingManager::shakePlatformLoose();

					m_timerTimer = 5 * GameState::getOneSecondInFrames();

					m_state = FLYING_DUTCHMAN_LEVEL_SHAKE_WAIT;

					CGameScene::setCameraShake(0,8);
				}
				else
				{
					if ( !m_animPlaying )
					{
						m_animPlaying = true;
						m_animNo = m_data[m_type].moveAnim;
						m_frame = 0;
					}
				}

				break;
			}

			case FLYING_DUTCHMAN_LEVEL_SHAKE_WAIT:
			{
				if ( m_timerTimer > 0 )
				{
					if ( !m_animPlaying )
					{
						m_animNo = m_data[m_type].moveAnim;
						m_animPlaying = true;
						m_frame = 0;

						CGameScene::setCameraShake(0,8);
					}

					m_timerTimer -= _frames;
				}
				else
				{
					m_state = FLYING_DUTCHMAN_RETURN;
				}

				break;
			}

			case FLYING_DUTCHMAN_RETURN:
			{
				if ( m_timerTimer > 0 )
				{
					if ( !m_animPlaying )
					{
						m_animNo = m_data[m_type].moveAnim;
						m_animPlaying = true;
						m_frame = 0;
					}

					m_timerTimer -= _frames;
				}
				else
				{
					m_controlFunc = NPC_CONTROL_MOVEMENT;
					m_movementTimer = GameState::getOneSecondInFrames() * 2;
					m_inRange = false;
					m_fadeDown = true;
					m_timerTimer = 0;
					m_fireCount = 0;

					s32 minX, maxX;
					m_npcPath.getPathXExtents( &minX, &maxX );

					if ( m_extension == minX )
					{
						m_extension = maxX;
					}
					else
					{
						m_extension = minX;
					}
				}

				break;
			}
		}

		/*switch( m_state )
		{
			case FLYING_DUTCHMAN_ATTACK_PLAYER_1:
			case FLYING_DUTCHMAN_ATTACK_PLAYER_2:
			{
				if ( m_timerTimer > 0 )
				{
					if ( !m_animPlaying )
					{
						m_animNo = m_data[m_type].moveAnim;
						m_animPlaying = true;
						m_frame = 0;
					}

					m_timerTimer -= _frames;
				}
				else
				{
					if ( m_animNo != ANIM_FLYINGDUTCHMAN_FIREATTACK )
					{
						m_animNo = ANIM_FLYINGDUTCHMAN_FIREATTACK;
						m_animPlaying = true;
						m_frame = 0;
					}
					//else if ( !m_animPlaying )
					else if ( m_frame > ( getFrameCount() >> 1 ) )
					{
						// fire at player

						s16 heading;

						if ( playerXDist > 0 )
						{
							heading = 0;
						}
						else
						{
							heading = 2048;
						}

						CProjectile *projectile;
						projectile = CProjectile::Create();
						DVECTOR newPos = Pos;
						newPos.vy -= 50;
						projectile->init( newPos, heading );
						projectile->setGraphic( FRM__SNAKEBILE );
						projectile->setSpeed( 6 );

						m_fireCount++;

						if ( m_health < ( m_data[m_type].initHealth >> 2 ) && m_fireCount < 2 )
						{
							m_timerTimer = GameState::getOneSecondInFrames() >> 2;
						}
						else
						{
							m_controlFunc = NPC_CONTROL_MOVEMENT;
							m_movementTimer = GameState::getOneSecondInFrames() * 2;

							m_state++;
							m_inRange = false;
							m_timerTimer = 0;
							m_fireCount = 0;
						}
					}
				}

				break;
			}

			default:
			{
				if ( !m_animPlaying )
				{
					m_animPlaying = true;
					m_animNo = m_data[m_type].moveAnim;
					m_frame = 0;
				}

				// charge player

				if ( playerXDistSqr + playerYDistSqr > 100 )
				{
					processGenericGotoTarget( _frames, playerXDist, playerYDist, 8 );
				}
				else
				{
					m_controlFunc = NPC_CONTROL_MOVEMENT;
					m_movementTimer = GameState::getOneSecondInFrames() * 2;
					m_state = FLYING_DUTCHMAN_ATTACK_PLAYER_1;
					m_inRange = false;

					s32 minX, maxX;
					m_npcPath.getPathXExtents( &minX, &maxX );

					if ( m_extension == minX )
					{
						m_extension = maxX;
					}
					else
					{
						m_extension = minX;
					}
				}

				break;
			}
		}*/
	}

	if ( playerXDist > 0 )
	{
		m_heading = 0;
	}
	else
	{
		m_heading = 2048;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcFlyingDutchmanEnemy::processShotRecoil( int _frames )
{
	if ( !m_animPlaying )
	{
		m_controlFunc = NPC_CONTROL_MOVEMENT;
		m_movementTimer = GameState::getOneSecondInFrames() * 2;
		m_inRange = false;
		m_fadeDown = true;
		m_timerTimer = 0;
		m_fireCount = 0;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcFlyingDutchmanEnemy::processShotDeathEnd( int _frames )
{
	if ( !m_animPlaying )
	{
		CNpcEnemy::processShotDeathEnd( _frames );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcFlyingDutchmanEnemy::shutdown()
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

void CNpcFlyingDutchmanEnemy::render()
{
	SprFrame = NULL;

	if ( m_isActive )
	{
		CEnemyThing::render();

		if (canRender())
		{
			DVECTOR &renderPos=getRenderPos();

			SprFrame = m_actorGfx->Render(renderPos,m_animNo,( m_frame >> 8 ),m_reversed);
			setShadeTex(SprFrame,0);
			setSemiTrans( SprFrame, true );
			SprFrame->tpage|=1<<5;
			m_actorGfx->RotateScale( SprFrame, renderPos, 0, 5120, 5120 );
			setRGB0( SprFrame, m_fadeVal, m_fadeVal, m_fadeVal );

			sBBox boundingBox = m_actorGfx->GetBBox();

			int xSize = boundingBox.XMax - boundingBox.XMin;
			int ySize = boundingBox.YMax - boundingBox.YMin;

			setCollisionSize( xSize + ( xSize >> 2 ), ySize + ( ySize >> 2 ) );
			setCollisionCentreOffset( ( boundingBox.XMax + boundingBox.XMin ) >> 1, ( boundingBox.YMax + boundingBox.YMin ) >> 1 );
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcFlyingDutchmanEnemy::renderOnMapScreen( DVECTOR drawPos )
{
	drawPos.vy += 35;

	SprFrame = NULL;

	SprFrame = m_actorGfx->Render(drawPos,0,0,false);
	//setShadeTex(SprFrame,0);
	//setSemiTrans( SprFrame, true );
	//SprFrame->tpage|=1<<5;
	m_actorGfx->RotateScale( SprFrame, drawPos, 0, 5120, 5120 );
	//setRGB0( SprFrame, m_fadeVal, m_fadeVal, m_fadeVal );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcFlyingDutchmanEnemy::collidedWith(CThing *_thisThing)
{
	if ( m_isActive && !m_isCaught && !m_isDying )
	{
		switch(_thisThing->getThingType())
		{
			case TYPE_PLAYER:
			{
				if ( m_fadeVal == 128 )
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

									processUserCollision( _thisThing );

									break;
								}

								case DETECT_ATTACK_COLLISION_GENERIC:
								{
									processAttackCollision();
									processUserCollision( _thisThing );

									break;
								}
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

							s32 minX, maxX;
							m_npcPath.getPathXExtents( &minX, &maxX );

							if ( m_extension == minX )
							{
								m_extension = maxX;
							}
							else
							{
								m_extension = minX;
							}

							drawAttackEffect();
						}
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

u8 CNpcFlyingDutchmanEnemy::hasBeenAttacked()
{
	if ( m_fadeVal == 128 )
	{
		if ( m_controlFunc != NPC_CONTROL_SHOT )
		{
			m_controlFunc = NPC_CONTROL_SHOT;
			m_state = NPC_GENERIC_HIT_CHECK_HEALTH;

			/*m_controlFunc = NPC_CONTROL_MOVEMENT;
			m_movementTimer = GameState::getOneSecondInFrames() * 2;
			m_inRange = false;
			m_fadeDown = true;
			m_timerTimer = 0;
			m_fireCount = 0;*/

			s32 minX, maxX;
			m_npcPath.getPathXExtents( &minX, &maxX );

			if ( m_extension == minX )
			{
				m_extension = maxX;
			}
			else
			{
				m_extension = minX;
			}
		}
	}

	return( true );
}
