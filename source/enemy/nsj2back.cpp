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

void CNpcSmallJellyfish2BackgroundEnemy::postInit()
{
	CNpcEnemy::postInit();

	m_RGB = 128 + ( 128 << 8 ) + ( 255 << 16 );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcSmallJellyfish2BackgroundEnemy::render()
{
	CPlayer *player = GameScene.getPlayer();

//	if ( player->isWearingGlasses() )		(PKG)
	{
		SprFrame = NULL;

		if ( m_isActive )
		{
			CEnemyThing::render();

			if (canRender())
			{
				DVECTOR &renderPos=getRenderPos();

				SprFrame = CGameScene::getSpriteBank()->printRotatedScaledSprite( FRM_JELLYFISH1_SWIM1 + ( m_frame >> 8 ),renderPos.vx,renderPos.vy,4096,4096,m_drawRotation,15);
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

				XMax -= renderPos.vx;
				XMin -= renderPos.vx;
				YMax -= renderPos.vy;
				YMin -= renderPos.vy;

				setCollisionSize( ( XMax - XMin ), ( YMax - YMin ) );
				setCollisionCentreOffset( ( XMax + XMin ) >> 1, ( YMax + YMin ) >> 1 );
			}
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcSmallJellyfish2BackgroundEnemy::collidedWith( CThing *_thisThing )
{
	if ( m_isActive && !m_isCaught && !m_isDying )
	{
		switch(_thisThing->getThingType())
		{
			case TYPE_PLAYER:
			{
				CPlayer *player = (CPlayer *) _thisThing;

				ATTACK_STATE playerState = player->getAttackState();

				if(playerState==ATTACK_STATE__NONE)
				{
					if ( !player->isRecoveringFromHit() )
					{
						switch( m_data[m_type].detectCollision )
						{
							case DETECT_NO_COLLISION:
							{
								// ignore

								break;
							}

							case DETECT_ALL_COLLISION:
							{
								if ( m_controlFunc != NPC_CONTROL_COLLISION )
								{
									m_oldControlFunc = m_controlFunc;
									m_controlFunc = NPC_CONTROL_COLLISION;
								}

								processUserCollision( _thisThing );

								break;
							}

							case DETECT_ATTACK_COLLISION_GENERIC:
							{
								processAttackCollision();
								processUserCollision( _thisThing );

								break;
							}
						}
					}
				}

				break;
			}

			default:
				break;
		}
	}
}
