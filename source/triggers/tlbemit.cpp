/*=========================================================================

	tlbemit.cpp

	Author:		CRB
	Created:	
	Project:	Spongebob
	Purpose:	

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__TRIGGERS_TLBEMIT_H__
#include "triggers\tlbemit.h"
#endif

#ifndef __GAME_GAME_H__
#include	"game\game.h"
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void	CLavaBubbleEmitterTrigger::think(int _frames)
{
	CTrigger::think( _frames );

	if ( m_timer <= 0 )
	{
		CFX::Create( CFX::FX_TYPE_BUBBLE_LAVA, Pos );

		m_timer = GameState::getOneSecondInFrames() >> 2;
	}
	else
	{
		m_timer -= _frames;
	}
}
