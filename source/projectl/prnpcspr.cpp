/*=========================================================================

	prnpcspr.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef	__PROJECTL_PRNPCSPR_H__
#include "projectl\prnpcspr.h"
#endif

#ifndef __VID_HEADER_
#include "system\vid.h"
#endif

#ifndef __LEVEL_LEVEL_H__
#include "level\level.h"
#endif

#include	"game/game.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CEnemyAsSpriteProjectile	*CEnemyAsSpriteProjectile::Create()
{
	CEnemyAsSpriteProjectile *projectile;

	projectile = (CEnemyAsSpriteProjectile*)CThingManager::GetThing(TYPE_PLAYERPROJECTILE,1);
	if ( !projectile )
	{
		projectile = new ("player projectile") CEnemyAsSpriteProjectile;
	}

	ASSERT(projectile);
	projectile->setThingSubType(1);

	return( projectile );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CEnemyAsSpriteProjectile::think( int _frames )
{
	CPlayerProjectile::think( _frames );

	m_rotation += 512;
	m_rotation &= 4095;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CEnemyAsSpriteProjectile::setGraphic( int frame )
{
	m_spriteFrame = frame;
	m_rotation = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CEnemyAsSpriteProjectile::setRGB( int R, int G, int B )
{
	m_R = R;
	m_G = G;
	m_B = B;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CEnemyAsSpriteProjectile::render()
{
	sFrameHdr	*frameHdr;
	int		x,y;
	int		scrnWidth = VidGetScrW();
	int		scrnHeight = VidGetScrH();
	int		spriteWidth = CGameScene::getSpriteBank()->getFrameWidth( m_spriteFrame );
	int		spriteHeight = CGameScene::getSpriteBank()->getFrameHeight( m_spriteFrame );

	DVECTOR const &offset = CLevel::getCameraPos();

	x = Pos.vx - offset.vx;
	y = Pos.vy - offset.vy;

	if ( x < -spriteWidth || y < -spriteHeight || x > scrnWidth || y > scrnHeight )
	{
		return;
	}

	frameHdr = CGameScene::getSpriteBank()->getFrameHeader( m_spriteFrame );
	POLY_FT4 *SprFrame = CGameScene::getSpriteBank()->printRotatedScaledSprite( frameHdr, x, y, 4096, 4096, m_rotation, 0 );

	if ( m_hasRGB )
	{
		setRGB0( SprFrame, m_R, m_G, m_B );
	}
}
