/*=========================================================================

	nsdart.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __ENEMY_NPC_H__
#include "enemy\npc.h"
#endif

#ifndef __ENEMY_NSDART_H__
#include "enemy\nsdart.h"
#endif

#ifndef __GAME_GAME_H__
#include	"game\game.h"
#endif

#ifndef __VID_HEADER_
#include "system\vid.h"
#endif

#ifndef __PROJECTL_PRNPCSPR_H__
#include "projectl\prnpcspr.h"
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcSquidDartEnemy::render()
{
	SprFrame = NULL;

	if ( m_isActive )
	{
		CEnemyThing::render();

		if (canRender())
		{
			DVECTOR &renderPos=getRenderPos();

			int frame = FRM_SQUIDDART_SWIM0001 + ( m_frame >> 8 );

			SprFrame = m_spriteBank->printFT4(frame,renderPos.vx,renderPos.vy,m_reversed,0,10);
			setRGB0( SprFrame, 255, 128, 255 );

			/*s32 XMax = SprFrame->x1 - origRenderPos.vx;
			s32 XMin = SprFrame->x0 - origRenderPos.vx;

			s32 YMax = SprFrame->y2 - origRenderPos.vy;
			s32 YMin = SprFrame->y0 - origRenderPos.vy;*/

			s32 XMax = SprFrame->x1 - renderPos.vx;
			s32 XMin = SprFrame->x0 - renderPos.vx;

			s32 YMax = SprFrame->y2 - renderPos.vy;
			s32 YMin = SprFrame->y0 - renderPos.vy;

			setCollisionSize( ( XMax - XMin ), ( YMax - YMin ) );
			setCollisionCentreOffset( ( XMax + XMin ) >> 1, ( YMax + YMin ) >> 1 );
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcSquidDartEnemy::fireAsProjectile( s16 heading )
{
	m_isActive = false;
	setToShutdown();

	DVECTOR newPos = Pos;

	newPos.vy -= 10;

	CEnemyAsSpriteProjectile *projectile;
	projectile = new( "blower projectile" ) CEnemyAsSpriteProjectile;
	projectile->init(	newPos,
						heading,
						CPlayerProjectile::PLAYER_PROJECTILE_DUMBFIRE,
						CPlayerProjectile::PLAYER_PROJECTILE_FINITE_LIFE,
						5*60);
	projectile->setLayerCollision( m_layerCollision );
	projectile->setGraphic( FRM_SQUIDDART_SWIM0001 );
	projectile->setHasRGB( true );
	projectile->setRGB( 255, 128, 255 );
}