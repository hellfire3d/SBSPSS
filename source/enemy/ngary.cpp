/*=========================================================================

	ngary.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef __ENEMY_NPC_H__
#include "enemy\npc.h"
#endif

#ifndef __GAME_GAME_H__
#include "game\game.h"
#endif


void CNpc::processGaryMovement( int _frames )
{
	s8 multiplier = -1 + ( 2 * m_extension );
	s32 maxHeight = 10;
	s32 fallSpeed = 5;

	// check vertical collision

	if ( isCollisionWithGround() )
	{
		// check horizontal collision

		if ( m_layerCollision->Get( ( Pos.vx + ( multiplier * _frames ) ) >> 4, ( Pos.vy - maxHeight ) >> 4 ) )
		{
			// reverse direction

			m_extension = !m_extension;
		}
		else
		{
			s32 distY;
			s32 lastPointY = 0;

			for ( distY = 0 ; distY <= maxHeight ; distY++ )
			{
				if ( !m_layerCollision->Get( Pos.vx >> 4, ( Pos.vy - distY ) >> 4 ) )
				{
					break;
				}
				else
				{
					lastPointY--;
				}
			}

			Pos.vy += lastPointY;

			Pos.vx += multiplier * _frames;
		}
	}
	else
	{
		if ( m_layerCollision->Get( Pos.vx >> 4, ( Pos.vy + ( fallSpeed * _frames ) ) >> 4 ) )
		{
			s32 distY;
			s32 lastPointY = 0;

			for ( distY = 1 ; distY <= _frames ; distY++ )
			{
				if ( m_layerCollision->Get( Pos.vx >> 4, ( Pos.vy + distY ) >> 4 ) )
				{
					break;
				}
				else
				{
					lastPointY++;
				}
			}

			Pos.vy += lastPointY;
		}
		else
		{
			Pos.vy += fallSpeed * _frames;
		}
	}
}