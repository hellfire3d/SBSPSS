/*=========================================================================

	toilgeysor

	Author:		Dave - based on the original story by CB
	Created:	
	Project:	Spongebob
	Purpose:	

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__TRIGGERS_TOILGEYSER_H__
#include "triggers\toilgeyser.h"
#endif

#ifndef __GAME_GAME_H__
#include	"game\game.h"
#endif

#include	"fx/fxgeyser.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void	COilGeyserTrigger::think(int _frames)
{
	CTrigger::think( _frames );

	if ( m_timer <= 0 )
	{
		m_FX->setHeight(m_Height);
		m_timer = GameState::getOneSecondInFrames() *5;
		m_Height^=128;
	}
	else
	{
		m_timer -= _frames;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void	COilGeyserTrigger::setPositionAndSize(int _x,int _y,int _w,int _h)
{
	CTrigger::setPositionAndSize( _x, _y, _w, _h );
	m_FX=(CFXGeyser*)CFX::Create( CFX::FX_TYPE_GEYSER_OIL, Pos );
	m_FX->setHeight(0);
	m_timer = 0;
	m_Height=128;
}
