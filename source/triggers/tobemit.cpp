/*=========================================================================

	tobemit.cpp

	Author:		CRB
	Created:	
	Project:	Spongebob
	Purpose:	

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__TRIGGERS_TOBEMIT_H__
#include "triggers\tobemit.h"
#endif

#ifndef __GAME_GAME_H__
#include	"game\game.h"
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void	COilBubbleEmitterTrigger::think(int _frames)
{
	CTrigger::think( _frames );

	if ( m_timer <= 0 )
	{
		CFX::Create( CFX::FX_TYPE_BUBBLE_OIL, Pos );

		m_timer = GameState::getOneSecondInFrames() >> 2;
	}
	else
	{
		m_timer -= _frames;
	}
}
