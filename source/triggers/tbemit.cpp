/*=========================================================================

	tbemit.cpp

	Author:		CRB
	Created:	
	Project:	Spongebob
	Purpose:	

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__TRIGGERS_TBEMIT_H__
#include "triggers\tbemit.h"
#endif

#ifndef __GAME_GAME_H__
#include	"game\game.h"
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void	CBubbleEmitterTrigger::think(int _frames)
{
	CTrigger::think( _frames );

	if ( m_timer <= 0 )
	{
#ifndef __USER_paul__
		CFX::Create( CFX::FX_TYPE_BUBBLE_WATER, Pos );
#endif

		m_timer = GameState::getOneSecondInFrames() >> 2;
	}
	else
	{
		m_timer -= _frames;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void	CBubbleEmitterTrigger::setPositionAndSize(int _x,int _y,int _w,int _h)
{
	CTrigger::setPositionAndSize( _x, _y, _w, _h );

	m_timer = 0;
}
