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


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcGaryFriend::postInit()
{
	CNpcFriend::postInit();

	m_started = false;
	m_fallDeath = false;
	m_drawRotation = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcGaryFriend::think( int _frames )
{
	CNpcFriend::think(_frames);

	if ( m_fallDeath )
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

		// check vertical collision

		groundHeight = CGameScene::getCollision()->getHeightFromGround( Pos.vx, Pos.vy, yMovement + 16 );

		switch ( CGameScene::getCollision()->getCollisionBlock( Pos.vx, Pos.vy ) & COLLISION_TYPE_MASK )
		{
			case COLLISION_TYPE_FLAG_DEATH_FALL:
			case COLLISION_TYPE_FLAG_DEATH_INSTANT:
			case COLLISION_TYPE_FLAG_DEATH_LIQUID:
			{
				CPlayer *player = GameScene.getPlayer();
				player->takeDamage( DAMAGE__KILL_OUTRIGHT );

				m_speed = -5;
				m_fallDeath = true;

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
				groundHeight = platformHeight;
			}

			//Pos.vy += platformHeight;
			//return;
		}

		if ( groundHeight <= 0 )
		{
			// groundHeight <= 0  indicates either on ground or below ground

			// check horizontal collision

			if ( CGameScene::getCollision()->getHeightFromGround( Pos.vx + ( multiplier * _frames ), Pos.vy ) < -maxHeight )
			{
				// reverse direction

				m_extension = !m_extension;
				m_reversed = !m_reversed;
			}
			else
			{
				// make sure we are on the ground, not below it

				Pos.vy += groundHeight;

				if ( m_started )
				{
					CSoundMediator::playSfx( CSoundMediator::SFX_GARY_DE_SNAIL );

					Pos.vx += multiplier * 2 * _frames;
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

				if ( CGameScene::getCollision()->getHeightFromGround( Pos.vx + ( multiplier * _frames ), Pos.vy ) < -maxHeight )
				{
					// reverse direction

					m_extension = !m_extension;
				}
				else
				{
					if ( m_started )
					{
						CSoundMediator::playSfx( CSoundMediator::SFX_GARY_DE_SNAIL );

						Pos.vx += multiplier * 2 * _frames;
					}
				}
			}
			else
			{
				Pos.vy += yMovement;
			}
		}
	}
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

		sBBox boundingBox = m_actorGfx->GetBBox();
		boundingBox.YMax = 0;
		setCollisionSize( ( boundingBox.XMax - boundingBox.XMin ), ( boundingBox.YMax - boundingBox.YMin ) );
		setCollisionCentreOffset( ( boundingBox.XMax + boundingBox.XMin ) >> 1, ( boundingBox.YMax + boundingBox.YMin ) >> 1 );
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

const CRECT *CNpcGaryFriend::getThinkBBox()
{
	CRECT objThinkBox = getCollisionArea();

	sBBox &thinkBBox = CThingManager::getThinkBBox();
	objThinkBox.x1 = thinkBBox.XMin;
	objThinkBox.x2 = thinkBBox.XMax;
	objThinkBox.y1 = thinkBBox.YMin;
	objThinkBox.y2 = thinkBBox.YMax;

	return &objThinkBox;
}
