/*=========================================================================

	tabemit.cpp

	Author:		CRB
	Created:	
	Project:	Spongebob
	Purpose:	

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__TRIGGERS_TABEMIT_H__
#include "triggers\tabemit.h"
#endif

#ifndef __GAME_GAME_H__
#include	"game\game.h"
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void	CAcidBubbleEmitterTrigger::think(int _frames)
{
	CTrigger::think( _frames );

	if ( m_timer <= 0 )
	{
		CFX::Create( CFX::FX_TYPE_BUBBLE_ACID, Pos );

		m_timer = GameState::getOneSecondInFrames() >> 2;
	}
	else
	{
		m_timer -= _frames;
	}
}
