/*=========================================================================

	fgary.cpp

	Author:		CRB
	Created:
	Project:	Spongebob
	Purpose:

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef __FRIEND_FGARY_H__
#include "friend\fgary.h"
#endif

#ifndef __GAME_GAME_H__
#include "game\game.h"
#endif

#ifndef	__UTILS_HEADER__
#include	"utils\utils.h"
#endif

#ifndef	__PLAYER_PLAYER_H__
#include	"player\player.h"
#endif

#ifndef __VID_HEADER_
#include "system\vid.h"
#endif

#ifndef	__GAME_CONVO_H__
#include "game\convo.h"
#endif


#ifndef __ANIM_GARY_HEADER__
#include <ACTOR_GARY_Anim.h>
#endif

#define GARY_SPEED		1


u8 CNpcGaryFriend::m_garySpeech;
u8 CNpcGaryFriend::m_hasReachedDoor;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcGaryFriend::postInit()
{
	CNpcFriend::postInit();

	m_started = false;
	m_startMoving = false;
	m_fallDeath = false;
	m_drawRotation = 0;
	m_obstructed = false;
	m_garySB = false;
	m_garyMeow = false;

	m_soundId = (int) NOT_PLAYING;

	setCollisionSize( 40, 27 );
	setCollisionCentreOffset( 0, -14 );

	m_garySpeech = false;
	m_isStopping = false;

	CNpcGaryFriend::clearReachedDoor();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcGaryFriend::shutdown()
{
	if ( m_soundId != NOT_PLAYING )
	{
		CSoundMediator::stopAndUnlockSfx( (xmPlayingId) m_soundId );
	}

	CNpcFriend::shutdown();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcGaryFriend::think( int _frames )
{
	if ( m_soundId != NOT_PLAYING )
	{
		if( !CSoundMediator::isSfxStillPlaying( (xmPlayingId) m_soundId ) )
		{
			// unlock sound if it has finished

			CSoundMediator::stopAndUnlockSfx( (xmPlayingId) m_soundId );
			m_soundId = NOT_PLAYING;
		}
	}

	if ( m_garySB && !m_startMoving )
	{
		if ( !CSoundMediator::isSpeechPlaying() )
		{
			if ( m_garyMeow )
			{
				m_startMoving = true;
			}
			else
			{
				m_garyMeow = true;
				CSoundMediator::playSpeech( SPEECH_029 );
			}
		}
	}

	if ( m_animPlaying )
	{
		s32 frameCount;

		frameCount = m_actorGfx->getFrameCount( m_animNo );

		s32 frameShift = ( _frames << 8 ) >> 1;

		if ( ( frameCount << 8 ) - m_frame > frameShift )
		{
			m_frame += frameShift;
		}
		else
		{
			m_frame = ( frameCount - 1 ) << 8;
			m_animPlaying = false;
		}
	}
	else
	{
		if ( m_startMoving )
		{
			m_animNo = ANIM_GARY_SLITHER;
		}
		else
		{
			m_animNo = m_data[m_type].idleAnim;
		}

		m_animPlaying = true;
		m_frame = 0;
	}

	if ( m_fallDeath )
	{
		m_drawRotation += 64 * _frames;
		m_drawRotation &= 4095;

		Pos.vy += m_speed * _frames;

		if ( m_speed < 5 )
		{
			m_speed++;
		}

		DVECTOR const &offset = CLevel::getCameraPos();

		if ( Pos.vy - offset.vy > VidGetScrH() )
		{
			setToShutdown();
		}
	}
	else
	{
		s8 multiplier = -1 + ( 2 * m_extension );
		s32 maxHeight = 20;
		s32 fallSpeed = 3;
		s8 yMovement = fallSpeed * _frames;
		s8 groundHeight;
		u8 conveyorOverride = false;

		// check vertical collision

		groundHeight = CGameScene::getCollision()->getHeightFromGroundNonSB( Pos.vx, Pos.vy, yMovement + 16 );

		switch ( CGameScene::getCollision()->getCollisionBlock( Pos.vx, Pos.vy ) & COLLISION_TYPE_MASK )
		{
			case COLLISION_TYPE_FLAG_DEATH_FALL:
			case COLLISION_TYPE_FLAG_DEATH_INSTANT:
			case COLLISION_TYPE_FLAG_DEATH_LIQUID:
			case COLLISION_TYPE_FLAG_DAMAGE:
			{
				CPlayer *player = GameScene.getPlayer();
				player->dieYouPorousFreak();

				m_speed = -5;
				m_fallDeath = true;

				break;
			}

			case COLLISION_TYPE_FLAG_MOVE_LEFT:
			{
				conveyorOverride = true;
				int Time = GameState::getFramesSinceLast();

				Pos.vx -= Time;
				Pos.vx -= _frames;

				break;
			}

			case COLLISION_TYPE_FLAG_MOVE_RIGHT:
			{
				conveyorOverride = true;
				int Time = GameState::getFramesSinceLast();

				Pos.vx += Time;
				Pos.vx += _frames;

				break;
			}

			default:
				break;
		}

		if ( m_platform )
		{
			s32 platformHeight = m_platform->getHeightFromPlatformAtPosition( Pos.vx, Pos.vy );

			if ( platformHeight < groundHeight )
			{
				//groundHeight = platformHeight;
				groundHeight = 0;

				Pos.vy += platformHeight;
			}
			//return;
		}

		int garyXMovement = multiplier * GARY_SPEED * _frames;
		
		if ( m_isStopping )
		{
			int garyRequiredXMovement = m_xStopPos - Pos.vx;

			if ( garyRequiredXMovement > 0 )
			{
				if ( garyXMovement > garyRequiredXMovement )
				{
					garyXMovement = garyRequiredXMovement;
				}
			}
			else if ( garyRequiredXMovement < 0 )
			{
				if ( garyXMovement < garyRequiredXMovement )
				{
					garyXMovement = garyRequiredXMovement;
				}
			}
			else
			{
				m_isStopping = false;
				m_startMoving = false;
				m_started = false;
				m_animNo = m_data[m_type].idleAnim;
				m_animPlaying = true;
				m_frame = 0;
				m_garyMeow = false;
				m_garySB = false;
			}
		}

		if ( groundHeight <= 0 )
		{
			// groundHeight <= 0  indicates either on ground or below ground

			// check horizontal collision

			if ( CGameScene::getCollision()->getHeightFromGroundNonSB( Pos.vx + garyXMovement, Pos.vy ) < -maxHeight )
			{
				// reverse direction

				m_extension = !m_extension;
				m_reversed = !m_reversed;
			}
			else
			{
				// make sure we are on the ground, not below it

				Pos.vy += groundHeight;

				if ( m_startMoving )
				{
					if ( canRender() && m_soundId == NOT_PLAYING )
					{
						m_soundId = (int) CSoundMediator::playSfx( CSoundMediator::SFX_GARY_DE_SNAIL, true );
					}

					if ( !conveyorOverride && !m_obstructed )
					{
						Pos.vx += garyXMovement;
					}
				}
			}
		}
		else
		{
			// above ground

			if ( groundHeight < yMovement )
			{
				// colliding with ground

				Pos.vy += groundHeight;

				if ( CGameScene::getCollision()->getHeightFromGroundNonSB( Pos.vx + garyXMovement, Pos.vy ) < -maxHeight )
				{
					// reverse direction

					m_extension = !m_extension;
				}
				else
				{
					if ( m_startMoving )
					{
						if ( m_soundId == NOT_PLAYING )
						{
							m_soundId = (int) CSoundMediator::playSfx( CSoundMediator::SFX_GARY_DE_SNAIL, true );
						}

						if ( !conveyorOverride && !m_obstructed )
						{
							Pos.vx += garyXMovement;
						}
					}
				}
			}
			else
			{
				Pos.vy += yMovement;
			}
		}
	}

	m_obstructed = false;

	// sort out draw rotation

	DVECTOR testPos1, testPos2;

	testPos1 = testPos2 = Pos;
	testPos1.vx -= 10;
	testPos2.vx += 10;

	int groundDist = CGameScene::getCollision()->getHeightFromGround( testPos1.vx, testPos1.vy, 16 );

	if ( abs( groundDist ) > 15 )
	{
		m_drawRotation = 0;
		CNpcThing::think(_frames);
		return;
	}

	testPos1.vy += groundDist;

	groundDist = CGameScene::getCollision()->getHeightFromGround( testPos2.vx, testPos2.vy, 16 );

	if ( abs( groundDist ) > 15 )
	{
		m_drawRotation = 0;
		CNpcThing::think(_frames);
		return;
	}

	testPos2.vy += groundDist;

	s32 xDist = testPos2.vx - testPos1.vx;
	s32 yDist = testPos2.vy - testPos1.vy;

	s16 heading = ratan2( yDist, xDist );

	m_drawRotation = heading;

	CNpcThing::think(_frames);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcGaryFriend::setupWaypoints( sThingActor *ThisActor )
{
	if ( ThisActor->PointCount > 1 )
	{
		u16	*PntList=(u16*)MakePtr(ThisActor,sizeof(sThingActor));

		u16 newXPos, newYPos;

		// skip first waypoint
		
		PntList++;
		PntList++;

		// get trigger position

		newXPos = (u16) *PntList;
		PntList++;
		newYPos = (u16) *PntList;
		PntList++;

		m_triggerPos.vx = newXPos;
		m_triggerPos.vy = newYPos;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcGaryFriend::render()
{
	CNpcThing::render();

	POLY_FT4 *frame;

// Render

	if (canRender())
	{
		DVECTOR &renderPos=getRenderPos();

		frame = m_actorGfx->Render(renderPos,m_animNo,(m_frame>>8),m_reversed);
		m_actorGfx->RotateScale( frame, renderPos, m_drawRotation, 4096, 4096 );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcGaryFriend::startLeft()
{
	start();

	m_extension = EXTEND_LEFT;
	m_reversed = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcGaryFriend::startRight()
{
	start();

	m_extension = EXTEND_RIGHT;
	m_reversed = false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcGaryFriend::start()
{
	if ( !m_started )
	{
		m_animNo = ANIM_GARY_SLITHER;
		m_animPlaying = true;
		m_frame = 0;
		m_started = true;

		if ( !m_garySpeech )
		{
			m_garySpeech = true;
			m_garyMeow = false;
			m_garySB = true;
			CConversation::trigger(SCRIPTS_TRIGGERSPEECH_028_DAT);
		}
		else
		{
			m_startMoving = true;
			m_garySB = false;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcGaryFriend::stop( int xPos )
{
	if ( m_started && !m_isStopping )
	{
		m_isStopping = true;
		m_xStopPos = xPos;
	}
}
