/*=========================================================================

	projectl.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef	__PROJECTL_PROJECTL_H__
#include "projectl\projectl.h"
#endif

#ifndef __GFX_SPRBANK_H__
#include "gfx\sprbank.h"
#endif

#ifndef __LEVEL_LEVEL_H__
#include "level\level.h"
#endif

#ifndef __FILE_EQUATES_H__
#include <biglump.h>
#endif

#ifndef __SPR_UIGFX_H__
#include <uigfx.h>
#endif

#ifndef __VID_HEADER_
#include "system\vid.h"
#endif

#ifndef __GAME_GAME_H__
#include "game\game.h"
#endif


/*****************************************************************************/

void CProjectile::init()
{
	m_spriteBank=new ("projectile sprites") SpriteBank();
	m_spriteBank->load(UI_UIGFX_SPR);

	m_heading = 0;
	m_lifetime = GameState::getOneSecondInFrames() * 2;
}

void CProjectile::init( DVECTOR initPos, s16 initHeading )
{
	init();

	m_heading = initHeading;
	Pos = initPos;
}

void CProjectile::shutdown()
{
	m_spriteBank->dump();		delete m_spriteBank;
}

void CProjectile::think(int _frames)
{
	Pos.vx += ( _frames * 3 * rcos( m_heading ) ) >> 12;
	Pos.vy += ( _frames * 3 * rsin( m_heading ) ) >> 12;

	m_lifetime -= _frames;

	if ( m_lifetime <= 0 )
	{
		shutdown();
		delete this;
	}
}

void CProjectile::render()
{
	DVECTOR	offset;
	int		x,y;
	int		scrnWidth = VidGetScrW();
	int		scrnHeight = VidGetScrH();
	int		spriteWidth = m_spriteBank->getFrameWidth(FRM_BARNACLEBOY);
	int		spriteHeight = m_spriteBank->getFrameHeight(FRM_BARNACLEBOY);

	offset = getScreenOffset();

	x = Pos.vx - offset.vx + ( scrnWidth >> 1 ) - ( spriteWidth >> 1 );
	y = Pos.vy - offset.vy + ( scrnHeight >> 1 ) - ( spriteHeight >> 1 );

	m_spriteBank->printFT4(FRM_BARNACLEBOY,x,y,0,0,0);
}

DVECTOR CProjectile::getScreenOffset()
{
	return CLevel::getCameraPos();
}

void CProjectile::processEvent( GAME_EVENT evt, CThing *sourceThing )
{
}

/*****************************************************************************/
