/*=========================================================================

	nsj2back.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef __ENEMY_NSJ2BACK_H__
#include "enemy\nsj2back.h"
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

void CNpcSmallJellyfish2BackgroundEnemy::render()
{
	CPlayer *player = GameScene.getPlayer();

	if ( player->isWearingGlasses() )
	{
		SprFrame = NULL;

		if ( m_isActive )
		{
			CEnemyThing::render();

			// Render
			DVECTOR renderPos;
			DVECTOR origRenderPos;
			DVECTOR	offset = CLevel::getCameraPos();
			int		spriteWidth = m_spriteBank->getFrameWidth( m_frame >> 8 );
			int		spriteHeight = m_spriteBank->getFrameHeight( m_frame >> 8 );

			renderPos.vx = Pos.vx - offset.vx;
			origRenderPos.vx = renderPos.vx;
			renderPos.vx += m_drawOffset.vx;

			renderPos.vy = Pos.vy - offset.vy;
			origRenderPos.vy = renderPos.vy;
			renderPos.vy += m_drawOffset.vy - ( spriteHeight >> 1 );

			CRECT collisionRect = getCollisionArea();
			collisionRect.x1 -= Pos.vx;
			collisionRect.x2 -= Pos.vx;
			collisionRect.y1 -= Pos.vy;
			collisionRect.y2 -= Pos.vy;

			if ( renderPos.vx + collisionRect.x2 >= 0 && renderPos.vx + collisionRect.x1 <= VidGetScrW() )
			{
				if ( renderPos.vy + collisionRect.y2 >= 0 && renderPos.vy + collisionRect.y1 <= VidGetScrH() )
				{
					SprFrame = m_spriteBank->printRotatedScaledSprite( FRM_JELLYFISH1_SWIM1 + ( m_frame >> 8 ),renderPos.vx,renderPos.vy,4096,4096,m_drawRotation,15);
					setRGB0( SprFrame, 128, 128, 255 );			// Let me know if these change! ALso ket me know when the different coloured ones go in pls! (pkg)

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

					XMax -= origRenderPos.vx;
					XMin -= origRenderPos.vx;
					YMax -= origRenderPos.vy;
					YMin -= origRenderPos.vy;

					setCollisionSize( ( XMax - XMin ), ( YMax - YMin ) );
					setCollisionCentreOffset( ( XMax + XMin ) >> 1, ( YMax + YMin ) >> 1 );
				}
			}
		}
	}
}
