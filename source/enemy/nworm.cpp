/*=========================================================================

	nworm.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __ENEMY_NPC_H__
#include "enemy\npc.h"
#endif

//#ifndef __GAME_GAME_H__
//#include "game\game.h"
//#endif

#ifndef	__PLAYER_PLAYER_H__
#include	"player\player.h"
#endif

void CNpcEnemy::processParasiticWormMovement( int _frames )
{
	s32 moveX = 0, moveY = 0;
	s32 moveVel = 0;
	s32 moveDist = 0;

	processGenericFixedPathMove( _frames, &moveX, &moveY, &moveVel, &moveDist );

	Pos.vx += moveX;
	Pos.vy += moveY;
}