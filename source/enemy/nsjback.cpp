/*=========================================================================

	nsjback.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef __ENEMY_NSJBACK_H__
#include "enemy\nsjback.h"
#endif

#ifndef __JELLFISH_JELLFISH_H__
#include "jellfish\jellfish.h"
#endif

#ifndef	__UTILS_HEADER__
#include	"utils\utils.h"
#endif

#ifndef __VID_HEADER_
#include "system\vid.h"
#endif

#ifndef __GAME_GAME_H__
#include	"game\game.h"
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcSmallJellyfishBackgroundEnemy::postInit()
{
	CNpcSmallJellyfishEnemy::postInit();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool CNpcSmallJellyfishBackgroundEnemy::processSensor()
{
	// no sensor

	return( false );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcSmallJellyfishBackgroundEnemy::processMovement( int _frames )
{
	// change direction?

	DVECTOR renderPos;
	DVECTOR const &offset = CLevel::getCameraPos();

	renderPos.vx = Pos.vx - offset.vx;
	renderPos.vy = Pos.vy - offset.vy;

	if ( renderPos.vx < -50 || renderPos.vx > VidGetScrW() + 50 ||
			renderPos.vy < -50 || renderPos.vy > VidGetScrH() + 50 )
	{
		setToShutdown();
		return;
	}

	if ( renderPos.vx >= 0 && renderPos.vx <= VidGetScrW() )
	{
		if ( renderPos.vy >= 0 && renderPos.vy <= VidGetScrH() )
		{
			u8 directionChange = getRnd() % 100;

			if ( directionChange == 0 )
			{
				CPlayer *player = GameScene.getPlayer();

				if ( player->isHoldingNet() )
				{
					if ( ( getRnd() % 4 ) == 0 )
					{
						m_targetHeading = ratan2( playerYDist, playerXDist );
					}
					else
					{
						m_targetHeading += -1024 + ( getRnd() % 2049 );
					}
				}
				else
				{
					m_targetHeading += -1024 + ( getRnd() % 2049 );
				}

				m_targetHeading &= 4095;
			}
		}
	}

	s32 moveX, moveY, moveDist, moveVel;

	s16 decDir, incDir;
	s16 maxTurnRate = m_data[m_type].turnSpeed;

	decDir = m_heading - m_targetHeading;

	if ( decDir < 0 )
	{
		decDir += ONE;
	}

	incDir = m_targetHeading - m_heading;

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

	moveVel = ( _frames * m_speed ) << 8;

	/*if ( processGroundCollisionReverse( &moveX, &moveY ) )
	{
		m_targetHeading += 1024;
		m_targetHeading &= 4095;
	}*/

	processMovementModifier( _frames, moveX, moveY, moveVel, moveDist );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcSmallJellyfishBackgroundEnemy::shutdown()
{
	CNpcSmallJellyfishEnemy::shutdown();

	CJellyfishGenerator::decCounter();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcSmallJellyfishBackgroundEnemy::processUserCollision( CThing *thisThing )
{
	DVECTOR const &otherPos = thisThing->getPos();
	DVECTOR otherDelta = thisThing->getPosDelta();

	s32 xDist = Pos.vx - otherPos.vx;
	s32 yDist = Pos.vy - otherPos.vy;

	s16 headingFromTarget = ratan2( yDist, xDist ) & 4095;

	if ( ( xDist > 0 && otherDelta.vx < 0 ) || ( xDist < 0 && otherDelta.vx > 0 ) )
	{
		otherDelta.vx = -otherDelta.vx;
	}

	if ( ( yDist > 0 && otherDelta.vy < 0 ) || ( yDist < 0 && otherDelta.vy > 0 ) )
	{
		otherDelta.vy = -otherDelta.vy;
	}

	if ( !otherDelta.vx && !otherDelta.vy )
	{
		if ( !xDist )
		{
			xDist = 1;
		}

		if ( !yDist )
		{
			yDist = 1;
		}

		otherDelta.vx = ( 1 * xDist ) / abs( xDist );
		otherDelta.vy = ( 1 * yDist ) / abs( yDist );
	}

	Pos.vx += otherDelta.vx;
	Pos.vy += otherDelta.vy;

	m_targetHeading = m_heading = headingFromTarget;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcSmallJellyfishBackgroundEnemy::render()
{
	SprFrame = NULL;

	if ( m_isActive )
	{
		CEnemyThing::render();

		if (canRender())
		{
			DVECTOR &renderPos=getRenderPos();

			SprFrame = CGameScene::getSpriteBank()->printRotatedScaledSprite( FRM_JELLYFISH1_SWIM1 + ( m_frame >> 8 ),renderPos.vx,renderPos.vy,4096,4096,m_drawRotation,15);
			setRGB0( SprFrame, 255, 128, 255 );			// Let me know if these change! ALso ket me know when the different coloured ones go in pls! (pkg)

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

u8 CNpcSmallJellyfishBackgroundEnemy::hasBeenAttacked()
{
	// do nothing

	return( false );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcSmallJellyfishBackgroundEnemy::collidedWith( CThing *_thisThing )
{
	// do nothing
}
