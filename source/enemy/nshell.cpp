/*=========================================================================

	nshell.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __ENEMY_NSHELL_H__
#include "enemy\nshell.h"
#endif

#ifndef __SPR_SPRITES_H__
#include <sprites.h>
#endif

#ifndef	__UTILS_HEADER__
#include	"utils\utils.h"
#endif

#include	"game/game.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcShellEnemy::postInit()
{
	CNpcEnemy::postInit();

	m_shellType = FRM_SHELL_STATIC0000 + ( getRnd() % ( FRM_SHELL_STATIC0002 - FRM_SHELL_STATIC0000 + 1 ) );

	Pos.vy -= 20;
	m_base.vy -= 20;
	m_initPos.vy -= 20;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcShellEnemy::render()
{
	SprFrame = NULL;

	if ( m_isActive )
	{
		CEnemyThing::render();

		if (canRender())
		{
			DVECTOR &renderPos=getRenderPos();

			SprFrame = CGameScene::getSpriteBank()->printRotatedScaledSprite( m_shellType,renderPos.vx,renderPos.vy,4096,4096,m_drawRotation,10);

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

void CNpcShellEnemy::collidedWith( CThing *_thisThing )
{
	// do nothing
}
