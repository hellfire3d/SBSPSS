/*=========================================================================

	ndustdev.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef __ENEMY_NPC_H__
#include "enemy\npc.h"
#endif

#ifndef	__ENEMY_NDUSTDEV_H__
#include "enemy\ndustdev.h"
#endif

#ifndef __GAME_GAME_H__
#include	"game\game.h"
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcDustDevilEnemy::postInit()
{
	m_npcPath.setPathType( CNpcPath::SINGLE_USE_PATH );

	m_fadeVal = 128;
	m_fadeOut = false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcDustDevilEnemy::render()
{
	SprFrame = NULL;

	if ( m_isActive )
	{
		CEnemyThing::render();

		if (canRender())
		{
			DVECTOR &renderPos=getRenderPos();

			SprFrame = m_actorGfx->Render(renderPos,m_animNo,( m_frame >> 8 ),m_reversed);
			setSemiTrans( SprFrame, true );
			m_actorGfx->RotateScale( SprFrame, renderPos, m_drawRotation, 4096, 4096 );
			setRGB0( SprFrame, m_fadeVal, m_fadeVal, m_fadeVal );

			sBBox boundingBox = m_actorGfx->GetBBox();
			setCollisionSize( ( boundingBox.XMax - boundingBox.XMin ), ( boundingBox.YMax - boundingBox.YMin ) );
			setCollisionCentreOffset( ( boundingBox.XMax + boundingBox.XMin ) >> 1, ( boundingBox.YMax + boundingBox.YMin ) >> 1 );
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcDustDevilEnemy::processMovement( int _frames )
{
	s32 maxHeight = 40;
	s32 distX, distY;
	s32 fallSpeed = 3;
	s8 yMovement = fallSpeed * _frames;
	s32 groundHeight;

	s32 moveX = 0;
	s32 moveY = 0;
	s32 moveVel = 0;
	s32 moveDist = 0;

	bool pathComplete;

	if ( m_soundId == NOT_PLAYING && m_data[m_type].moveSfx < CSoundMediator::NUM_SFXIDS )
	{
		m_soundId = (int) CSoundMediator::playSfx( m_data[m_type].moveSfx, true );
	}

	// deal with anims

	if ( !m_animPlaying )
	{
		m_animPlaying = true;
		m_frame = 0;
		m_animNo = m_data[m_type].moveAnim;
	}

	if ( m_fadeOut )
	{
		m_fadeVal -= _frames * 12;

		if ( m_fadeVal < 0 )
		{
			m_npcPath.resetPath();

			Pos = m_base;

			m_fadeOut = false;
			m_fadeVal = 128;
		}
	}
	else
	{
		// ignore y component of waypoint, since we are stuck to the ground

		if ( m_npcPath.thinkFlat( Pos, &pathComplete, &distX, &distY, &m_heading ) )
		{
			// path has finished, waypoint has changed, or there are no waypoints - do not move horizontally

			if ( pathComplete )
			{
				m_fadeOut = true;
			}
			else
			{
				// check for vertical movement

				groundHeight = CGameScene::getCollision()->getHeightFromGround( Pos.vx, Pos.vy, yMovement + 16 );

				if ( groundHeight <= yMovement )
				{
					// groundHeight <= yMovement indicates either just above ground or on or below ground

					moveY = groundHeight;
				}
				else
				{
					// fall

					moveY = yMovement;
				}
			}
		}
		else
		{
			// check for collision

			distX = distX / abs( distX );

			if ( CGameScene::getCollision()->getHeightFromGround( Pos.vx + ( distX * m_speed * _frames ), Pos.vy ) < -maxHeight )
			{
				// there is an obstacle in the way, increment the path point (hopefully this will resolve the problem)

				m_npcPath.incPath();
			}
			else
			{
				// check for vertical movement

				groundHeight = CGameScene::getCollision()->getHeightFromGround( Pos.vx, Pos.vy, yMovement + 16 );

				if ( groundHeight <= yMovement )
				{
					// groundHeight <= yMovement indicates either just above ground or on or below ground

					moveX = distX * m_speed * _frames;
					moveY = groundHeight;
				}
				else
				{
					// fall

					moveY = yMovement;
				}
			}
		}

		processMovementModifier( _frames, moveX, moveY, moveVel, moveDist );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcDustDevilEnemy::processMovementModifier( int _frames, s32 distX, s32 distY, s32 dist, s16 headingChange )
{
	Pos.vx += distX;
	Pos.vy += distY;

	// sort out draw rotation

	DVECTOR testPos1, testPos2;

	testPos1 = testPos2 = Pos;
	testPos1.vx -= 10;
	testPos2.vx += 10;

	int groundDist = CGameScene::getCollision()->getHeightFromGround( testPos1.vx, testPos1.vy, 16 );

	if ( abs( groundDist ) > 15 )
	{
		m_drawRotation = 0;
		return;
	}

	testPos1.vy += groundDist;

	groundDist = CGameScene::getCollision()->getHeightFromGround( testPos2.vx, testPos2.vy, 16 );

	if ( abs( groundDist ) > 15 )
	{
		m_drawRotation = 0;
		return;
	}

	testPos2.vy += groundDist;

	s32 xDist = testPos2.vx - testPos1.vx;
	s32 yDist = testPos2.vy - testPos1.vy;

	s16 heading = ratan2( yDist, xDist ) & 4095;

	m_drawRotation = heading;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

s32 CNpcDustDevilEnemy::getFrameShift( int _frames )
{
	return( ( _frames << 8 ) >> 2 );
}
