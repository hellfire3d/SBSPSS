/*=========================================================================

	fgary.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef __FRIEND_FGARY_H__
#include "friend\fgary.h"
#endif

#ifndef __GAME_GAME_H__
#include "game\game.h"
#endif


void CNpcGaryFriend::think( int _frames )
{
	CNpcFriend::think(_frames);

	s8 multiplier = -1 + ( 2 * m_extension );
	s32 maxHeight = 20;
	s32 fallSpeed = 3;
	s8 yMovement = fallSpeed * _frames;
	s8 groundHeight;

	// check vertical collision

	groundHeight = CGameScene::getCollision()->getHeightFromGround( Pos.vx, Pos.vy, yMovement + 16 );

	if ( groundHeight <= 0 )
	{
		// groundHeight <= 0  indicates either on ground or below ground

		// check horizontal collision

		if ( CGameScene::getCollision()->getHeightFromGround( Pos.vx + ( multiplier * _frames ), Pos.vy ) < -maxHeight )
		{
			// reverse direction

			m_extension = !m_extension;
		}
		else
		{
			// make sure we are on the ground, not below it

			Pos.vy += groundHeight;

			Pos.vx += multiplier * _frames;
		}
	}
	else
	{
		// above ground

		if ( groundHeight < yMovement )
		{
			// colliding with ground

			Pos.vy += groundHeight;

			if ( CGameScene::getCollision()->getHeightFromGround( Pos.vx + ( multiplier * _frames ), Pos.vy ) < -maxHeight )
			{
				// reverse direction

				m_extension = !m_extension;
			}
			else
			{
				Pos.vx += multiplier * _frames;
			}
		}
		else
		{
			Pos.vy += yMovement;
		}
	}
}