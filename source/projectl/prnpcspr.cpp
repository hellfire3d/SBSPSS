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

void CEnemyAsSpriteProjectile::think( int _frames )
{
	CPlayerProjectile::think( _frames );

	m_rotation += 512;
	m_rotation &= 4095;
}

void CEnemyAsSpriteProjectile::setGraphic( int frame )
{
	m_spriteFrame = frame;
}

void CEnemyAsSpriteProjectile::setRGB( int R, int G, int B )
{
	m_R = R;
	m_G = G;
	m_B = B;
}

void CEnemyAsSpriteProjectile::render()
{
	sFrameHdr	*frameHdr;
	DVECTOR	offset;
	int		x,y;
	int		scrnWidth = VidGetScrW();
	int		scrnHeight = VidGetScrH();
	int		spriteWidth = m_spriteBank->getFrameWidth( m_spriteFrame );
	int		spriteHeight = m_spriteBank->getFrameHeight( m_spriteFrame );

	offset = getScreenOffset();

	x = Pos.vx - offset.vx;
	y = Pos.vy - offset.vy;

	if ( x < -spriteWidth || y < -spriteHeight || x > scrnWidth || y > scrnHeight )
	{
		return;
	}

	frameHdr = m_spriteBank->getFrameHeader( m_spriteFrame );
	POLY_FT4 *SprFrame = m_spriteBank->printRotatedScaledSprite( frameHdr, x, y, 4096, 4096, m_rotation, 0 );

	if ( m_hasRGB )
	{
		setRGB0( SprFrame, m_R, m_G, m_B );
	}
}
