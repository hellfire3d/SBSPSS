/*=========================================================================

	pbranch.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PBRANCH_H__
#include "platform\pbranch.h"
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

#ifndef	__PLAYER_PLAYER_H__
#include	"player\player.h"
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcBranchPlatform::postInit()
{
	sBBox boundingBox = m_modelGfx->GetBBox();
	boundingBox.YMin = ( ( boundingBox.YMin - boundingBox.YMax ) >> 1 ) + boundingBox.YMax + 16;
	m_boundingBox = boundingBox;
	setCollisionSize( ( boundingBox.XMax - boundingBox.XMin ) - 8, ( boundingBox.YMax - boundingBox.YMin ) );
	setCollisionCentreOffset( ( boundingBox.XMax + boundingBox.XMin ) >> 1, ( boundingBox.YMax + boundingBox.YMin ) >> 1 );

	calculateNonRotatedCollisionData();
	setCollisionAngle( m_tiltAngle >> 8 );

	m_angularVelocity = 0;

	if ( m_reversed )
	{
		m_initRotation = 64;
	}
	else
	{
		m_initRotation = -64;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcBranchPlatform::setWaypoints( sThingPlatform *ThisPlatform )
{
	int pointNum;

	u16	*PntList=(u16*)MakePtr(ThisPlatform,sizeof(sThingPlatform));

	u16 initXPos, newXPos, newYPos;

	initXPos = newXPos = (u16) *PntList;
	PntList++;
	newYPos = (u16) *PntList;
	PntList++;

	DVECTOR startPos;
	startPos.vx = ( newXPos << 4 ) + 8;
	startPos.vy = ( newYPos << 4 ) + 16;

	init( startPos );

	if ( ThisPlatform->PointCount > 1 )
	{
		newXPos = (u16) *PntList;

		if ( newXPos < initXPos )
		{
			m_reversed = true;
		}
		else
		{
			m_reversed = false;
		}
	}
	else
	{
		m_reversed = false;
	}

	s32 minX, maxX, minY, maxY;

	m_npcPath.getPathXExtents( &minX, &maxX );
	m_npcPath.getPathYExtents( &minY, &maxY );

	m_thinkArea.x1 = minX;
	m_thinkArea.x2 = maxX;
	m_thinkArea.y1 = minY;
	m_thinkArea.y2 = maxY;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcBranchPlatform::processMovement( int _frames )
{
	s16 newAngle = getCollisionAngle();

	if ( m_contact )
	{
		CPlayer *player = GameScene.getPlayer();

		if ( m_reversed )
		{
			if ( /*m_angularVelocity > 6 &&*/ newAngle < -56 )
			{
				player->springPlayerUp();
			}
		}
		else
		{
			if ( /*m_angularVelocity < -6 &&*/ newAngle > 56 )
			{
				player->springPlayerUp();
			}
		}

		s16 angularForce = 3 * _frames;

		if ( m_reversed )
		{
			angularForce = -angularForce;
		}

		m_angularVelocity += angularForce;
	}
	else if ( ( getRnd() % 50 ) == 0 )
	{
		s16 angularForce = 6 * _frames;

		if ( m_reversed )
		{
			angularForce = -angularForce;
		}

		m_angularVelocity += angularForce;
	}

	s32 resistance = -( 10 * _frames * newAngle ) >> 8;

	if ( newAngle > 0 && resistance > -2 )
	{
		resistance = -2;
	}
	else if ( newAngle < 0 && resistance < 2 )
	{
		resistance = 2;
	}

	// get direction of resistance

	m_angularVelocity += resistance;

	newAngle += m_angularVelocity;

	if ( m_angularVelocity )
	{
		m_angularVelocity += -m_angularVelocity / abs( m_angularVelocity );

		if ( m_angularVelocity > 40 )
		{
			m_angularVelocity = 40;
		}
		else if ( m_angularVelocity < -40 )
		{
			m_angularVelocity = -40;
		}
	}

	/*if ( newAngle > 320 )
	{
		newAngle = 320;
	}
	else if ( newAngle < -320 )
	{
		newAngle = -320;
	}*/

	setCollisionAngle( newAngle );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int	BX=-16;
int	BY=+18;
void CNpcBranchPlatform::render()
{
	if ( m_isActive )
	{
		CPlatformThing::render();

		if (canRender())
		{
			DVECTOR &renderPos=getRenderPos();
			SVECTOR rotation;
			rotation.vx = 0;
			if ( m_reversed )
			{
				rotation.vy = 0;
				rotation.vz = getCollisionAngle();
				renderPos.vx-= BX;
				renderPos.vy+= BY;
			}
			else
			{
				rotation.vy = 0;
				rotation.vz = getCollisionAngle();
				renderPos.vx+= BX;
				renderPos.vy+= BY;
			}

			VECTOR scale;
			scale.vx = ONE;
			scale.vy = ONE;
			scale.vz = ONE;

			m_modelGfx->Render(renderPos,&rotation,&scale);
		}
	}
}
