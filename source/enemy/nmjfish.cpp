/*=========================================================================

	nmjfish.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef __ENEMY_NPC_H__
#include "enemy\npc.h"
#endif

#ifndef	__ENEMY_NMJFISH_H__
#include "enemy\nmjfish.h"
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

#define MJ_CYCLE_WIDTH			400
#define MJ_HALF_CYCLE_WIDTH		( MJ_CYCLE_WIDTH >> 1 )


void CNpcMotherJellyfishEnemy::postInit()
{
	m_state = MOTHER_JELLYFISH_RETURN_TO_START_1;
}

void CNpcMotherJellyfishEnemy::processMovement( int _frames )
{
	s32 xDist, yDist;
	s32 xDistSqr, yDistSqr;

	switch( m_state )
	{
		case MOTHER_JELLYFISH_RETURN_TO_START_1:
		case MOTHER_JELLYFISH_RETURN_TO_START_2:
		case MOTHER_JELLYFISH_RETURN_TO_START_3:
		{
			xDist = m_base.vx - this->Pos.vx - MJ_HALF_CYCLE_WIDTH;
			xDistSqr = xDist * xDist;
			yDist = m_base.vy - this->Pos.vy;
			yDistSqr = yDist * yDist;

			if ( xDistSqr + yDistSqr > 100 )
			{
				processGenericGotoTarget( _frames, xDist, yDist, m_data[m_type].speed );
			}
			else
			{
				// have arrived at base position

				m_movementTimer = GameState::getOneSecondInFrames() * 10;
				m_state++;
				m_extension = -MJ_HALF_CYCLE_WIDTH;
				m_extendDir = EXTEND_RIGHT;
			}

			break;
		}

		case MOTHER_JELLYFISH_CYCLE_1:
		case MOTHER_JELLYFISH_CYCLE_2:
		case MOTHER_JELLYFISH_CYCLE_3:
		{
			m_movementTimer -= _frames;

			s32 xExtension;

			if ( m_extendDir == EXTEND_RIGHT )
			{
				if ( m_extension < MJ_HALF_CYCLE_WIDTH )
				{
					m_extension += 3 * _frames;

					xExtension = ( MJ_HALF_CYCLE_WIDTH * rsin( ( m_extension << 10 ) / MJ_HALF_CYCLE_WIDTH ) ) >> 12;

					Pos.vx = m_base.vx + xExtension;
					Pos.vy = m_base.vy - ( ( 50 * rsin( ( xExtension << 12 ) / MJ_CYCLE_WIDTH ) ) >> 12 );

					m_heading = 0;
				}
				else
				{
					m_extendDir = EXTEND_LEFT;

					if ( m_movementTimer < 0 )
					{
						m_controlFunc = NPC_CONTROL_CLOSE;
						m_state++;
					}
				}
			}
			else
			{
				if ( m_extension > -MJ_HALF_CYCLE_WIDTH )
				{
					m_extension -= 3 * _frames;

					xExtension = ( MJ_HALF_CYCLE_WIDTH * rsin( ( m_extension << 10 ) / MJ_HALF_CYCLE_WIDTH ) ) >> 12;

					Pos.vx = m_base.vx + xExtension;
					Pos.vy = m_base.vy + ( ( 50 * rsin( ( xExtension << 12 ) / MJ_CYCLE_WIDTH ) ) >> 12 );

					m_heading = 2048;
				}
				else
				{
					m_extendDir = EXTEND_RIGHT;

					if ( m_movementTimer < 0 )
					{
						m_controlFunc = NPC_CONTROL_CLOSE;
						m_state++;
					}
				}
			}

			break;
		}

		default:
			break;
	}
}

void CNpcMotherJellyfishEnemy::processClose( int _frames )
{
	switch( m_state )
	{
		case MOTHER_JELLYFISH_ATTACK_PLAYER_SHOCK:
		{
			// seek position above user

			CPlayer *player = GameScene.getPlayer();
			DVECTOR playerPos = player->getPos();
			DVECTOR seekPos;
			s32 xDist, yDist;
			s32 xDistSqr, yDistSqr;

			seekPos = playerPos;
			seekPos.vy -= 100;

			xDist = seekPos.vx - this->Pos.vx;
			xDistSqr = xDist * xDist;
			yDist = seekPos.vy - this->Pos.vy;
			yDistSqr = yDist * yDist;

			if ( xDistSqr + yDistSqr > 400 )
			{
				processGenericGotoTarget( _frames, xDist, yDist, m_data[m_type].speed );
			}
			else
			{
				// fire at user

				m_controlFunc = NPC_CONTROL_MOVEMENT;
				m_state = MOTHER_JELLYFISH_RETURN_TO_START_1;
			}

			break;
		}

		default:
		{
			s32 xExtension;

			if ( m_extendDir == EXTEND_RIGHT )
			{
				if ( m_extension < MJ_HALF_CYCLE_WIDTH )
				{
					m_extension += 3 * _frames;

					xExtension = ( MJ_HALF_CYCLE_WIDTH * rsin( ( m_extension << 10 ) / MJ_HALF_CYCLE_WIDTH ) ) >> 12;

					Pos.vx = m_base.vx + xExtension;
					Pos.vy = m_base.vy - ( ( 50 * rcos( ( xExtension << 11 ) / MJ_CYCLE_WIDTH ) ) >> 12 );

					m_heading = 0;
				}
				else
				{
					m_controlFunc = NPC_CONTROL_MOVEMENT;
					m_state++;
				}
			}
			else
			{
				if ( m_extension > -MJ_HALF_CYCLE_WIDTH )
				{
					m_extension -= 3 * _frames;

					xExtension = ( MJ_HALF_CYCLE_WIDTH * rsin( ( m_extension << 10 ) / MJ_HALF_CYCLE_WIDTH ) ) >> 12;

					Pos.vx = m_base.vx + xExtension;
					Pos.vy = m_base.vy + ( ( 50 * rcos( ( xExtension << 11 ) / MJ_CYCLE_WIDTH ) ) >> 12 );

					m_heading = 2048;
				}
				else
				{
					m_controlFunc = NPC_CONTROL_MOVEMENT;
					m_state++;
				}
			}

			break;
		}
	}
}

/*void CNpcMotherJellyfishEnemy::render()
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

		if ( renderPos.vx >= 0 && renderPos.vx <= VidGetScrW() )
		{
			if ( renderPos.vy >= 0 && renderPos.vy <= VidGetScrH() )
			{
				SprFrame = m_spriteBank->printRotatedScaledSprite( FRM_JELLYFISH1_SWIM1,renderPos.vx,renderPos.vy,4096,4096,0,10);
				setRGB0( SprFrame, 255, 128, 255 );

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
}*/
