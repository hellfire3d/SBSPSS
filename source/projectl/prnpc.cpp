/*=========================================================================

	prnpc.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef	__PROJECTL_PRNPC_H__
#include "projectl\prnpc.h"
#endif

#ifndef __VID_HEADER_
#include "system\vid.h"
#endif

#ifndef __LEVEL_LEVEL_H__
#include "level\level.h"
#endif

void CEnemyAsProjectile::think( int _frames )
{
	CPlayerProjectile::think( _frames );

	m_rotation += 512;
	m_rotation &= 4095;
}

void CEnemyAsProjectile::setGraphic( CActorGfx *graphic )
{
	m_actorGfx = graphic;
	m_rotation = 0;
}

void CEnemyAsProjectile::render()
{
	SprFrame = NULL;

	// Render
	DVECTOR renderPos;
	DVECTOR const &offset = CLevel::getCameraPos();

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
			SprFrame = m_actorGfx->Render(renderPos,0,0,0);
			m_actorGfx->RotateScale( SprFrame, renderPos, m_rotation, 4096, 4096 );
		}
	}
}
