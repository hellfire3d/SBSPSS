/*=========================================================================

	tldripemit.cpp

	Author:		CRB
	Created:	
	Project:	Spongebob
	Purpose:	

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__TRIGGERS_TLDRIPEMIT_H__
#include "triggers\tldripemit.h"
#endif

#ifndef __GAME_GAME_H__
#include	"game\game.h"
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CLavaDripEmitterTrigger::think(int _frames)
{
	CTrigger::think( _frames );

	if ( m_timer <= 0 )
	{
		CFX::Create( CFX::FX_TYPE_DROP_LAVA, Pos );

		m_timer = GameState::getOneSecondInFrames();
	}
	else
	{
		m_timer -= _frames;
	}
}
