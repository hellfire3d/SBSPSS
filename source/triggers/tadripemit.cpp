/*=========================================================================

	tadripemit.cpp

	Author:		CRB
	Created:	
	Project:	Spongebob
	Purpose:	

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__TRIGGERS_TADRIPEMIT_H__
#include "triggers\tadripemit.h"
#endif

#ifndef __GAME_GAME_H__
#include	"game\game.h"
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CAcidDripEmitterTrigger::think(int _frames)
{
	CTrigger::think( _frames );

	if ( m_timer <= 0 )
	{
		CFX::Create( CFX::FX_TYPE_DROP_ACID, Pos );

		m_timer = GameState::getOneSecondInFrames();
	}
	else
	{
		m_timer -= _frames;
	}
}
